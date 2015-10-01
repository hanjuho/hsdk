#include <hsdk/win/frame/hwnd.h>



using namespace hsdk;
using namespace win::frame;


// grobal
LRESULT CALLBACK hsdk::win::frame::_WndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	//
	i_Hwnd * user;
	user = (i_Hwnd *)GetWindowLongPtr(_hWnd, GWLP_USERDATA);
	if (user)
	{
		user->message_Proc(_msg, _wParam, _lParam);
	}

	//
	switch (_msg)
	{
	case WM_NCCREATE:
		user = (i_Hwnd *)((LPCREATESTRUCT)_lParam)->lpCreateParams;
		SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)(user));
		user->my_vaild = true;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		user->my_vaild = false;
		break;
	}

	//
	return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(i_Hwnd, i_Hwnd)(void)
: m_windowhandle(nullptr), my_vaild(false)
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
	/* [x] */ void)
{
	return m_windowhandle;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(i_Hwnd, void, get_Message)(
	/* [x] */ void)
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
CLASS_REALIZE_FUNC_T(i_Hwnd, bool, is_Valid)(
	/* [x] */ void)
{
	return my_vaild;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(i_Hwnd, initialize)(
	/* [r] */ HINSTANCE _hInstance,
	/* [r] */ const wchar_t * _title,
	/* [r] */ unsigned int _x,
	/* [r] */ unsigned int _y,
	/* [r] */ unsigned int _w,
	/* [r] */ unsigned int _h)
{
	//
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hInstance = _hInstance;
	wcex.lpfnWndProc = _WndProc;
	wcex.lpszClassName = L"DX11PROJ";
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