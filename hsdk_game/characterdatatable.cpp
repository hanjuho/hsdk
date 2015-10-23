#include <game/datatable/characterdatatable.h>



using namespace hsdk;
using namespace game;
using namespace datatable;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(CharacterDataTable, CharacterDataTable)()
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(CharacterDataTable, CharacterDataTable)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(CharacterDataTable, read_Table)(
	_In_ unsigned int _state,
	_Out_ void * (&_data),
	_In_ unsigned int _size)const
{
	return HSDK_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(CharacterDataTable, write_Table)(
	_In_ unsigned int _state,
	_In_ ...)
{
	return HSDK_FAIL;
}