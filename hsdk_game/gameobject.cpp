#include <game/gameobject.h>



using namespace hsdk;
using namespace game;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(GameObject, GameObject)(
	/* [set] */ i::i_DataTable * _datatable,
	/* [set] */ Controller * _controller,
	/* [set] */ ActionBase *_actionbase,
	/* [set] */ ModelRenderer * _renderer)
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
	m_Renderer->link_DataTable(_datatable);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(GameObject, GameObject)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameObject, i::i_DataTable *, datatalbe)(
	/* [void] */ void)const
{
	return m_DataTable;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameObject, i::i_Controller *, controller)(
	/* [void] */ void)const
{
	return m_Controller;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameObject, i::i_ModelRenderer *, renderer)(
	/* [void] */ void)const
{
	return m_Renderer;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameObject, void, update)(
	_X_ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameObject, void, render)(
	_X_ void)
{
	m_Renderer->render();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameObject, void, listen_Action)(
	/* [ref] */ i::i_ActionLayer * _actionlayer)
{
	unsigned int p = _actionlayer->priority();
	if (p)
	{		
		if (p < my_Action->priority())
		{
			//
			_actionlayer->initialize();
			my_Action = _actionlayer;

			//
			m_Renderer->initialize();
		}
	}
	else
	{
		//
		_actionlayer->initialize();
		my_Effects.push_back(_actionlayer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameObject, void, act_ActionLayers)(
	_X_ void)
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

	if (nullptr == my_Action || my_Action->act())
	{
		m_Controller->wait(0);
	}
}