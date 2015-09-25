#include <game/gameobject.h>



using namespace hsdk;
using namespace game;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(GameObject, GameObject)(
	/* [include] */ i::i_DataTable * _datatable,
	/* [include] */ Controller * _controller,
	/* [include] */ ActionBase *_actionbase,
	/* [include] */ i::i_ModelRenderer * _renderer)
	: m_DataTable(_datatable), m_Controller(_controller),
	m_ActionBase(_actionbase), m_Renderer(_renderer)
{
	IF_FALSE(_datatable && _controller &&_actionbase && _renderer)
	{
		throw HSDK_FAIL;
	}

	m_Controller->link_ActionBase(_actionbase);
	m_Controller->link_ActionListener(this);
	m_ActionBase->link_DataTable(_datatable);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(GameObject, GameObject)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, i::i_DataTable *, datatalbe)(
	/* [void] */ void)const
{
	return m_DataTable;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, i::i_Controller *, controller)(
	/* [void] */ void)const
{
	return m_Controller;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, i::i_ModelRenderer *, renderer)(
	/* [void] */ void)const
{
	return m_Renderer;
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

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, listen_Action)(
	/* [ref] */ i::i_ActionLayer * _actionlayer)
{
	unsigned int p = _actionlayer->priority();
	if (p)
	{
		if (p < my_Action->priority())
		{
			my_Action = _actionlayer;
		}
	}
	else
	{
		my_Effects.push_back(_actionlayer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(GameObject, void, act)(
	/* [none] */ void)
{
	std::list<i::i_ActionLayer *>::iterator begin = my_Effects.begin();
	std::list<i::i_ActionLayer *>::iterator end = my_Effects.begin();
	while (begin != end)
	{
		IF_FALSE((*begin)->act())
		{
			begin = my_Effects.erase(begin);
			continue;
		}

		++begin;
	}

	if (my_Action || my_Action->act())
	{
		m_Controller->wait(0);
	}
}