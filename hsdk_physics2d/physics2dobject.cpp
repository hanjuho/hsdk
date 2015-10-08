#include <physics2d/physics2dobject.h>
#include <physics2d/interface/rigidbody2d.h>
#include <physics2d/interface/collider2d.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Physics2DObject, Physics2DObject)(
	/* [set] */ i::i_RigidBody2D * _rigidbody,
	/* [set] */ i::i_Collider2D * _collider)
	: my_RigidBody(_rigidbody), my_Collider(_collider)
{
	IF_FALSE(_rigidbody && _collider)
	{
		throw HSDK_FAIL;
	}

	initialize_Body();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Physics2DObject, i::i_RigidBody2D *, rigidBody)(
	/* [x] */ void)const
{
	return my_RigidBody;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Physics2DObject, i::i_Collider2D *, collider)(
	/* [x] */ void)const
{
	return my_Collider;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Physics2DObject, void, integrate_Velocity)(
	/* [r] */ const Vector2D & _gravity,
	/* [r] */ float _dt)
{
	move(my_RigidBody->velocity() * _dt);
	rotate(my_RigidBody->angularVelocity() * _dt);

	integrate_Forces(_gravity, 0.0f, _dt);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Physics2DObject, void, integrate_Forces)(
	/* [r] */ const Vector2D & _gravity,
	/* [r] */ float _torque,
	/* [r] */ float _dt)
{
	my_RigidBody->accelerate(
		(my_RigidBody->force() * my_RigidBody->mass() + _gravity) * (_dt / 2.0f));

	my_RigidBody->spin(
		my_RigidBody->angularVelocity() + (_torque * my_RigidBody->inertia() * (_dt / 2.0f)));
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Physics2DObject, void, initialize_Body)(
	/* [x] */ void)
{
	my_RigidBody->apply_Shape(my_Collider);
}