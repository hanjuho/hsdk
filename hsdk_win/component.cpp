#include <hsdk/win/frame/component.h>
#include <hsdk/interface/frame/inputeventhelper.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;


// grobal
unsigned int component_id = 0;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Component, Component)(void)
: my_id(component_id++), my_Parent(nullptr), my_AbsX(0.0f), my_AbsY(0.0f)
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
CLASS_REALIZE_FUNC_T(Component, i_Graphics *, graphics)(
	/* [none] */ void)
{
	return &m_D3D11Graphics;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_Mouseable)(
	/* [in] */ i_Mouseable * _mouseable)
{
	m_Mouseable = _mouseable;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, i_Mouseable *, get_Mouseable)(
	/* [none] */ void)
{
	return m_Mouseable;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, i_Component *, parent)(
	/* [none] */ void)
{
	return my_Parent;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Component, add_Component)(
	/* [include] */ i_Component * _component)
{
	return 0x8000000;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Component, remove_Component)(
	/* [in] */ i_Component * _component)
{
	return 0x8000000;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, bool, contain_Component)(
	/* [in] */ i_Component * _component)
{
	return false;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Component, get_Component)(
	/* [out] */ i_Component * (&_component),
	/* [in] */ unsigned int _id)
{
	return 0x8000000;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, unsigned int, get_id)(
	/* [none] */ void)
{
	return my_id;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_X)(
	/* [in] */ float _value)
{
	my_Rectangle[0] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_Y)(
	/* [in] */ float _value)
{
	my_Rectangle[1] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_W)(
	/* [in] */ float _value)
{
	my_Rectangle[2] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_H)(
	/* [in] */ float _value)
{
	my_Rectangle[3] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_X)(
	/* [none] */ void)
{
	return my_Rectangle[0];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_Y)(
	/* [none] */ void)
{
	return my_Rectangle[1];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_W)(
	/* [none] */ void)
{
	return my_Rectangle[2];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_H)(
	/* [none] */ void)
{
	return my_Rectangle[3];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Component, set_Visible)(
	/* [in] */ bool _visible)
{
	return m_D3D11Graphics.visible = _visible;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, bool, is_Visible)(
	/* [none] */ void)
{
	return m_D3D11Graphics.visible;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, bool, event_chain)(
	/* [in] */ i_inputEventHelper * _eventhelper)
{
	return _eventhelper->chain(this);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, update)(
	/* [none] */ void)
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
	/* [none] */ void)
{
	m_D3D11Graphics.render_Panel();
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_AbsX)(
	/* [none] */ void)
{
	return my_AbsX;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_AbsY)(
	/* [none] */ void)
{
	return my_AbsY;
}