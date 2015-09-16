#include <physics2d/rigidbody2d.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(RigidBody2D, RigidBody2D)(
	/* [in] */float sFriction,
	/* [in] */float dFriction,
	/* [in] */float restitution)
	: m_inertia(0.0f), m_mass(0.0f),
	m_SFriction(sFriction),
	m_DFriction(dFriction),
	m_Restitution(restitution),
	m_AngularVelocity(0.0f)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, void, apply_Shape)(
	/* [in] */ const i::i_Collider2D * _collider)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, void, apply_Force)(
	/* [in] */ const Vector2D & _vector)
{
	set_Force(get_Force() + _vector);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, void, apply_impulse)(
	/* [in] */ const Vector2D & _impulse,
	/* [in] */ const Vector2D & _contact)
{
	set_Velocity(
		get_Velocity() +
		_impulse * mass());

	set_AngularVelocity(
		get_AngularVelocity() +
		vector2d::cross(_contact, _impulse) * inertia());
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, void, set_Velocity)(
	/* [in] */ const Vector2D & _vector)
{
	m_Velocity = _vector;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, Vector2D, get_Velocity)(
	/* [none] */ void)const
{
	return m_Velocity;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, void, set_Force)(
	/* [in] */ const Vector2D & _vector)
{
	m_Force = _vector;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, Vector2D, get_Force)(
	/* [none] */ void)const
{
	return m_Force;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, void, set_AngularVelocity)(
	/* [in] */ float _value)
{
	m_AngularVelocity = _value;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, float, get_AngularVelocity)(
	/* [none] */ void)const
{
	return m_AngularVelocity;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, float, mass)(
	/* [none] */ void)const
{
	return m_mass;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, float, inertia)(
	/* [none] */ void)const
{
	return m_inertia;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, float, s_Friction)(
	/* [none] */ void)const
{
	return m_SFriction;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, float, d_Friction)(
	/* [none] */ void)const
{
	return m_DFriction;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, float, restitution)(
	/* [none] */ void)const
{
	return m_Restitution;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, void, fixed)(
	/* [in] */ void)
{
	m_inertia = 0.0f;
	m_mass = 0.0f;
}