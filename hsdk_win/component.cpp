#include <hsdk/win/frame/component.h>
#include <hsdk/interface/frame/inputeventhelper.h>
#include <hsdk/win/direct3d/d3d10_renderer.h>
#include <hsdk/win/framework.h>



using namespace hsdk::frame;


// grobal
volatile unsigned int component_id = 0;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Component, Component)(
	_In_ PARENT_RELATION _relation)
	: my_Relation(_relation), my_id(component_id++)
{
	my_Rectangle[0] = 0.0f;
	my_Rectangle[1] = 0.0f;
	my_Rectangle[2] = 0.0f;
	my_Rectangle[3] = 0.0f;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(Component, Component)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, hsdk::i::frame::i_Component *, parent)(
	_X_ void)const
{
	return my_Parent;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, hsdk::i::frame::i_Graphics *, graphics)(
	_X_ void)const
{
	return (hsdk::i::frame::i_Graphics *)&m_Graphics;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Component, add_Component)(
	/* [set] */ i_Component * _component,
	_In_ hsdk::i::frame::LAYOUT_COMPOSITION _composition)
{
	return E_NOTIMPL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Component, remove_Component)(
	_In_ i_Component * _component)
{
	return E_NOTIMPL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Component, contain_Component)(
	_In_ i_Component * _component)const
{
	return E_NOTIMPL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Component, get_Component)(
	_Out_ i_Component * (&_component),
	_In_ unsigned int _id)const
{
	return E_NOTIMPL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, unsigned int, get_id)(
	_X_ void)const
{
	return my_id;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, set_X)(
	_In_ float _value)
{
	my_Rectangle[0] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, set_Y)(
	_In_ float _value)
{
	my_Rectangle[1] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, set_W)(
	_In_ float _value)
{
	my_Rectangle[2] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, set_H)(
	_In_ float _value)
{
	my_Rectangle[3] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, float, get_X)(
	_X_ void)const
{
	return my_Rectangle[0];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, float, get_Y)(
	_X_ void)const
{
	return my_Rectangle[1];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, float, get_W)(
	_X_ void)const
{
	return my_Rectangle[2];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, float, get_H)(
	_X_ void)const
{
	return my_Rectangle[3];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Component, set_Visible)(
	_In_ bool _visible)
{
	bool b = my_Visible;
	my_Visible = _visible;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, bool, is_Visible)(
	_X_ void)const
{
	return my_Visible;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, bool, event_chain)(
	_In_ hsdk::i::frame::i_inputEventHelper * _eventhelper)
{
	return _eventhelper->chain(this);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, update)(
	_X_ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, reform)(
	_X_ void)
{
	if (my_Parent)
	{
		m_AbsX = my_Parent->m_AbsX + my_Rectangle[0];
		m_AbsY = my_Parent->m_AbsY + my_Rectangle[1];
	}

	float screenWidth;
	float screenHeigth;
	float x, y, w, h;

	if (my_Relation == PARENT_RELATION_ABSOLUTE)
	{
		x = m_AbsX;
		y = m_AbsY;
	}
	else
	{
		x = my_Rectangle[0];
		y = my_Rectangle[1];
	}

	screenWidth = (float)framework::g_Framework_Window.width;
	screenHeigth = (float)framework::g_Framework_Window.height;
	w = my_Rectangle[2] / screenWidth;
	h = my_Rectangle[3] / screenHeigth;

	D3DXMATRIX t;
	D3DXMatrixTranslation(&t,
		(x / screenWidth * 2.0f) - (1.0f - w),
		(1.0f - h) - (y / screenHeigth * 2.0f), 0.0f);

	D3DXMATRIX s;
	D3DXMatrixScaling(&s, w, h, 0.0f);

	m_Position = s * t;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, render)(
	_X_ void)
{
	if (is_Visible())
	{
		direct3d::g_D3D10_Renderer.set_MatrixWorldViewProj(&m_Position);
		direct3d::g_D3D10_Renderer.set_ScalarPSTime(1.0f);
		if (m_Graphics.texture)
		{
			direct3d::g_D3D10_Renderer.set_ScalarVSFlag(0);
			direct3d::g_D3D10_Renderer.set_ScalarPSFlag(direct3d::PS_TEXTURE_0 | direct3d::PS_CALLFUNCTION_0 | direct3d::PS_TEXMATRIX_0);
			direct3d::g_D3D10_Renderer.render_UITexture(
				m_Graphics.texture,
				&m_Graphics.mTexcoord);
		}
		else
		{
			direct3d::g_D3D10_Renderer.set_ScalarVSFlag(0);
			direct3d::g_D3D10_Renderer.set_ScalarPSFlag(direct3d::PS_MARERIAL_0 | direct3d::PS_CALLFUNCTION_0);
			direct3d::g_D3D10_Renderer.render_UIRectangle(
				&m_Graphics.bgColor);
		}
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, reset)(
	_X_ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, set_Mouseable)(
	_In_ hsdk::i::frame::i_Mouseable * _mouseable)
{
	m_Mouseable = _mouseable;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, hsdk::i::frame::i_Mouseable *, get_Mouseable)(
	_X_ void)const
{
	return m_Mouseable;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, set_Keyboardable)(
	/* [set] */ hsdk::i::frame::i_Keyboardable * _Keyboardable)
{
	m_Keyboardable = _Keyboardable;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, hsdk::i::frame::i_Keyboardable *, get_Keyboardable)(
	_X_ void)const
{
	return m_Keyboardable;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, set_Actable)(
	/* [set] */ hsdk::i::frame::i_Actable * _actable)
{
	m_Actable = _actable;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, hsdk::i::frame::i_Actable *, get_Actable)(
	_X_ void)const
{
	return m_Actable;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, onMouse_Enter)(
	_In_ int _x,
	_In_ int _y)
{
	if (m_Mouseable)
	{
		m_Mouseable->onMouse_Enter(_x, _y);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, onMouse_Exit)(
	_In_ int _x,
	_In_ int _y)
{
	if (m_Mouseable)
	{
		m_Mouseable->onMouse_Exit(_x, _y);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, onClick_Down)(
	_In_ hsdk::i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	if (m_Mouseable)
	{
		m_Mouseable->onClick_Down(_button, _x, _y);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, onClick_Up)(
	_In_ hsdk::i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	if (m_Mouseable)
	{
		m_Mouseable->onClick_Up(_button, _x, _y);
	}
}


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, onDrag)(
	_In_ hsdk::i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	if (m_Mouseable)
	{
		m_Mouseable->onDrag(_button, _x, _y);
	}
}


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, onMove)(
	_In_ int _x,
	_In_ int _y)
{
	if (m_Mouseable)
	{
		m_Mouseable->onMove(_x, _y);
	}
}


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, onWheel)(
	_In_ int _x,
	_In_ int _y,
	_In_ int _w)
{
	if (m_Mouseable)
	{
		m_Mouseable->onWheel(_x, _y, _w);
	}
}


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, onKey_Down)(
	_In_ unsigned char _vKey)
{
	if (m_Keyboardable)
	{
		m_Keyboardable->onKey_Down(_vKey);
	}
}


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, onKey_Up)(
	_In_ unsigned char _vKey)
{
	if (m_Keyboardable)
	{
		m_Keyboardable->onKey_Up(_vKey);
	}
}


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, onAct)(
	_X_ void)
{
	if (m_Actable)
	{
		m_Actable->onAct();
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, float, get_AbsX)(
	_X_ void)const
{
	return m_AbsX;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, float, get_AbsY)(
	_X_ void)const
{
	return m_AbsY;
}

