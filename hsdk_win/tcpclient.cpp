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
	/* [in] */ const char * _url,
	/* [in] */ unsigned int _port,
	/* [in] */ unsigned int _wait)
{
	SOCKET new_socket;

	// ������ ��� �غ� ���� ������ ����
	IF_FAILED(new_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, NULL))
	{
		return ADD_FLAG(HSDK_FAIL, WSAGetLastError());
	}

	// ������ �ּҿ� ��Ʈ
	sockaddr_in server;
	server.sin_family = AF_INET;
	IF_FAILED(inet_pton(AF_INET, _url, &server.sin_addr))
	{
		return ADD_FLAG(HSDK_FAIL, WSAGetLastError());
	}

	server.sin_port = htons(USHORT(_port));
	HRESULT hr;

	// ������ ������ �õ�
	IF_FAILED(::connect(new_socket, (SOCKADDR *)&server, sizeof(server)))
	{
		// ::connect�� ���� ���� ����
		hr = ADD_FLAG(HSDK_FAIL, WSAGetLastError());
		closesocket(new_socket);

		return hr;
	}
	
	// �븮�� �ʱ�ȭ
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
	/* [none] */ void)
{
	try
	{
		// none ���� �ʱ�ȭ
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
	/* [in] */ byte * _context,
	/* [in] */ unsigned int _offset,
	/* [in] */ unsigned int _size)
{
	return m_Proxy.send(_context, _offset, _size);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(TCPClient, receive)(
	/* [out] */ byte * (&_context),
	/* [out] */ unsigned int(&_size))
{
	return m_Proxy.receive(_context, _size);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(TCPClient, wait_Recv)(
	/* [in] */ unsigned int _time)
{
	return m_Proxy.wait_Recv(_time);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(TCPClient, reset)(
	/* [none] */ void)
{
	return disconnect() | connect(m_Url, m_Port, 1000);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(TCPClient, SOCKET, get_Socket)(
	/* [none] */ void)
{
	return m_Proxy.get_Socket();
}