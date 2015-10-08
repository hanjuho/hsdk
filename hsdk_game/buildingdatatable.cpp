#include <game/datatable/buildingdatatable.h>



using namespace hsdk;
using namespace game;
using namespace datatable;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(BuildingDataTable, BuildingDataTable)(
	const wchar_t * name,
	unsigned int type,
	unsigned int state,
	unsigned int maxLife,
	int defense)
	: m_Name(name), m_Type(type), m_State(state),
	m_MaxLife(maxLife), m_Life(maxLife), m_Defense(defense)
{
	if (maxLife < 1)
	{
		throw HSDK_FAIL;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(BuildingDataTable, BuildingDataTable)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(BuildingDataTable, read_Table)(
	/* [r] */ unsigned int _offset,
	/* [r] */ unsigned int _size,
	/* [w] */ void * (&_data))const
{
	return HSDK_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(BuildingDataTable, write_Table)(
	/* [r] */ unsigned int _state,
	/* [r] */ ...)
{
	return HSDK_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BuildingDataTable, const wchar_t *, name)(
	/* [x] */ void)const
{
	return m_Name.c_str();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BuildingDataTable, unsigned int, type)(
	/* [x] */ void)const
{
	return m_Type;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BuildingDataTable, unsigned int, state)(
	/* [x] */ void)const
{
	return m_State;
}