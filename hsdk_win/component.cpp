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
	my_rectangle[0] = 0.0f;
	my_rectangle[1] = 0.0f;
	my_rectangle[2] = 0.0f;
	my_rectangle[3] = 0.0f;
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
	my_rectangle[0] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_Y)(
	/* [in] */ float _value)
{
	my_rectangle[1] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_W)(
	/* [in] */ float _value)
{
	my_rectangle[2] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, set_H)(
	/* [in] */ float _value)
{
	my_rectangle[3] = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_X)(
	/* [none] */ void)
{
	return my_rectangle[0];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_Y)(
	/* [none] */ void)
{
	return my_rectangle[1];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_W)(
	/* [none] */ void)
{
	return my_rectangle[2];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, float, get_H)(
	/* [none] */ void)
{
	return my_rectangle[3];
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
	if (my_parent)
	{
		D3D11Graphics * pgraphics;
		pgraphics = (D3D11Graphics *)my_parent->graphics();

		// 부모의 x, y를 사용하지 않는 이유는 부모의 x, y가 그 부모로부터의 상대적 좌표이기
		// 때문에 절대 좌표계를 사용하는 D3D11Graphics::form을 호출하는 것
		if (pgraphics)
		{
			// 부모의 x, y + 상대적 좌표 x, y = 자신의 x, y
			m_d3d11Graphics.form[0] = pgraphics->form[0] + my_rectangle[0];
			m_d3d11Graphics.form[1] = pgraphics->form[1] + my_rectangle[1];

			// 자신의 x, y + 상대적 좌표 w, h = 자신의 x2, y2
			m_d3d11Graphics.form[2] = m_d3d11Graphics.form[0] + my_rectangle[2];
			m_d3d11Graphics.form[3] = m_d3d11Graphics.form[1] + my_rectangle[3];
		}
	}
	else
	{
		m_d3d11Graphics.form[0] = my_rectangle[0];
		m_d3d11Graphics.form[1] = my_rectangle[1];
		m_d3d11Graphics.form[2] = my_rectangle[0] + my_rectangle[3];
		m_d3d11Graphics.form[3] = my_rectangle[1] + my_rectangle[4];
	}

	m_d3d11Graphics.update();
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Component, void, render)(
	/* [none] */ void)
{
	m_d3d11Graphics.render();
}