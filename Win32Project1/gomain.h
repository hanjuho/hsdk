#pragma once



#include "godatatable.h"
#include "gomodelrenderer.h"



namespace go
{

	// 설명 : 
	DECL_FUNC_T(void, callback_CollisionResult)(
		_In_ const btPersistentManifold & _manifold,
		_In_ btScalar _timeStep);

	// 설명 : 
	extern std::vector<AutoDelete<game::GameObject*>> g_GameWorld;

}