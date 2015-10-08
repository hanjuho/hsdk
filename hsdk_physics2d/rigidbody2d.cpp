#include <physics2d/rigidbody2d.h>
#include <physics2d/interface/collider2d.h>



using namespace hsdk;
using namespace physics2d;


// grobal function
IMPL_FUNC_T(float, compute_inertia)(
	/* [r] */ const Vector2D * _vertices,
	/* [r] */ unsigned int _size)
{
	// Calculate centroid and moment of interia
	float I = 0.0f;
	const float k_inv3 = 1.0f / 3.0f;

	for (unsigned int i1 = 0; i1 < _size; ++i1)
	{
		Vector2D p1(_vertices[i1]);
		unsigned int i2 = i1 + 1 < _size ? i1 + 1 : 0;
		Vector2D p2(_vertices[i2]);

		float D = vector2d::cross(p1, p2);

		float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
		float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
		I += (0.25f * k_inv3 * D) * (intx2 + inty2);
	}

	return I;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(RigidBody2D, RigidBody2D)(
	/* [r] */float sFriction,
	/* [r] */float dFriction,
	/* [r] */float restitution)
	: m_inertia(0.0f), m_mass(0.0f),
	m_SFriction(sFriction),
	m_DFriction(dFriction),
	m_Restitution(restitution)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RigidBody2D, void, apply_Shape)(
	/* [r] */ const i::i_Collider2D * _collider)
{
	float d = _collider->density();
	float m = d * _collider->area();
	m_mass = m ? 1.0f / m : 0.0f;

	unsigned int i;
	if (i = _collider->numOfVerties())
	{
		m_inertia = compute_inertia(
			_collider->vertices(), i) * d;
	}
	else
	{
		float r = _collider->radius();
		m_inertia = m * r * r;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RigidBody2D, void, impulse)(
	/* [r] */ const Vector2D & _impulse,
	/* [r] */ const Vector2D & _contact)
{
	accelerate(_impulse * mass());
	spin(vector2d::cross(_contact, _impulse) * inertia());
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RigidBody2D, float, mass)(
	/* [x] */ void)const
{
	return m_mass;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RigidBody2D, float, inertia)(
	/* [x] */ void)const
{
	return m_inertia;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RigidBody2D, float, s_Friction)(
	/* [x] */ void)const
{
	return m_SFriction;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RigidBody2D, float, d_Friction)(
	/* [x] */ void)const
{
	return m_DFriction;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RigidBody2D, float, restitution)(
	/* [x] */ void)const
{
	return m_Restitution;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RigidBody2D, void, fixed)(
	/* [r] */ void)
{
	m_inertia = 0.0f;
	m_mass = 0.0f;
}