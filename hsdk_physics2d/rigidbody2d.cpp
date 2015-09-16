#include <physics2d/rigidbody.h>
#include <physics2d/object.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(RigidBody, RigidBody)(
	/* [in] */ float _vx,
	/* [in] */ float _vy,
	/* [in] */ float _av)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody, void, apply_Force)(
	/* [in] */ const Vector2D & _vector)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody, void, apply_impulse)(
	/* [in] */ const Vector2D & _impulse,
	/* [in] */ const Vector2D & _contact)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody, void, integrate_Forces)(
	/* [in] */ const Vector2D & _gravity,
	/* [in] */ float _dt)
{
	if (im == 0.0f)
	{
		return;
	}

	// 각속도 계산
	angularVelocity += ((torque * iI) *	(_dt * 0.5f));

	// 중력 계산
	velocity += (force * im + _gravity) * (_dt * 0.5f);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody, void, integrate_Velocity)(
	/* [out] */ Object * _obj,
	/* [in] */ const Vector2D & _gravity,
	/* [in] */ float _dt)
{
	if (im == 0.0f)
	{
		return;
	}

	// 프레임당 각속도 계산
	_obj->move_Orient(angularVelocity * _dt);

	// 프레임당 속도 계산
	_obj->move_Position(velocity * _dt);

	// 중력 적용
	integrate_Forces(_gravity, _dt);
}