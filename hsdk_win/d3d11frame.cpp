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
	// ����� ��ǥ�� ����ؼ� �����ϴ� ���� component���� ���߱� ���� ���� ���� ��ǥ�ʹ� �ٸ���.
	m_d3d11Graphics.form[0] = 0.0f;
	m_d3d11Graphics.form[1] = 0.0f;
	m_d3d11Graphics.form[2] = get_W();
	m_d3d11Graphics.form[3] = get_H();
	m_d3d11Graphics.update();

	// ���� ���̵� �缳��
	D3D11Graphics::set_Wide(m_d3d11Graphics.form);

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
	std::hash_map<unsigned int, i_Component *>::iterator iter = m_Container.begin();
	std::hash_map<unsigned int, i_Component *>::iterator end = m_Container.end();
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