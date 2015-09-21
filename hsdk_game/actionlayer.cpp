#include <game/actionlayer.h>
#include <game/interface/datatable.h>



using namespace hsdk;
using namespace game;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(ActionLayer, ActionLayer)(void)
: m_DataTable(nullptr), m_Value(0.0f)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(ActionLayer, ActionLayer)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ActionLayer, void, link_DataTable)(
	/* [ref] */ i::i_DataTable * _datatable,
	/* [in] */ float _value)
{
	m_DataTable = _datatable;
	m_Value = _value;
}