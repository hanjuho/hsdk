#include <game/datatable/objectdatatable.h>



using namespace hsdk;
using namespace game;
using namespace datatable;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(ObjectDataTable, ObjectDataTable)()
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(ObjectDataTable, ObjectDataTable)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(ObjectDataTable, read_Table)(
	_In_ unsigned int _state,
	_Out_ void * (&_data),
	_In_ unsigned int _size)const
{
	return HSDK_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(ObjectDataTable, write_Table)(
	_In_ unsigned int _state,
	_In_ ...)
{
	return HSDK_FAIL;
}