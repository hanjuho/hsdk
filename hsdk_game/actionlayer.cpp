#include <game/actionlayer.h>
#include <game/interface/datatable.h>



using namespace hsdk;
using namespace game;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(ActionLayer, ActionLayer)(void)
: m_DataTable(nullptr)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(ActionLayer, ActionLayer)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ActionLayer, void, initialize)(
	_X_ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ActionLayer, bool, act)(
	_In_ float _dt)const
{
	return false;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ActionLayer, unsigned int, priority)(
	_X_ void)const
{
	return 0xffffffff;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ActionLayer, void, link_DataTable)(
	/* [ref] */ i::i_DataTable * _datatable)
{
	m_DataTable = _datatable;
}