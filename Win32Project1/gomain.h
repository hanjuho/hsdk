#pragma once



#include "godatatable.h"
#include "gomodelrenderer.h"



namespace go
{

	// ���� : 
	DECL_FUNC_T(void, callback_CollisionResult)(
		_In_ const btPersistentManifold & _manifold,
		_In_ btScalar _timeStep);

	// ���� : 
	extern std::vector<AutoDelete<game::GameObject*>> g_GameWorld;

}