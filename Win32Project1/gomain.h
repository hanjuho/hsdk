#pragma once



#include "godatatable.h"
#include "gomodelrenderer.h"



namespace go
{

	// ���� : 
	DECL_CLASS(GameEngine)
		: public GOE, public GOE::i_Factory, public GOE::i_Container, public GOE::i_Finder
	{
	public:

		// ���� : 
		CLASS_DECL_CONSTRUCTOR(GameEngine)(void);

		// ���� : 
		INTERFACE_DECL_FUNC(create_ID)(
			_Out_ unsigned int * _id,
			_In_ const REF_KEY _key,
			_In_ unsigned int _option)const;

		// ���� : 
		INTERFACE_DECL_FUNC(create_Data)(
			_Out_ PTR_DATA * _data,
			_In_ const REF_KEY _key,
			_In_ unsigned int _option,
			_Out_opt_ unsigned int * _id = nullptr)const;

		// ���� : 
		INTERFACE_DECL_FUNC(create_Software)(
			_Out_ PTR_SOFTWARE * _software,
			_In_ const PTR_DATA _data,
			_In_ unsigned int _id)const;
		
		// ���� : 
		INTERFACE_DECL_FUNC_T(PTR_SOFTWARE, push)(
			_In_ const REF_KEY _key,
			_In_ unsigned int _option);

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, pop)(
			_In_ const REF_KEY _key,
			_In_ unsigned int _id);

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, clear)(
			_In_ const REF_KEY _key);

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, clear_All)(
			_X_ void);

		// ���� : 
		INTERFACE_DECL_FUNC(move)(
			_In_ const REF_KEY _from,
			_In_ unsigned int _id,
			_In_ const REF_KEY _to,
			_In_ PTR_SOFTWARE _software);

		// ���� : 
		INTERFACE_DECL_FUNC(move_all)(
			_In_ const REF_KEY _from,
			_In_ const REF_KEY _to,
			_In_ unsigned int _size,
			_In_ PTR_SOFTWARE * _softwarebuffer);

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, sort)(
			_X_ void);

		// ���� : 
		INTERFACE_DECL_FUNC_T(unsigned int, size)(
			_In_ const REF_KEY _key);

		// ���� : 
		INTERFACE_DECL_FUNC_T(unsigned int, size_All)(
			_X_ void);
		
		// ���� : ������� _key �� _id ������ Ž��
		INTERFACE_DECL_FUNC(find_Explicit)(
			_Out_ PTR_SOFTWARE * _software,
			_In_ const REF_KEY _key,
			_In_ unsigned int _id)const;

		// ���� : ������� _key ������ ��� Ž��
		INTERFACE_DECL_FUNC(find_Explicit_All)(
			_Out_ PTR_SOFTWARE * _softwarebuffer,
			_In_ const REF_KEY _key,
			_In_ unsigned int _size)const;

		// ���� : ������� _key�� �Ϲ����� _data�� Ž��
		INTERFACE_DECL_FUNC(find_implicit)(
			_Out_ PTR_SOFTWARE * _software,
			_In_ const REF_KEY _key,
			_In_ const LPVOID _data,
			_In_ unsigned int _format,
			_In_ unsigned int _size)const;

		// ���� : ������� _key�� �Ϲ����� _data�� ��� Ž��
		INTERFACE_DECL_FUNC(find_implicit_All)(
			_Out_ PTR_SOFTWARE * _softwarebuffer,
			_In_ const LPVOID _data,
			_In_ unsigned int _format,
			_In_ unsigned int _size)const;

		// ���� : 
		INTERFACE_DECL_FUNC_T(PTR_FACTORY, get_Factory)(
			_X_ void);

		// ���� : 
		INTERFACE_DECL_FUNC_T(PTR_CONTAINER, get_Container)(
			_X_ void);

		// ���� : 
		INTERFACE_DECL_FUNC_T(PTR_FINDER, get_Finder)(
			_X_ void);

	private:

		// ���� : 
		bullet::Bullet_Engine my_PhysicsEngine;

		// ���� : 


	};

}