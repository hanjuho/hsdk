#include <game/actionlayer.h>
#include <game/interface/datatable.h>



using namespace hsdk;
using namespace game;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(ActionLayer, ActionLayer)(void)
: m_DataTable(nullptr)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(ActionLayer, ActionLayer)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionLayer, void, initialize)(
	/* [x] */ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionLayer, bool, act)(
	/* [r] */ float _dt)const
{
	return false;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionLayer, unsigned int, priority)(
	/* [x] */ void)const
{
	return 0xffffffff;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionLayer, void, link_DataTable)(
	/* [ref] */ i::i_DataTable * _datatable)
{
	m_DataTable = _datatable;
}