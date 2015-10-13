#include <hsdk/win/frame/frame.h>
#include <hsdk/win/frame/graphics.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;


//--------------------------------------------------------------------------------------
// Grobal 
//--------------------------------------------------------------------------------------
direct3d::D3D10_Manager g_Manager;

//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Frame, Frame)(void)
: m_inputEventHelper(this)
{
	HRESULT hr;
	IF_FAILED(hr = Graphics::initialize(g_Manager))
	{
		throw hr;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(Frame, Frame)(void)
{
	g_Manager.destroy();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Frame, void, update)(
	/* [x] */ void)
{
		IF_FALSE(is_FullScreen())
		{
			// 윈도 사각형 재설정
			if (SetWindowPos(
				g_Manager.get_HWND_Focus(),
				HWND_TOP,
				(long)(get_X()),
				(long)(get_Y()),
				0,
				0,
				SWP_NOSIZE))
			{
				g_Manager.change_Monitor(true, (int)get_W(), (int)get_H());
			}
		}

	// 상대적 좌표를 사용해서 연산하는 하위 component에게 맞추기 위해 윈도 실제 좌표와는 다르다.
	if (m_D3D10Graphics)
	{
		m_D3D10Graphics->update({
			0.0f,
			0.0f,
			get_W(),
			get_H() });
	}

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
CLASS_IMPL_FUNC_T(Frame, void, render)(
	/* [x] */ void)
{
	//
	Container::render();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Frame, void, message_Proc)(
	/* [r] */unsigned int _uMsg,
	/* [r] */unsigned int _wParam,
	/* [r] */unsigned long _lParam)
{
	switch (_uMsg)
	{
	case WM_MOVE:
		break;
	case WM_SIZE:
	{
					unsigned int width = LOWORD(_lParam);
					unsigned int height = HIWORD(_lParam);

					if (width != 0 && height != 0)
					{
						if (width != get_W() || height != get_H())
						{
							set_W((float)(width));
							set_H((float)(height));
							update();
						}
					}
	}
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
	};
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Frame, set_Visible)(
	/* [r] */ bool _visible)
{
	if (_visible)
	{
		ShowWindow(g_Manager.get_HWND_Focus(), SW_SHOW);
	}
	else
	{
		ShowWindow(g_Manager.get_HWND_Focus(), SW_HIDE);
	}

	return Container::set_Visible(_visible);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Frame, void, set_FullScreen)(
	/* [r] */ bool _full)
{
	g_Manager.change_Monitor(!_full, 0, 0);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Frame, BOOL, is_FullScreen)(
	/* [x] */ void)const
{
	return !g_Manager.is_Windowed();
}