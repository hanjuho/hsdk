#pragma once



#include "godatatable.h"
#include "gorenderer.h"
#include <hash_map>
#include <list>



#define WORLD_SIZE 1024



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
		CLASS_DECL_DESTRUCTOR(GameEngine)(void);

		/*
		설명 : 지형을 설치.
		$ 참고 : _cellX, _cellY는 반드시 2의 배수이어야 함(그렇지 않은 경우 내부에서 수정함).
		*/
		CLASS_DECL_FUNC(setup0_Terrain)(
			_In_ int _cellX,
			_In_ int _cellY,
			_In_ const float * _heightBuffer,
			_In_ unsigned int _size);

		// 설명 :
		CLASS_DECL_FUNC(usetSet_Terrain)(
			_In_ const wchar_t * _texture);

		// 설명 :
		CLASS_DECL_FUNC(usetSet_Sky)(
			_In_ unsigned int _width,
			_In_ unsigned int _height,
			_In_ const wchar_t * _front,
			_In_ const wchar_t * _back,
			_In_ const wchar_t * _left,
			_In_ const wchar_t * _right,
			_In_ const wchar_t * _top,
			_In_ const wchar_t * _bottom);

		// 설명 : 
		INTERFACE_DECL_FUNC(create_User)(
			_Out_ PTR_USER * _user,
			_In_ const REF_KEY _key,
			_In_ void * _context = nullptr);

		// 설명 : 
		INTERFACE_DECL_FUNC(create_User)(
			_Out_ PTR_USER * _user,
			_In_ unsigned int _id,
			_In_ const PTR_DATA _data,
			_In_ void * _context = nullptr);

		// 설명 : 
		INTERFACE_DECL_FUNC(create_ID)(
			_Out_ unsigned int * _id,
			_In_ const REF_KEY _key);

		// 설명 : 
		INTERFACE_DECL_FUNC(get_Data)(
			_Out_ PTR_DATA * _data,
			_In_ const REF_KEY _key);

		// 설명 : 
		INTERFACE_DECL_FUNC(push)(
			/* [set] */ PTR_USER _user);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, pop)(
			_In_ PTR_USER _user);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, pop)(
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
			_Out_ PTR_USER * _user = nullptr);

		// 설명 : 
		INTERFACE_DECL_FUNC(move_all)(
			_In_ const REF_KEY _from,
			_In_ const REF_KEY _to,
			_In_ unsigned int _size,
			_Out_ PTR_USER * _uerbuffer = nullptr);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, sort)(
			_X_ void);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(unsigned int, size)(
			_In_ const REF_KEY _key);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(unsigned int, size_All)(
			_X_ void);

		// 설명 : 명시적인 _id 만으로 탐색
		INTERFACE_DECL_FUNC(find_Explicit)(
			_Out_ PTR_USER * _user,
			_In_ unsigned int _id)const;

		// 설명 : 명시적인 _key 만으로 모두 탐색
		INTERFACE_DECL_FUNC(find_Explicit_All)(
			_Out_ PTR_USER * _userbuffer,
			_In_ const REF_KEY _key,
			_In_ unsigned int _size)const;

		// 설명 : 명시적인 _key와 암묵적인 _data로 탐색
		INTERFACE_DECL_FUNC(find_implicit)(
			_Out_ PTR_USER * _user,
			_In_ const REF_KEY _key,
			_In_ const LPVOID _data,
			_In_ unsigned int _format,
			_In_ unsigned int _size)const;

		// 설명 : 명시적인 _key와 암묵적인 _data로 모두 탐색
		INTERFACE_DECL_FUNC(find_implicit_All)(
			_Out_ PTR_USER * _userbuffer,
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

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, update)(
			_In_ float _time);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, render)(
			_X_ void);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, reset)(
			_X_ void);

	protected:

		// 설명 : 
		INTERFACE_DECL_FUNC_T(game::DataTable *, create_DataTable)(
			_In_ const PTR_DATA _data,
			_In_ const D3DXMATRIX & _matrix);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(game::Controller *, create_Controller)(
			_In_ const PTR_DATA _data);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(game::ActionBase *, create_ActionBase)(
			_In_ const PTR_DATA _data);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(game::Renderer *, create_Renderer)(
			_In_ const PTR_DATA _data);

	private:

		// 설명 : 
		typedef std::hash_map<KEY, PTR_DATA> GOE_DataMap;

		// 설명 : 
		typedef std::list<PTR_USER> GOE_UserList;

		// 설명 : 
		typedef std::hash_map<KEY, GOE_UserList> GOE_UserMap;

		// 설명 : 
		bullet::Bullet_Engine my_PhysicsEngine;

		// 설명 : 
		direct3d::D3D10_Terrain my_Terrain;

		// 설명 : 
		direct3d::D3D10_Mesh my_TerrainMesh;

		// 설명 : 
		direct3d::D3D10_Mesh my_SkyMesh;

		// 설명 :
		GOE_DataMap my_DataMap;

		// 설명 : 
		GOE_UserMap my_UserMap;

		// 설명 :


	};

}