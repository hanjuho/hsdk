#include <hsdk/win/frame/frame.h>
#include <hsdk/win/frame/graphics.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;


//--------------------------------------------------------------------------------------
// Grobal 
//--------------------------------------------------------------------------------------
const direct3d::Direct3D_State * g_Window_State;
const direct3d::Direct3D_Window * g_Window_Master;

//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Frame, Frame)(void)
: m_inputEventHelper(this), my_FullScreen(false), my_ChangedSize(false)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(Frame, Frame)(void)
{
	PostQuitMessage(0);

	destroy();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Frame, initialize)(
	/* [w] */ direct3d::D3D10_Master * _master)
{
	g_Window_State = _master->get_State();
	g_Window_Master = _master->get_Window();

	return Graphics::initialize(_master);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Frame, void, destroy)(
	/* [x] */ void)
{
	Graphics::destroy();

	g_Window_State = nullptr;
	g_Window_Master = nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Frame, void, update)(
	/* [x] */ void)
{
	IF_FALSE(my_ChangedSize)
	{
		if (g_Window_State->windowed)
		{
			if (my_FullScreen)
			{
				// full to window
				GetWindowRect(g_Window_Master->hwnd, &my_Rect);
			}
			else
			{
				// window to window

				// 윈도 사각형 재설정
				SetWindowPos(
					g_Window_Master->hwnd,
					HWND_TOP,
					(long)get_X(),
					(long)get_Y(),
					(long)get_W(),
					(long)get_H(),
					SWP_NOSIZE);
			}
		}
		else
		{
			if (my_FullScreen)
			{
				// none action full to full
			}
			else
			{
				// window to full

				// 윈도 사각형 재설정
				SetWindowPos(
					g_Window_Master->hwnd,
					HWND_TOP,
					my_Rect.left,
					my_Rect.top,
					my_Rect.right - my_Rect.left,
					my_Rect.bottom - my_Rect.top,
					SWP_NOSIZE);
			}
		}
	}

	my_FullScreen = !g_Window_State->windowed;

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
	case WM_SIZE:
	{		
					unsigned int width = LOWORD(_lParam);
					unsigned int height = HIWORD(_lParam);

					if (width != 0 && height != 0)
					{
						set_W((float)(width));
						set_H((float)(height));

						// external control
						my_ChangedSize = true;
						update();
						my_ChangedSize = false;
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
		ShowWindow(g_Window_Master->hwnd, SW_SHOW);
	}
	else
	{
		ShowWindow(g_Window_Master->hwnd, SW_HIDE);
	}

	return Container::set_Visible(_visible);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Frame, void, set_FullScreen)(
	/* [r] */ BOOL _full)
{
	my_FullScreen = _full;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Frame, BOOL, is_FullScreen)(
	/* [x] */ void)const
{
	return my_FullScreen;
}