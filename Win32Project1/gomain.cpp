#include "gomain.h"



//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, go::callback_CollisionResult)(
	_In_ const btPersistentManifold & _manifold,
	_In_ btScalar _timeStep)
{
	int a = 0;
}

//--------------------------------------------------------------------------------------
std::vector<AutoDelete<game::GameObject*>> go::g_GameWorld;