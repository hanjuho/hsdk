#include <hsdk/win/frame/component.h>
#include <hsdk/interface/frame/inputeventhelper.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;


// grobal
unsigned int component_id = 0;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Component, Component)(void)
: my_id(component_id++), my_Parent(nullptr), my_AbsX(0.0f), my_AbsY(0.0f), my_Visible(true)
{
	my_Rectangle[0] = 0.0f;
	my_Rectangle[1] = 0.0f;
	my_Rectangle[2] = 0.0f;
	my_Rectangle[3] = 0.0f;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(Component, Component)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, i_Component *, parent)(
	/* [x] */ void)const
{
	return my_Parent;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, i_Graphics *, graphics)(
	/* [x] */ void)const
{
	return (i_Graphics *)&m_D3D11Graphics;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_Mouseable)(
	/* [r] */ i_Mouseable * _mouseable)
{
	m_Mouseable = _mouseable;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, i_Mouseable *, get_Mouseable)(
	/* [x] */ void)const
{
	return m_Mouseable;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Component, add_Component)(
	/* [set] */ i_Component * _component)
{
	return 0x8000000;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Component, remove_Component)(
	/* [r] */ i_Component * _component)
{
	return 0x8000000;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, bool, contain_Component)(
	/* [r] */ i_Component * _component)const
{
	return false;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Component, get_Component)(
	/* [w] */ i_Component * (&_component),
	/* [r] */ unsigned int _id)const
{
	return 0x8000000;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, unsigned int, get_id)(
	/* [x] */ void)const
{
	return my_id;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_X)(
	/* [r] */ float _value)
{
	my_Rectangle[0] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_Y)(
	/* [r] */ float _value)
{
	my_Rectangle[1] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_W)(
	/* [r] */ float _value)
{
	my_Rectangle[2] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_H)(
	/* [r] */ float _value)
{
	my_Rectangle[3] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_X)(
	/* [x] */ void)const
{
	return my_Rectangle[0];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_Y)(
	/* [x] */ void)const
{
	return my_Rectangle[1];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_W)(
	/* [x] */ void)const
{
	return my_Rectangle[2];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_H)(
	/* [x] */ void)const
{
	return my_Rectangle[3];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Component, set_Visible)(
	/* [r] */ bool _visible)
{
	bool b = my_Visible;
	my_Visible = _visible;

	return b;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, bool, is_Visible)(
	/* [x] */ void)const
{
	return my_Visible;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, bool, event_chain)(
	/* [r] */ i_inputEventHelper * _eventhelper)
{
	return _eventhelper->chain(this);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, update)(
	/* [x] */ void)
{
	if (my_Parent)
	{
		my_AbsX = my_Parent->get_AbsX() + get_X();
		my_AbsY = my_Parent->get_AbsY() + get_Y();
	}

	m_D3D11Graphics.update_Panel({
		my_AbsX,
		my_AbsY,
		my_Rectangle[2],
		my_Rectangle[3] });
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, render)(
	/* [x] */ void)
{
	if (is_Visible())
	{
		m_D3D11Graphics.render_Panel();
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_AbsX)(
	/* [x] */ void)const
{
	return my_AbsX;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_AbsY)(
	/* [x] */ void)const
{
	return my_AbsY;
}