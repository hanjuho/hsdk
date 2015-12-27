#pragma once



#include "common.h"
#include "hsdk/embeddedpattern.h"
#include "game/gameobject.h"
#include "game/physics/bullet_engine.h"



namespace go
{
	// ���� : 
	DECL_STRUCT(GOAnimateMesh)
		: public direct3d::D3D10_Mesh
	{

		// ���� : 
		direct3d::D3D10_Animation anim;

	};
	
	// ���� : 
	DECL_STRUCT(GOE_Data)
	{

		// ���� : 
		unsigned int cbsize;

		// ���� : 
		unsigned int count;

		// ���� : 
		unsigned int dataTable_Key;

		// ���� : 
		unsigned int controller_key;
		
		// ���� : 
		unsigned int actionBase_Key;

		// ���� : 
		unsigned int renderer_Key;

		// ���� : 
		btCollisionShape * shape;

	};

	// ���� :
	typedef hsdk::game::GameObject GOE_Use;

	// ���� : 
	typedef hsdk::EMBEDDED_ARCHITECTURE_PATTERN<GOE_Data, GOE_Use, unsigned short> GOE;

	// ���� : 
	inline DECL_FUNC_T(void, callback_CollisionResult)(
		_In_ const btPersistentManifold & _manifold,
		_In_ btScalar _timeStep);

}



#include "gocommon.inl"