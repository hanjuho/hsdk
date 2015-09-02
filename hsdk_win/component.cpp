#include <hsdk/win/frame/component.h>
#include <hsdk/interface/frame/inputeventhelper.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;


// grobal
unsigned int component_id = 0;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Component, Component)(void)
: my_id(component_id++), my_parent(nullptr)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(Component, Component)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, i_Graphics *, graphics)(
	/* [none] */ void)
{
	return &m_d3d11Graphics;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_Mouseable)(
	/* [in] */ i_Mouseable * _mouseable)
{
	//m_mouseable = _mouseable;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, i_Mouseable *, get_Mouseable)(
	/* [none] */ void)
{
	return m_mouseable;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, i_Component *, parent)(
	/* [none] */ void)
{
	return my_parent;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Component, add_Component)(
	/* [in] */ i_Component * _component)
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
	m_d3d11Graphics.form[0] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_Y)(
	/* [in] */ float _value)
{
	m_d3d11Graphics.form[1] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_W)(
	/* [in] */ float _value)
{
	m_d3d11Graphics.form[2] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_H)(
	/* [in] */ float _value)
{
	m_d3d11Graphics.form[3] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_X)(
	/* [none] */ void)
{
	return m_d3d11Graphics.form[0];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_Y)(
	/* [none] */ void)
{
	return m_d3d11Graphics.form[1];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_W)(
	/* [none] */ void)
{
	return m_d3d11Graphics.form[2];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_H)(
	/* [none] */ void)
{
	return m_d3d11Graphics.form[3];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Component, set_Visible)(
	/* [in] */ bool _visible)
{
	return m_d3d11Graphics.visible = _visible;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, bool, is_Visible)(
	/* [none] */ void)
{
	return m_d3d11Graphics.visible;
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
	m_d3d11Graphics.update();
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, render)(
	/* [none] */ void)
{
	m_d3d11Graphics.render();
}