#include <hsdk/win/network/tcpproxy.h>


using namespace hsdk;
using namespace win;
using namespace network;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(TCPProxy, TCPProxy)(void)
{
	// 이벤트를 생성한다
	IF_TRUE((m_Event = WSACreateEvent()) == WSA_INVALID_EVENT)
	{
		throw ADD_FLAG(HSDK_FAIL, WSAGetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(TCPProxy, TCPProxy)(void)
{
	// 이벤트를 제거한다
	IF_FALSE(WSACloseEvent(m_Event))
	{
		throw ADD_FLAG(HSDK_FAIL, WSAGetLastError());
	}

	// 소켓을 닫는다.
	HRESULT hr;
	IF_FAILED(hr = shutdown(m_Socket, SD_BOTH))
	{
		IF_FALSE(hr == 0xffffffff)
		{
			throw ADD_FLAG(HSDK_FAIL, WSAGetLastError());
		}
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(TCPProxy, send)(
	/* [r] */ char * _context,
	/* [r] */ unsigned int _offset,
	/* [r] */ unsigned int _size)
{
	// 패킷으로 변환된 정보를 꺼내 송신한다.
	IF_FAILED(::send(m_Socket, &_context[_offset], _size, 0))
	{
		return ADD_FLAG(HSDK_FAIL, WSAGetLastError());
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(TCPProxy, receive)(
	/* [w] */ char * (&_context),
	/* [w] */ unsigned int (&_size))
{
	unsigned int buffer_len;

	// 소켓에 들어있는 정보를 꺼내 수신한다.
	buffer_len = ::recv(m_Socket, _context, 65535, 0);

	// recv 가 실패하면 0xffffffff를 반환함
	IF_FAILED(buffer_len)
	{
		return ADD_FLAG(HSDK_FAIL, WSAGetLastError());
	}
	
	// 패킷의 길이 반환
	_size = buffer_len;

	return S_OK;

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(TCPProxy, wait_Recv)(
	/* [r] */ unsigned int _time)
{
	HRESULT hr;

	// 클라이언트가 접속하기를 기다린다.
	IF_FAILED(hr = WSAWaitForMultipleEvents(1, &m_Event, FALSE, _time, FALSE))
	{
		return ADD_FLAG(HSDK_FAIL, WSAGetLastError());
	}

	// 함수 호출은 성공했지만 타임아웃이다.
	IF_TRUE(hr == WSA_WAIT_TIMEOUT)
	{
		return ADD_FLAG(HSDK_FAIL, WSA_WAIT_TIMEOUT);
	}

	// 이벤트 결과 검색
	WSAEnumNetworkEvents(m_Socket, m_Event, LPWSANETWORKEVENTS(&m_Events));

	// 수신 성공
	IF_TRUE(IS_FLAG(m_Events.lNetworkEvents, FD_READ))
	{
		// 이벤트 유효성 검사 
		IF_TRUE(hr = m_Events.iErrorCode[FD_READ_BIT])
		{
			return ADD_FLAG(HSDK_FAIL, hr);
		}

		// 대기 성공
		return FD_READ_BIT;
	}

	// 종료
	IF_TRUE(IS_FLAG(m_Events.lNetworkEvents, FD_CLOSE))
	{
		// 이벤트 유효성 검사 
		IF_TRUE(hr = m_Events.iErrorCode[FD_CLOSE_BIT])
		{
			return ADD_FLAG(HSDK_FAIL, hr);
		}

		// 접속 단절 이벤트가 호출 되었음에도 실패인 이유는 더 이상 해당 소켓을 사용할 수 없기 때문
		return ADD_FLAG(HSDK_FAIL, FD_CLOSE_BIT);
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(TCPProxy, initailize)(
	/* [r] */ unsigned int _ip,
	/* [r] */ SOCKET _socket)
{
	// 소켓과 이벤트를 연결한다.
	IF_FAILED(WSAEventSelect(_socket, m_Event, FD_READ | FD_CLOSE))
	{
		return ADD_FLAG(HSDK_FAIL, WSAGetLastError());
	}

	// ip
	m_ip = _ip;

	// 소켓을 교체한다.
	SOCKET temp_socket = m_Socket;
	m_Socket = _socket;

	// 이전에 열려있던 소켓을 닫는다.
	closesocket(temp_socket);

	// 크기 재 설정
	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(TCPProxy, SOCKET, get_Socket)(
	/* [x] */ void)
{
	return m_Socket;
}
