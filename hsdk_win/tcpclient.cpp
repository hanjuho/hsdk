#include <hsdk/win/network/tcpclient.h>


using namespace hsdk;
using namespace win;
using namespace network;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(TCPClient, TCPClient)(void)
: m_Url(""), m_Port(0)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(TCPClient, TCPClient)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(TCPClient, connect)(
	/* [r] */ const char * _url,
	/* [r] */ unsigned int _port,
	/* [r] */ unsigned int _wait)
{
	SOCKET new_socket;

	// 서버와 통신 준비를 위해 소켓을 생성
	IF_FAILED(new_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, nullptr, nullptr))
	{
		return ADD_FLAG(HSDK_FAIL, WSAGetLastError());
	}

	// 서버의 주소와 포트
	sockaddr_in server;
	server.sin_family = AF_INET;
	IF_FAILED(inet_pton(AF_INET, _url, &server.sin_addr))
	{
		return ADD_FLAG(HSDK_FAIL, WSAGetLastError());
	}

	server.sin_port = htons(USHORT(_port));
	HRESULT hr;

	// 서버와 연결을 시도
	IF_FAILED(::connect(new_socket, (SOCKADDR *)&server, sizeof(server)))
	{
		// ::connect의 에러 원인 유지
		hr = ADD_FLAG(HSDK_FAIL, WSAGetLastError());
		closesocket(new_socket);

		return hr;
	}
	
	// 대리자 초기화
	IF_FAILED(hr = m_Proxy.initailize(htonl(INADDR_ANY), new_socket))
	{
		return hr;
	}
	
	m_Url = _url;
	m_Port = _port;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(TCPClient, disconnect)(
	/* [x] */ void)
{
	try
	{
		// none 소켓 초기화
		m_Proxy.~TCPProxy();
	}
	catch (long hr)
	{
		return hr;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(TCPClient, send)(
	/* [r] */ byte * _context,
	/* [r] */ unsigned int _offset,
	/* [r] */ unsigned int _size)
{
	return m_Proxy.send(_context, _offset, _size);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(TCPClient, receive)(
	/* [w] */ byte * (&_context),
	/* [w] */ unsigned int(&_size))
{
	return m_Proxy.receive(_context, _size);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(TCPClient, wait_Recv)(
	/* [r] */ unsigned int _time)
{
	return m_Proxy.wait_Recv(_time);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(TCPClient, reset)(
	/* [x] */ void)
{
	return disconnect() | connect(m_Url, m_Port, 1000);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(TCPClient, SOCKET, get_Socket)(
	/* [x] */ void)
{
	return m_Proxy.get_Socket();
}