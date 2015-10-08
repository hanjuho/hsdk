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
	/* [r] */ unsigned int _state,
	/* [w] */ void * (&_data),
	/* [r] */ unsigned int _size)const
{
	return HSDK_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(CharacterDataTable, write_Table)(
	/* [r] */ unsigned int _state,
	/* [r] */ ...)
{
	return HSDK_FAIL;
}