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
	/* [in] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, i::i_ActionLayer *, suffer)(
	/* [in] */ unsigned int _frequency,
	/* [in] */ float _amount,
	/* [in] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, i::i_ActionLayer *, move)(
	/* [in] */ float _x,
	/* [in] */ float _y,
	/* [in] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, i::i_ActionLayer *, wait)(
	/* [in] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, i::i_ActionLayer *, effect)(
	/* [in] */ unsigned int _effect,
	/* [in] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, i::i_ActionLayer *, recovery)(
	/* [in] */ unsigned int _frequency,
	/* [in] */ float _amount,
	/* [in] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, i::i_ActionLayer *, resurrect)(
	/* [in] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, i::i_ActionLayer *, disappear)(
	/* [in] */ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionBase, void, link_DataTable)(
	/* [ref] */ i::i_DataTable * _datatable)
{
	m_Datatable = _datatable;
}