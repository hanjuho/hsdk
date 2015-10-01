#include <hsdk/win/frame/frame.h>
#include <hsdk/win/frame/graphics.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Frame, Frame)(
	/* [r] */ HINSTANCE _hInstance,
	/* [r] */ const wchar_t * _title,
	/* [r] */ unsigned int _x,
	/* [r] */ unsigned int _y,
	/* [r] */ unsigned int _w,
	/* [r] */ unsigned int _h)
	: D3D11(L"", _hInstance, _title, _x, _y, _w, _h), m_inputEventHelper(this)
{
	HRESULT hr;
	if (FAILED(hr = Graphics::initialize()))
	{
		throw hr;
	}

	set_X(float(_x));
	set_Y(float(_y));
	set_W(float(_w));
	set_H(float(_h));
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(Frame, Frame)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Frame, void, update)(
	/* [x] */ void)
{
	float rectangle[4] = {
		0.0f,
		0.0f,
		get_W(),
		get_H()
	};

	// ����� ��ǥ�� ����ؼ� �����ϴ� ���� component���� ���߱� ���� ���� ���� ��ǥ�ʹ� �ٸ���.
	m_D3D11Graphics.update_Panel(rectangle);

	// ���� ���̵� �缳��
	Graphics::set_Wide(rectangle);

	// ���� �簢�� �缳��
	SetWindowPos(
		get_Hwnd(),
		HWND_TOP,
		(long)(get_X()),
		(long)(get_Y()),
		(long)(get_X() + get_W()),
		(long)(get_Y() + get_H()),
		SWP_SHOWWINDOW);

	UpdateWindow(get_Hwnd());

	// ���� ������Ʈ ����
	std::hash_map<unsigned int, Component *>::iterator iter = m_Container.begin();
	std::hash_map<unsigned int, Component *>::iterator end = m_Container.end();
	while (iter != end)
	{
		iter->second->update();
		iter++;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Frame, void, render)(
	/* [x] */ void)
{
	//
	Graphics::shader_on();

	//
	Container::render();

	//
	Graphics::shader_off();
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Frame, void, message_Proc)(
	/* [r] */unsigned int _uMsg,
	/* [r] */unsigned int _wParam,
	/* [r] */unsigned long _lParam)
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