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

	// ���ӵ� ���
	angularVelocity += ((torque * iI) *	(_dt * 0.5f));

	// �߷� ���
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

	// �����Ӵ� ���ӵ� ���
	_obj->move_Orient(angularVelocity * _dt);

	// �����Ӵ� �ӵ� ���
	_obj->move_Position(velocity * _dt);

	// �߷� ����
	integrate_Forces(_gravity, _dt);
}