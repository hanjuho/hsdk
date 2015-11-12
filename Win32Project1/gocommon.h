#pragma once



#include "common.h"
#include "hsdk/embeddedpattern.h"
#include "game/gameobject.h"
#include "game/physics/bullet_engine.h"



namespace go
{

	// ���� : 
	typedef hsdk::EMBEDDED_ARCHITECTURE_PATTERN<btRigidBody *, game::i::i_GameObject, unsigned int> GOE;

	// ���� : 
	inline DECL_FUNC_T(void, callback_CollisionResult)(
		_In_ const btPersistentManifold & _manifold,
		_In_ btScalar _timeStep);

}



#include "gocommon.inl"