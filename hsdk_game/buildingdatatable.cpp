#include <game/datatable/buildingdatatable.h>



using namespace hsdk;
using namespace game;
using namespace datatable;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(BuildingDataTable, BuildingDataTable)(
	unsigned int maxLife,
	unsigned int maxMana,
	unsigned int defense)
	: m_MaxLife(maxLife),
	m_Life(maxLife),
	m_MaxMana(maxMana),
	m_Mana(maxMana),
	m_Defense(defense)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(BuildingDataTable, BuildingDataTable)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(BuildingDataTable, read_Table)(
	/* [in] */ unsigned int _state,
	/* [out] */ void * (&_data),
	/* [in] */ unsigned int _size)const
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(BuildingDataTable, write_Table)(
	/* [in] */ unsigned int _state,
	/* [in] */ ...)
{

}