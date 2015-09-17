#include <physics2d/physics2dobject.h>
#include <physics2d/interface/rigidbody2d.h>
#include <physics2d/interface/collider2d.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Physics2DObject, void, set_RigidBody)(
	/* [include] */ i::i_RigidBody2D * _rigidbody)
{
	my_Rigid = _rigidbody;
	if (my_Shape)
	{
		_rigidbody->apply_Shape(my_Shape);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Physics2DObject, i::i_RigidBody2D *, get_RigidBody)(
	/* [none] */ void)const
{
	return my_Rigid;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Physics2DObject, void, set_Collider)(
	/* [include] */ i::i_Collider2D * _collider)
{
	my_Shape = _collider;
	if (my_Rigid)
	{
		my_Rigid->apply_Shape(_collider);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Physics2DObject, i::i_Collider2D *, get_Collider)(
	/* [none] */ void)const
{
	return my_Shape;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Physics2DObject, void, integrate_Velocity)(
	/* [in] */ const Vector2D & _gravity,
	/* [in] */ float _dt)
{
	if (my_Rigid)
	{
		move(my_Rigid->velocity() * _dt);
		rotate(my_Rigid->angularVelocity() * _dt);

		integrate_Forces(_gravity, 0.0f, _dt);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Physics2DObject, void, integrate_Forces)(
	/* [in] */ const Vector2D & _gravity,
	/* [in] */ float _torque,
	/* [in] */ float _dt)
{
	if (my_Rigid)
	{
		my_Rigid->accelerate(
			(my_Rigid->force() * my_Rigid->mass() + _gravity) * (_dt / 2.0f));

		my_Rigid->spin(
			my_Rigid->angularVelocity() + (_torque * my_Rigid->inertia() * (_dt / 2.0f)));
	}
}