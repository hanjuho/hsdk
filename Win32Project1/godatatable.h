#pragma once



#include "gocommon.h"



namespace go
{

	// ���� : 
	DECL_CLASS(GameDataTable)
		: public hsdk::game::DataTable
	{
	public:
		
		// ���� :
		INTERFACE_DECL_FUNC(read_Table)(
			_Out_ void * _data,
			_In_ unsigned int _offset,
			_In_ unsigned int _size)const;

		// ���� :               
		INTERFACE_DECL_FUNC(write_Table)(
			_In_ unsigned int _datavalue,
			_In_ ...);

		// ���� : 
		INTERFACE_DECL_FUNC_T(const wchar_t *, name)(
			_X_ void)const;

		// ���� : 
		INTERFACE_DECL_FUNC_T(unsigned int, type)(
			_X_ void)const;

		// ���� : 
		INTERFACE_DECL_FUNC_T(unsigned int, state)(
			_X_ void)const;

		// ���� : 


	private:

		// ���� : 
		btRigidBody * my_Body;

		// ���� :
		unsigned int my_Type;

		// ���� :
		unsigned int my_State;
		
		// ���� : 
		float my_Attack;

		// ���� :
		float my_Defense;

		// ���� :
		float my_Life;

		// ���� :
		float my_Speed;

	};

}