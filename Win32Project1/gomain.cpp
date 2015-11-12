#include "gomain.h"



using namespace go;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(GameEngine, GameEngine)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, create_ID)(
	_Out_ unsigned int * _id,
	_In_ const REF_KEY _key,
	_In_ unsigned int _option)const
{
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, create_Data)(
	_Out_ PTR_DATA * _data,
	_In_ const REF_KEY _key,
	_In_ unsigned int _option,
	_Out_opt_ unsigned int * _id)const
{
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, create_Software)(
	_Out_ PTR_SOFTWARE * _software,
	_In_ const PTR_DATA _data,
	_In_ unsigned int _id)const
{
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, GOE::PTR_SOFTWARE, push)(
	_In_ const REF_KEY _key,
	_In_ unsigned int _option)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, void, pop)(
	_In_ const REF_KEY _key,
	_In_ unsigned int _id)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, void, clear)(
	_In_ const REF_KEY _key)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, void, clear_All)(
	_X_ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, move)(
	_In_ const REF_KEY _from,
	_In_ unsigned int _id,
	_In_ const REF_KEY _to,
	_In_ PTR_SOFTWARE _software)
{
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, move_all)(
	_In_ const REF_KEY _from,
	_In_ const REF_KEY _to,
	_In_ unsigned int _size,
	_In_ PTR_SOFTWARE * _softwarebuffer)
{
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, void, sort)(
	_X_ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, unsigned int, size)(
	_In_ const REF_KEY _key)
{
	return 0;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, unsigned int, size_All)(
	_X_ void)
{
	return 0;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, find_Explicit)(
	_Out_ PTR_SOFTWARE * _software,
	_In_ const REF_KEY _key,
	_In_ unsigned int _id)const
{
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, find_Explicit_All)(
	_Out_ PTR_SOFTWARE * _softwarebuffer,
	_In_ const REF_KEY _key,
	_In_ unsigned int _size)const
{
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, find_implicit)(
	_Out_ PTR_SOFTWARE * _software,
	_In_ const REF_KEY _key,
	_In_ const LPVOID _data,
	_In_ unsigned int _format,
	_In_ unsigned int _size)const
{
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, find_implicit_All)(
	_Out_ PTR_SOFTWARE * _softwarebuffer,
	_In_ const LPVOID _data,
	_In_ unsigned int _format,
	_In_ unsigned int _size)const
{
	return E_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, GOE::PTR_FACTORY, get_Factory)(
	_X_ void)
{
	return this;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, GOE::PTR_CONTAINER, get_Container)(
	_X_ void)
{
	return this;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, GOE::PTR_FINDER, get_Finder)(
	_X_ void)
{
	return this;
}