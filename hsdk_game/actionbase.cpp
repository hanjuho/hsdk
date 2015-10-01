#include <game/actionbase.h>



using namespace hsdk;
using namespace game;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(ActionBase, ActionBase)(void)
: m_Datatable(nullptr)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(ActionBase, ActionBase)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, i::i_ActionLayer *, attack)(
	/* [r] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, i::i_ActionLayer *, suffer)(
	/* [r] */ unsigned int _frequency,
	/* [r] */ float _amount,
	/* [r] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, i::i_ActionLayer *, move)(
	/* [r] */ float _x,
	/* [r] */ float _y,
	/* [r] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, i::i_ActionLayer *, wait)(
	/* [r] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, i::i_ActionLayer *, effect)(
	/* [r] */ unsigned int _effect,
	/* [r] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, i::i_ActionLayer *, recovery)(
	/* [r] */ unsigned int _frequency,
	/* [r] */ float _amount,
	/* [r] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, i::i_ActionLayer *, resurrect)(
	/* [r] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, i::i_ActionLayer *, disappear)(
	/* [r] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, void, link_DataTable)(
	/* [ref] */ i::i_DataTable * _datatable)
{
	m_Datatable = _datatable;
}