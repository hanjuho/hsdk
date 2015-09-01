#include <hsdk/win/frame/d3d11frame.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(D3D11Frame, D3D11Frame)(
	/* [in] */ HINSTANCE _hInstance,
	/* [in] */ const wchar_t * _title,
	/* [in] */ unsigned int _x,
	/* [in] */ unsigned int _y,
	/* [in] */ unsigned int _w,
	/* [in] */ unsigned int _h)
	: m_inputEventHelper(this), m_vaild(false)
{
	if (FAILED(i_Hwnd::initialize(_hInstance, _title, _x, _y, _w, _h)))
	{
		throw - 1;
	}

	if (FAILED(D3D11::initialize(get_Hwnd(), nullptr)))
	{
		throw - 1;
	}

	if (FAILED(D3D11Graphics::initialize()))
	{
		throw - 1;
	}

	set_X(float(_x));
	set_Y(float(_y));
	set_W(float(_w));
	set_H(float(_h));
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(D3D11Frame, D3D11Frame)(void)
{
	D3D11Graphics::destroy();
	D3D11::destroy();
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11Frame, void, render)(
	/* [none] */ void)
{
	D3D11Graphics::shader_on();
	Container::render();
	D3D11Graphics::shader_off();
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11Frame, void, message_Proc)(
	/* [in] */unsigned int _uMsg,
	/* [in] */unsigned int _wParam,
	/* [in] */unsigned long _lParam)
{
	switch (_uMsg)
	{
	case WM_CREATE:
		m_vaild = true;
		return;
	case WM_KEYDOWN:
		if ((HIWORD(_lParam) & KF_REPEAT))
		{
			// on_Keyboard_Key_Keep(_wParam);
		}
		else
		{
			// on_Keyboard_Key_Down(_wParam);
		}
		return;
	case WM_KEYUP:
		// on_Keyboard_Key_Up(_wParam);
		return;
	case WM_MOUSEMOVE:
		if (LOWORD(_wParam) & MK_LBUTTON)
		{
			m_inputEventHelper.onDrag(i::frame::i_Mouseable::LBUTTON, LOWORD(_lParam), HIWORD(_lParam));
		}
		else
		{
			m_inputEventHelper.onMove(LOWORD(_lParam), HIWORD(_lParam));
		}
		return;
	case WM_MOUSEWHEEL:
		m_inputEventHelper.onWheel(LOWORD(_lParam), HIWORD(_lParam), _wParam);
		return;
	case WM_LBUTTONDOWN:
		m_inputEventHelper.onClick_Down(i::frame::i_Mouseable::LBUTTON, LOWORD(_lParam), HIWORD(_lParam));
		return;
	case WM_LBUTTONUP:
		m_inputEventHelper.onClick_Up(i::frame::i_Mouseable::LBUTTON, LOWORD(_lParam), HIWORD(_lParam));
		return;
	case WM_RBUTTONDOWN:
		m_inputEventHelper.onClick_Down(i::frame::i_Mouseable::RBUTTON, LOWORD(_lParam), HIWORD(_lParam));
		return;
	case WM_RBUTTONUP:
		m_inputEventHelper.onClick_Up(i::frame::i_Mouseable::RBUTTON, LOWORD(_lParam), HIWORD(_lParam));
		return;
	case WM_MBUTTONDOWN:
		m_inputEventHelper.onClick_Down(i::frame::i_Mouseable::WBUTTON, LOWORD(_lParam), HIWORD(_lParam));
		return;
	case WM_MBUTTONUP:
		m_inputEventHelper.onClick_Up(i::frame::i_Mouseable::WBUTTON, LOWORD(_lParam), HIWORD(_lParam));
		return;
	case WM_DESTROY:
		m_vaild = false;
		return;
	};
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11Frame, bool, is_Valid)(
	/* [none] */ void)
{
	return m_vaild;
}
