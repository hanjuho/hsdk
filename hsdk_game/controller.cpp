#include <game/controller.h>



using namespace hsdk;
using namespace game;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Controller, Controller)(void)
: m_ActionBase(nullptr), m_ActionListener(nullptr)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(Controller, Controller)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, attack)(
	/* [in] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->attack(_flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, suffer)(
	/* [in] */ unsigned int _frequency,
	/* [in] */ float _amount,
	/* [in] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->suffer(_frequency, _amount, _flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, move)(
	/* [in] */ float _x,
	/* [in] */ float _y,
	/* [in] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->move(_x, _y, _flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, wait)(
	/* [in] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->wait(_flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, effect)(
	/* [in] */ unsigned int _effect,
	/* [in] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->effect(_effect, _flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, recovery)(
	/* [in] */ unsigned int _frequency,
	/* [in] */ float _amount,
	/* [in] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->recovery(_frequency, _amount, _flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, resurrect)(
	/* [in] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->resurrect(_flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, disappear)(
	/* [in] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->disappear(_flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, link_ActionBase)(
	/* [ref] */ i::i_ActionBase * _actionbase)
{
	m_ActionBase = _actionbase;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, link_ActionListener)(
	/* [ref] */ i::i_ActionListener * _actionlistener)
{
	m_ActionListener = _actionlistener;
}