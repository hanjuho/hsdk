#include <game/controller.h>



using namespace hsdk;
using namespace game;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Controller, Controller)(void)
: m_ActionBase(nullptr), m_ActionListener(nullptr)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(Controller, Controller)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Controller, void, attack)(
	/* [r] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->attack(_flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Controller, void, suffer)(
	/* [r] */ unsigned int _frequency,
	/* [r] */ float _amount,
	/* [r] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->suffer(_frequency, _amount, _flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Controller, void, move)(
	/* [r] */ float _x,
	/* [r] */ float _y,
	/* [r] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->move(_x, _y, _flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Controller, void, wait)(
	/* [r] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->wait(_flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Controller, void, effect)(
	/* [r] */ unsigned int _effect,
	/* [r] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->effect(_effect, _flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Controller, void, recovery)(
	/* [r] */ unsigned int _frequency,
	/* [r] */ float _amount,
	/* [r] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->recovery(_frequency, _amount, _flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Controller, void, resurrect)(
	/* [r] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->resurrect(_flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Controller, void, disappear)(
	/* [r] */ long _flag)
{
	i::i_ActionLayer * layer;
	if (layer = m_ActionBase->disappear(_flag))
	{
		m_ActionListener->listen_Action(layer);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Controller, void, link_ActionBase)(
	/* [ref] */ i::i_ActionBase * _actionbase)
{
	m_ActionBase = _actionbase;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Controller, void, link_ActionListener)(
	/* [ref] */ i::i_ActionListener * _actionlistener)
{
	m_ActionListener = _actionlistener;
}