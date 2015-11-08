#include "godatatable.h"



using namespace go;


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(PhysicsDataTable, read_Table)(
	_Out_ void * _data,
	_In_ unsigned int _offset,
	_In_ unsigned int _size)const
{
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(PhysicsDataTable, write_Table)(
	_In_ unsigned int _datavalue,
	_In_ ...)
{
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(PhysicsDataTable, const wchar_t *, name)(
	_X_ void)const
{
	return L"";
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(PhysicsDataTable, unsigned int, type)(
	_X_ void)const
{
	return 0;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(PhysicsDataTable, unsigned int, state)(
	_X_ void)const
{
	return 0;
}