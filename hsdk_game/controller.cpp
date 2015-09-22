#include <game/controller.h>



using namespace hsdk;
using namespace game;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Controller, Controller)(void)
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

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, suffer)(
	/* [in] */ unsigned int _frequency,
	/* [in] */ float _amount,
	/* [in] */ long _flag)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, move)(
	/* [in] */ float _x,
	/* [in] */ float _y,
	/* [in] */ long _flag)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, wait)(
	/* [in] */ long _flag)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, effect)(
	/* [in] */ unsigned int _effect,
	/* [in] */ long _flag)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, recovery)(
	/* [in] */ unsigned int _frequency,
	/* [in] */ float _amount,
	/* [in] */ long _flag)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, resurrect)(
	/* [in] */ long _flag)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, disappear)(
	/* [in] */ long _flag)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, link_ActionBase)(
	/* [ref] */ i::i_ActionBase * _actionbase)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Controller, void, link_ActionListener)(
	/* [ref] */ i::i_ActionListener * _actionlistener)
{

}