#include <game/gameobject.h>



using namespace hsdk;
using namespace game;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(GameObject, GameObject)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(GameObject, GameObject)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, set_Controller)(
	/* [ref] */ i::i_Controller * _controller)
{
	m_Controller = _controller;

	if (_controller)
	{
		_controller->link_ActionTable(m_ActionBase);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, set_DataModel)(
	/* [include] */ i::i_DataTable * _datatable)
{
	m_DataTable = _datatable;

	if (m_Renderer)
	{
		m_Renderer->link_DataTable(_datatable);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, set_ActionBase)(
	/* [include] */ i::i_ActionBase * _actionbase)
{
	m_ActionBase = _actionbase;

	if (_actionbase)
	{
		_actionbase->link_DataTable(m_DataTable);
	}

	if (m_Controller)
	{
		m_Controller->link_ActionTable(_actionbase);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, set_Renderer)(
	/* [include] */ i::i_ModelRenderer * _renderer)
{
	m_Renderer = _renderer;

	if (_renderer)
	{
		_renderer->link_DataTable(m_DataTable);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, attack)(
	/* [none] */ void)
{
	if (m_Controller)
	{
		m_Controller->attack();
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, suffer)(
	/* [in] */ unsigned int _frequency,
	/* [in] */ float _amount,
	/* [in] */ long _flag)
{
	if (m_Controller)
	{
		m_Controller->suffer(_frequency, _amount, _flag);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, move)(
	/* [in] */ float _x,
	/* [in] */ float _y,
	/* [in] */ long _flag)
{
	if (m_Controller)
	{
		m_Controller->move(_x, _y, _flag);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, wait)(
	/* [none] */ void)
{
	if (m_Controller)
	{
		m_Controller->wait();
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, effect)(
	/* [in] */ unsigned int _effect)
{
	if (m_Controller)
	{
		m_Controller->effect(_effect);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, recovery)(
	/* [in] */ unsigned int _frequency,
	/* [in] */ float _amount,
	/* [in] */ long _flag)
{
	if (m_Controller)
	{
		m_Controller->recovery(_frequency, _amount, _flag);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, resurrect)(
	/* [none] */ void)
{
	if (m_Controller)
	{
		m_Controller->resurrect();
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, disappear)(
	/* [none] */ void)
{
	if (m_Controller)
	{
		m_Controller->disappear();
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, notify)(
	/* [none] */ void)
{
	if (m_ActionBase)
	{
		m_ActionBase->update();
	}

	if (m_Controller)
	{
		m_Controller->update();
	}

	if (m_Renderer)
	{
		m_Renderer->update();
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, update)(
	/* [none] */ void)
{
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, render)(
	/* [none] */ void)
{
	if (m_Renderer)
	{
		m_Renderer->render();
	}
}