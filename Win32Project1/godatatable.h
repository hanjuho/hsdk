#pragma once



#include "gocommon.h"



namespace go
{

	// 설명 : 
	DECL_CLASS(PhysicsDataTable)
		: public game::i::i_DataTable
	{
	public:

		// 설명 :
		INTERFACE_DECL_FUNC(read_Table)(
			_Out_ void * _data,
			_In_ unsigned int _offset,
			_In_ unsigned int _size)const;

		// 설명 : 
		INTERFACE_DECL_FUNC(write_Table)(
			_In_ unsigned int _datavalue,
			_In_ ...);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(const wchar_t *, name)(
			_X_ void)const;

		// 설명 : 
		INTERFACE_DECL_FUNC_T(unsigned int, type)(
			_X_ void)const;

		// 설명 : 
		INTERFACE_DECL_FUNC_T(unsigned int, state)(
			_X_ void)const;

	private:

		// 설명 : 
		btRigidBody * my_Body;
		
	};

}