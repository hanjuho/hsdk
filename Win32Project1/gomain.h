#pragma once



#include "godatatable.h"
#include "gomodelrenderer.h"



namespace go
{

	// 설명 : 
	DECL_CLASS(GameEngine)
		: public GOE, public GOE::i_Factory, public GOE::i_Container, public GOE::i_Finder
	{
	public:

		// 설명 : 
		CLASS_DECL_CONSTRUCTOR(GameEngine)(void);

		// 설명 : 
		INTERFACE_DECL_FUNC(create_ID)(
			_Out_ unsigned int * _id,
			_In_ const REF_KEY _key,
			_In_ unsigned int _option)const;

		// 설명 : 
		INTERFACE_DECL_FUNC(create_Data)(
			_Out_ PTR_DATA * _data,
			_In_ const REF_KEY _key,
			_In_ unsigned int _option,
			_Out_opt_ unsigned int * _id = nullptr)const;

		// 설명 : 
		INTERFACE_DECL_FUNC(create_Software)(
			_Out_ PTR_SOFTWARE * _software,
			_In_ const PTR_DATA _data,
			_In_ unsigned int _id)const;
		
		// 설명 : 
		INTERFACE_DECL_FUNC_T(PTR_SOFTWARE, push)(
			_In_ const REF_KEY _key,
			_In_ unsigned int _option);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, pop)(
			_In_ const REF_KEY _key,
			_In_ unsigned int _id);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, clear)(
			_In_ const REF_KEY _key);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, clear_All)(
			_X_ void);

		// 설명 : 
		INTERFACE_DECL_FUNC(move)(
			_In_ const REF_KEY _from,
			_In_ unsigned int _id,
			_In_ const REF_KEY _to,
			_In_ PTR_SOFTWARE _software);

		// 설명 : 
		INTERFACE_DECL_FUNC(move_all)(
			_In_ const REF_KEY _from,
			_In_ const REF_KEY _to,
			_In_ unsigned int _size,
			_In_ PTR_SOFTWARE * _softwarebuffer);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, sort)(
			_X_ void);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(unsigned int, size)(
			_In_ const REF_KEY _key);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(unsigned int, size_All)(
			_X_ void);
		
		// 설명 : 명시적인 _key 와 _id 만으로 탐색
		INTERFACE_DECL_FUNC(find_Explicit)(
			_Out_ PTR_SOFTWARE * _software,
			_In_ const REF_KEY _key,
			_In_ unsigned int _id)const;

		// 설명 : 명시적인 _key 만으로 모두 탐색
		INTERFACE_DECL_FUNC(find_Explicit_All)(
			_Out_ PTR_SOFTWARE * _softwarebuffer,
			_In_ const REF_KEY _key,
			_In_ unsigned int _size)const;

		// 설명 : 명시적인 _key와 암묵적인 _data로 탐색
		INTERFACE_DECL_FUNC(find_implicit)(
			_Out_ PTR_SOFTWARE * _software,
			_In_ const REF_KEY _key,
			_In_ const LPVOID _data,
			_In_ unsigned int _format,
			_In_ unsigned int _size)const;

		// 설명 : 명시적인 _key와 암묵적인 _data로 모두 탐색
		INTERFACE_DECL_FUNC(find_implicit_All)(
			_Out_ PTR_SOFTWARE * _softwarebuffer,
			_In_ const LPVOID _data,
			_In_ unsigned int _format,
			_In_ unsigned int _size)const;

		// 설명 : 
		INTERFACE_DECL_FUNC_T(PTR_FACTORY, get_Factory)(
			_X_ void);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(PTR_CONTAINER, get_Container)(
			_X_ void);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(PTR_FINDER, get_Finder)(
			_X_ void);

	private:

		// 설명 : 
		bullet::Bullet_Engine my_PhysicsEngine;

		// 설명 : 


	};

}