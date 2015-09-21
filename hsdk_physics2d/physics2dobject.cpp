#include <physics2d/physics2dobject.h>
#include <physics2d/interface/rigidbody2d.h>
#include <physics2d/interface/collider2d.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Physics2DObject, Physics2DObject)(
	/* [include] */ i::i_RigidBody2D * _rigidbody,
	/* [include] */ i::i_Collider2D * _collider)
	: my_RigidBody(_rigidbody), my_Collider(_collider)
{
	initialize_Body();
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Physics2DObject, i::i_RigidBody2D *, rigidBody)(
	/* [none] */ void)const
{
	return my_RigidBody;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Physics2DObject, i::i_Collider2D *, collider)(
	/* [none] */ void)const
{
	return my_Collider;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Physics2DObject, void, integrate_Velocity)(
	/* [in] */ const Vector2D & _gravity,
	/* [in] */ float _dt)
{
	if (my_RigidBody)
	{
		move(my_RigidBody->velocity() * _dt);
		rotate(my_RigidBody->angularVelocity() * _dt);

		integrate_Forces(_gravity, 0.0f, _dt);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Physics2DObject, void, integrate_Forces)(
	/* [in] */ const Vector2D & _gravity,
	/* [in] */ float _torque,
	/* [in] */ float _dt)
{
	if (my_RigidBody)
	{
		my_RigidBody->accelerate(
			(my_RigidBody->force() * my_RigidBody->mass() + _gravity) * (_dt / 2.0f));

		my_RigidBody->spin(
			my_RigidBody->angularVelocity() + (_torque * my_RigidBody->inertia() * (_dt / 2.0f)));
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Physics2DObject, void, initialize_Body)(
	/* [none] */ void)
{
	IF_FALSE(my_Collider)
	{
		return;
	}

	IF_FALSE(my_RigidBody)
	{
		return;
	}
		
	my_RigidBody->apply_Shape(my_Collider);
}