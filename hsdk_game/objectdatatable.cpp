#include <game/datatable/objectdatatable.h>



using namespace hsdk;
using namespace game;
using namespace datatable;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(ObjectDataTable, ObjectDataTable)()
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(ObjectDataTable, ObjectDataTable)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(ObjectDataTable, read_Table)(
	/* [r] */ unsigned int _state,
	/* [w] */ void * (&_data),
	/* [r] */ unsigned int _size)const
{
	return HSDK_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(ObjectDataTable, write_Table)(
	/* [r] */ unsigned int _state,
	/* [r] */ ...)
{
	return HSDK_FAIL;
}