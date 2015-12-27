#pragma once



#include "common.h"
#include "hsdk/embeddedpattern.h"
#include "game/gameobject.h"
#include "game/physics/bullet_engine.h"



namespace go
{
	// 설명 : 
	DECL_STRUCT(GOAnimateMesh)
		: public direct3d::D3D10_Mesh
	{

		// 설명 : 
		direct3d::D3D10_Animation anim;

	};
	
	// 설명 : 
	DECL_STRUCT(GOE_Data)
	{

		// 설명 : 
		unsigned int cbsize;

		// 설명 : 
		unsigned int count;

		// 설명 : 
		unsigned int dataTable_Key;

		// 설명 : 
		unsigned int controller_key;
		
		// 설명 : 
		unsigned int actionBase_Key;

		// 설명 : 
		unsigned int renderer_Key;

		// 설명 : 
		btCollisionShape * shape;

	};

	// 설명 :
	typedef hsdk::game::GameObject GOE_Use;

	// 설명 : 
	typedef hsdk::EMBEDDED_ARCHITECTURE_PATTERN<GOE_Data, GOE_Use, unsigned short> GOE;

	// 설명 : 
	inline DECL_FUNC_T(void, callback_CollisionResult)(
		_In_ const btPersistentManifold & _manifold,
		_In_ btScalar _timeStep);

}



#include "gocommon.inl"