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
	: D3D11(TEXT(""), _hInstance, _title, _x, _y, _w, _h), m_inputEventHelper(this)
{
	set_X(float(_x));
	set_Y(float(_y));
	set_W(float(_w));
	set_H(float(_h));
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(D3D11Frame, D3D11Frame)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11Frame, void, update)(
	/* [none] */ void)
{
	float rectangle[4] = {
		0.0f,
		0.0f,
		get_W(),
		get_H()
	};

	// 상대적 좌표를 사용해서 연산하는 하위 component에게 맞추기 위해 윈도 실제 좌표와는 다르다.
	m_D3D11Graphics.update_Panel(rectangle);

	// 윈도 와이드 재설정
	D3D11Graphics::set_Wide(rectangle);

	// 윈도 사각형 재설정
	SetWindowPos(
		get_Hwnd(),
		HWND_TOP,
		(long)(get_X()),
		(long)(get_Y()),
		(long)(get_X() + get_W()),
		(long)(get_Y() + get_H()),
		SWP_SHOWWINDOW);

	UpdateWindow(get_Hwnd());

	// 하위 컴포넌트 갱신
	std::hash_map<unsigned int, Component *>::iterator iter = m_Container.begin();
	std::hash_map<unsigned int, Component *>::iterator end = m_Container.end();
	while (iter != end)
	{
		iter->second->update();
		iter++;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11Frame, void, render)(
	/* [none] */ void)
{
	//
	D3D11Graphics::shader_on();

	//
	Container::render();

	//
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
	};
}