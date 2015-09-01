#include <hsdk/win/frame/hwnd.h>



using namespace hsdk;
using namespace win::frame;


//--------------------------------------------------------------------------------------
LRESULT CALLBACK _WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//
	switch (msg)
	{
	case WM_NCCREATE:
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)((LPCREATESTRUCT)lParam)->lpCreateParams);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	//
	i_Hwnd * user = (i_Hwnd *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (user)
	{
		user->message_Proc(msg, wParam, lParam);
	}

	//
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(i_Hwnd, i_Hwnd)(void)
: m_windowhandle(nullptr)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(i_Hwnd, i_Hwnd)(void)
{
	// 핸들 파괴.
	DestroyWindow(m_windowhandle);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(i_Hwnd, HWND, get_Hwnd)(
	/* [none] */ void)
{
	return m_windowhandle;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(i_Hwnd, void, get_Message)(
	/* [none] */ void)
{
	// 윈도 메세지 갱신.
	ZeroMemory(&m_msg, sizeof(m_msg));
	while (PeekMessage(&m_msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(i_Hwnd, initialize)(
	/* [in] */ HINSTANCE _hInstance,
	/* [in] */ const wchar_t * _title,
	/* [in] */ unsigned int _x,
	/* [in] */ unsigned int _y,
	/* [in] */ unsigned int _w,
	/* [in] */ unsigned int _h)
{
	//
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hInstance = _hInstance;
	wcex.lpfnWndProc = _WndProc;
	wcex.lpszClassName = TEXT("DX11PROJ");
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wcex);

	// 윈도우 핸들 생성.
	IF_FALSE((m_windowhandle = CreateWindow(
		wcex.lpszClassName,
		_title,
		WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		_x,
		_y,
		_x + _w,
		_y + _h,
		nullptr,
		nullptr,
		_hInstance,
		this)))
	{
		return E_INVALIDARG;
	}

	return S_OK;
}