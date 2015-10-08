#include <hsdk/win/network/tcpserver.h>
#include <hsdk/win/network/tcpproxy.h>



using namespace hsdk;
using namespace win;
using namespace network;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(TCPServer, TCPServer)(void)
: m_Port(0)
{
	// 이벤트를 생성한다
	IF_TRUE((m_Event = WSACreateEvent()) == WSA_INVALID_EVENT)
	{
		throw ADD_FLAG(HSDK_FAIL, WSAGetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(TCPServer, TCPServer)(void)
{
	// 이벤트를 제거한다
	IF_FALSE(WSACloseEvent(m_Event))
	{
		throw ADD_FLAG(HSDK_FAIL, WSAGetLastError());
	}

	// 연결을 종료한다
	HRESULT hr;
	IF_FAILED(hr = close())
	{
		throw hr;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(TCPServer, open)(
	/* [r] */ unsigned int _port)
{
	// 새 소켓 
	SOCKET new_socket;

	// 인터넷 프로토콜 버전 4 (IPv4) 의 스트림 패킷 TPC 소켓을 생성한다
	IF_FAILED(new_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, 0))
	{
		return ADD_FLAG(HSDK_FAIL, WSAGetLastError());
	}

	// 이벤트와 소켓을 연결한다 
	IF_FAILED(WSAEventSelect(new_socket, m_Event, FD_ACCEPT))
	{
		// 가장 최근에 발생한 에러를 유지
		HRESULT hr = ADD_FLAG(HSDK_FAIL, WSAGetLastError());

		// 열린 소켓을 닫음
		closesocket(new_socket);

		// WSAEventSelect의 결과 반환
		return hr;
	}

	// SO_REUSEADDR : 비정상종료된 상태로 아직 커널이 bind정보를 유지하고 있음으로 발생하는 문제를 해결하기 위해
	// 기존에 bind로 할당된 소켓자원을 프로세스가 재 사용할 수 있도록 허락한다.
	INT optval = 1;
	IF_FAILED(setsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR, LPCSTR(&optval), sizeof(optval)))
	{
		// 가장 최근에 발생한 에러를 유지
		HRESULT hr = ADD_FLAG(HSDK_FAIL, WSAGetLastError());

		// 열린 소켓을 닫음
		closesocket(new_socket);

		return hr;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(USHORT(_port));

	// 서버를 OS에 등록.
	IF_FAILED(bind(new_socket, (SOCKADDR *)&addr, sizeof(addr)))
	{
		// 가장 최근에 발생한 에러를 유지
		HRESULT hr = ADD_FLAG(HSDK_FAIL, WSAGetLastError());

		// 열린 소켓을 닫음
		closesocket(new_socket);

		return hr;
	}

	// 클라이언트와 통신하기 위해 소켓을 송신 대기 상태로 만듦.
	IF_FAILED(listen(new_socket, SOMAXCONN))
	{
		// 가장 최근에 발생한 에러를 유지
		HRESULT hr = ADD_FLAG(HSDK_FAIL, WSAGetLastError());

		// 열린 소켓을 닫음
		closesocket(new_socket);

		return hr;
	}

	SOCKET temp_socket;
	temp_socket = m_Socket;

	// 소켓 초기화
	m_Socket = new_socket;
	m_Port = _port;

	// 이전 소켓 소거
	closesocket(temp_socket);

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(TCPServer, close)(
	/* [r] */ void)
{
	// 열린 소켓을 닫음
	closesocket(m_Socket);

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(TCPServer, accept)(
	/* [w] */ i::network::i_Proxy * (&_proxy),
	/* [r] */ unsigned int _time)
{
	HRESULT hr;

	// 소켓을 생성한다.
	SOCKET new_socket;
	unsigned int new_ip;
	IF_FAILED(hr = accept(new_socket, new_ip, _time))
	{
		return hr;
	}

	// 프록시를 생성한다.
	TCPProxy * new_proxy;
	try
	{
		new_proxy = new TCPProxy();
	}
	catch (long hr)
	{
		// 새로 생셩된 열린 소켓을 닫음
		closesocket(new_socket);
		return hr;
	}
	
	// 프록시를 초기화한다.
	IF_FAILED(hr = new_proxy->initailize(new_socket, new_ip))
	{
		// 열린 소켓을 닫음
		closesocket(new_socket);
		DEL_POINTER(new_proxy);

		return hr;
	}

	// 소켓을 파라미터를 통해 반환
	_proxy = new_proxy;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(TCPServer, reset)(
	/* [x] */ void)
{
	return close() | open(m_Port);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(TCPServer, accept)(
	/* [w] */ SOCKET & _socket,
	/* [w] */ unsigned int & _ip,
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

	// 이벤트가 발생했지만 접속 이벤트인지 확인이 필요하다.
	WSAEnumNetworkEvents(m_Socket, m_Event, LPWSANETWORKEVENTS(&m_Events));

	// 누군가 접속 시도, 다른 이벤트는 허용하지 않음.
	if (IS_FLAG(m_Events.lNetworkEvents, FD_ACCEPT))
	{
		IF_TRUE(hr = m_Events.iErrorCode[FD_ACCEPT_BIT])
		{
			return ADD_FLAG(HSDK_FAIL, hr);
		}
	}
	else
	{
		return ADD_FLAG(HSDK_FAIL, FD_ACCEPT);
	}

	sockaddr_in addr;
	INT	addr_len = sizeof(addr);

	// 접속을 허가한다.
	_socket = ::accept(m_Socket, (sockaddr*)&addr, &addr_len);
	_ip = addr.sin_addr.S_un.S_addr;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(TCPServer, SOCKET, get_Socket)(
	/* [x] */ void)
{
	return m_Socket;
}