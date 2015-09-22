#include <game/datatable/characterdatatable.h>



using namespace hsdk;
using namespace game;
using namespace datatable;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(CharacterDataTable, CharacterDataTable)()
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(CharacterDataTable, CharacterDataTable)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(CharacterDataTable, read_Table)(
	/* [in] */ unsigned int _state,
	/* [out] */ void * (&_data),
	/* [in] */ unsigned int _size)const
{
	return HSDK_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(CharacterDataTable, write_Table)(
	/* [in] */ unsigned int _state,
	/* [in] */ ...)
{
	return HSDK_FAIL;
}