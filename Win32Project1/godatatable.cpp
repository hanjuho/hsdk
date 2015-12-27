#include "godatatable.h"



using namespace go;


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameDataTable, read_Table)(
	_Out_ void * _data,
	_In_ unsigned int _offset,
	_In_ unsigned int _size)const
{
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameDataTable, write_Table)(
	_In_ unsigned int _datavalue,
	_In_ ...)
{
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameDataTable, const wchar_t *, name)(
	_X_ void)const
{
	return L"";
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameDataTable, unsigned int, type)(
	_X_ void)const
{
	return my_Type;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameDataTable, unsigned int, state)(
	_X_ void)const
{
	return my_State;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameDataTable, const float *, matrix)(
	_X_ void)const
{
	return nullptr;
}