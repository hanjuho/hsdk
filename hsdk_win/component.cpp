#include <hsdk/win/frame/component.h>
#include <hsdk/interface/frame/inputeventhelper.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;


// grobal
volatile unsigned int component_id = 0;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Component, Component)(
	_In_ float _x,
	_In_ float _y,
	_In_ float _w,
	_In_ float _h)
	: my_id(component_id++), my_Parent(nullptr), my_AbsX(0.0f), my_AbsY(0.0f), my_Visible(false)
{
	my_Rectangle[0] = _x;
	my_Rectangle[1] = _y;
	my_Rectangle[2] = _w;
	my_Rectangle[3] = _h;

	reform();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(Component, Component)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, i_Component *, parent)(
	/* [x] */ void)const
{
	return my_Parent;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, i_Graphics *, graphics)(
	/* [x] */ void)const
{
	return (i_Graphics *)&m_D3D10Graphics;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Component, add_Component)(
	/* [set] */ i_Component * _component)
{
	return 0x8000000;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Component, remove_Component)(
	_In_ i_Component * _component)
{
	return 0x8000000;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, bool, contain_Component)(
	_In_ i_Component * _component)const
{
	return false;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Component, get_Component)(
	_Out_ i_Component * (&_component),
	_In_ unsigned int _id)const
{
	return 0x8000000;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, unsigned int, get_id)(
	/* [x] */ void)const
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
	/* [x] */ void)const
{
	return my_Rectangle[0];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, float, get_Y)(
	/* [x] */ void)const
{
	return my_Rectangle[1];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, float, get_W)(
	/* [x] */ void)const
{
	return my_Rectangle[2];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, float, get_H)(
	/* [x] */ void)const
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
	/* [x] */ void)const
{
	return my_Visible;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, bool, event_chain)(
	_In_ i_inputEventHelper * _eventhelper)
{
	return _eventhelper->chain(this);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, update)(
	/* [x] */ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, reform)(
	/* [x] */ void)
{
	if (my_Parent)
	{
		my_AbsX = my_Parent->get_AbsX() + get_X();
		my_AbsY = my_Parent->get_AbsY() + get_Y();

		m_D3D10Graphics.update({
			my_AbsX,
			my_AbsY,
			my_AbsX + my_Rectangle[2],
			my_AbsY + my_Rectangle[3] });
	}
	else
	{
		m_D3D10Graphics.update(my_Rectangle);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, render)(
	/* [x] */ void)
{
	if (is_Visible())
	{
		m_D3D10Graphics.render(1.0f);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, reset)(
	/* [x] */ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, set_Mouseable)(
	_In_ i_Mouseable * _mouseable)
{
	m_Mouseable = _mouseable;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, i_Mouseable *, get_Mouseable)(
	/* [x] */ void)const
{
	return m_Mouseable;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, set_Keyboardable)(
	/* [set] */ i_Keyboardable * _Keyboardable)
{
	m_Keyboardable = _Keyboardable;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, i_Keyboardable *, get_Keyboardable)(
	/* [x] */ void)const
{
	return m_Keyboardable;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, set_Actable)(
	/* [set] */ i_Actable * _actable)
{
	m_Actable = _actable;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, i_Actable *, get_Actable)(
	/* [x] */ void)const
{
	return m_Actable;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, void, onClick_Down)(
	_In_ MOUSE_BUTTON _button,
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
	_In_ MOUSE_BUTTON _button,
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
	_In_ MOUSE_BUTTON _button,
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
	/* [x] */ void)
{
	if (m_Actable)
	{
		m_Actable->onAct();
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, float, get_AbsX)(
	/* [x] */ void)const
{
	return my_AbsX;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Component, float, get_AbsY)(
	/* [x] */ void)const
{
	return my_AbsY;
}

