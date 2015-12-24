#pragma once



#include "gocommon.h"



namespace go
{

	// 설명 : 
	DECL_CLASS(GameDataTable)
		: public hsdk::game::DataTable
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

		// 설명 : 


	private:

		// 설명 : 
		btRigidBody * my_Body;

		// 설명 :
		unsigned int my_Type;

		// 설명 :
		unsigned int my_State;
		
		// 설명 : 
		float my_Attack;

		// 설명 :
		float my_Defense;

		// 설명 :
		float my_Life;

		// 설명 :
		float my_Speed;

	};

}