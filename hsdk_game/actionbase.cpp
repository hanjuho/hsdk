#include <game/actionbase.h>



using namespace hsdk;
using namespace game;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(ActionBase, ActionBase)(void)
: m_Datatable(nullptr)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(ActionBase, ActionBase)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ActionBase, i::i_ActionLayer *, attack)(
	_In_ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ActionBase, i::i_ActionLayer *, suffer)(
	_In_ unsigned int _frequency,
	_In_ float _amount,
	_In_ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ActionBase, i::i_ActionLayer *, move)(
	_In_ float _x,
	_In_ float _y,
	_In_ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ActionBase, i::i_ActionLayer *, wait)(
	_In_ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ActionBase, i::i_ActionLayer *, effect)(
	_In_ unsigned int _effect,
	_In_ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ActionBase, i::i_ActionLayer *, recovery)(
	_In_ unsigned int _frequency,
	_In_ float _amount,
	_In_ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ActionBase, i::i_ActionLayer *, resurrect)(
	_In_ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ActionBase, i::i_ActionLayer *, disappear)(
	_In_ long _flag)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ActionBase, void, link_DataTable)(
	/* [ref] */ i::i_DataTable * _datatable)
{
	m_Datatable = _datatable;
}