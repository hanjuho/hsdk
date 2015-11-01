#include <bullet/bullet_engine.h>
#include <algorithm>



using namespace hsdk::bullet;


//--------------------------------------------------------------------------------------
DECL_FUNC_T(int, compute_Pow2)(
	_In_ int _value)
{
	int pow2 = 1;
	while (pow2 < _value)
	{
		pow2 <<= 1;
	}

	return pow2;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Bullet_Engine, Bullet_Engine)(
	_In_ float _gravityscale)
	: my_GravityScale(_gravityscale)
{
	my_CollisionConfiguration = new btDefaultCollisionConfiguration();
	my_CollisionDispatcher = new btCollisionDispatcher(my_CollisionConfiguration);
	my_ConstraintSolver = new btSequentialImpulseConstraintSolver();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Bullet_Engine, setup0_World)(
	_In_ int _width,
	_In_ int _height,
	_In_ int _depth)
{
	if (my_DynamicsWorld)
	{
		return HSDK_FAIL;
	}

	//
	int w = abs(_width / 2);
	int d = abs(_depth / 2);

	//
	my_WorldMinSize = btVector3(-w, 0, -d);
	my_WorldMaxSize = btVector3(w, abs(_height), d);

	my_OverlappingPairCache = new btAxisSweep3(
		my_WorldMinSize,
		my_WorldMaxSize);

	//
	my_DynamicsWorld = new btDiscreteDynamicsWorld(
		my_CollisionDispatcher,
		my_OverlappingPairCache,
		my_ConstraintSolver,
		my_CollisionConfiguration);

	// reset gravity to point in appropriate direction
	my_DynamicsWorld->setGravity(btVector3(0, -BULLET_GRAVITY * my_GravityScale, 0));

	//
	return 0;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Bullet_Engine, setup1_Terrain)(
	_In_ int _cellX,
	_In_ int _cellY,
	_In_ const float * _heightBuffer,
	_In_ unsigned int _size)
{
	// 이미 터레인이 있다면 터레인 생성 불가
	if (1 < my_CollisionShapes.size())
	{
		return 1;
	}

	// 
	if (_cellX < 1 || _cellY < 1)
	{
		return HSDK_FAIL;
	}

	//
	int w = compute_Pow2(abs(_cellX));
	int y = compute_Pow2(abs(_cellY));

	//	std::cerr << "\nRegenerating terrain\n";
	//	std::cerr << "  model = " << model << "\n";
	//	std::cerr << "  type = " << type << "\n";
	btTransform form;
	form.setIdentity();

	//
	add(form, new btHeightfieldTerrainShape(
		w + 1, y + 1, _heightBuffer, 1.0f, 0,
		_FMAX, 1, PHY_FLOAT, false), 0.0f);

	add(form, new btBoxShape(
		btVector3(w / 2, _FMAX, y / 2)), 0.0f);

	// 반드시 첫번째 두번째 유닛은 터레인이어야만 함
	if (my_CollisionShapes.size() != 2)
	{
		reset();

		return HSDK_FAIL;
	}

	return 0;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Bullet_Engine, void, update)(
	_X_ float _time)
{
	my_DynamicsWorld->stepSimulation(_time);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Bullet_Engine, void, reset)(
	_X_ void)
{
	//delete collision shapes
	for (int index = 0; index < my_CollisionShapes.size(); index++)
	{
		btCollisionShape * shape = my_CollisionShapes[index];
		delete shape;
	}
	my_CollisionShapes.clear();

	//remove the rigidbodies from the dynamics world and delete them
	// 첫번째 유닛은 반드시 터레인이므로 reset()으로 밖에 지울 수 없다.
	for (int index = my_DynamicsWorld->getNumCollisionObjects() - 1; index != -1; --index)
	{
		btCollisionObject * obj = my_DynamicsWorld->getCollisionObjectArray()[index];
		btRigidBody * body = btRigidBody::upcast(obj);

		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}

		my_DynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	my_DynamicsWorld.~AutoDelete();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Bullet_Engine, btRigidBody *, add)(
	_In_ const btTransform & _form,
	_In_ btCollisionShape * _shape,
	_In_ float _mass)
{
	btAssert((!_shape || _shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	btVector3 localInertia(0, 0, 0);
	if (_mass != 0.f)
	{
		_shape->calculateLocalInertia(_mass, localInertia);
	}

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState =
		new btDefaultMotionState(_form);

	//
	btRigidBody::btRigidBodyConstructionInfo cInfo(
		_mass, myMotionState, _shape, localInertia);

	//
	btRigidBody* body = new btRigidBody(cInfo);
	body->setContactProcessingThreshold(0);

	// 
	my_CollisionShapes.push_back(_shape);
	my_DynamicsWorld->addRigidBody(body);

	//
	return body;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Bullet_Engine, void, remove)(
	_In_ btRigidBody * _body)
{
	my_DynamicsWorld->removeRigidBody(_body);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Bullet_Engine, void, clear)(
	_X_ void)
{
	//remove the rigidbodies from the dynamics world and delete them
	// 첫번째 유닛은 반드시 터레인이므로 reset()으로 밖에 지울 수 없다.
	for (int index = my_DynamicsWorld->getNumCollisionObjects() - 1; index != 1; --index)
	{
		btCollisionObject * obj = my_DynamicsWorld->getCollisionObjectArray()[index];
		btRigidBody * body = btRigidBody::upcast(obj);

		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}

		my_DynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	// 첫번째 유닛은 반드시 터레인이므로 reset()으로 밖에 지울 수 없다.
	for (int index = 2; index < my_CollisionShapes.size(); index++)
	{
		btCollisionShape * shape = my_CollisionShapes[index];
		my_CollisionShapes.remove(shape);
		delete shape;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Bullet_Engine, btVector3, minWorldSize)(
	_X_ void)
{
	return my_WorldMinSize;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Bullet_Engine, btVector3, maxWorldSize)(
	_X_ void)
{
	return my_WorldMaxSize;
}