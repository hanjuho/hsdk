#include <physics2d/manifold2d.h>
#include <physics2d/rigidbody2d.h>



using namespace hsdk;
using namespace physics2d;
using namespace manifold;


//--------------------------------------------------------------------------------------
REALIZE_FUNC_T(void, hsdk::physics2d::manifold::initialize)(
	/* [w] */ Manifold2D & _m,
	/* [r] */ const i::i_RigidBody2D * _abody,
	/* [r] */ const i::i_RigidBody2D * _bbody)
{
	// Calculate average restitution
	_m.e = std::min(_abody->restitution(), _bbody->restitution());

	// Calculate static and dynamic friction
	_m.sf = std::sqrt(_abody->s_Friction() * _bbody->s_Friction());
	_m.df = std::sqrt(_abody->d_Friction() * _bbody->d_Friction());
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC_T(void, hsdk::physics2d::manifold::impulse_Apply)(
	/* [w] */ Manifold2D & _m,
	/* [r] */ const Vector2D & _apos,
	/* [r] */ i::i_RigidBody2D * _abody,
	/* [r] */ const Vector2D & _bpos,
	/* [r] */ i::i_RigidBody2D * _bbody,
	/* [r] */ float _glength)
{
	float amass = _abody->mass();
	float bmass = _bbody->mass();

	// Early out and positional correct if both objects have infinite mass
	if (abs(amass + bmass) <= EPSILON)
	{
		_abody->accelerate(_abody->velocity());
		_bbody->accelerate(_bbody->velocity());

		return;
	}

	float ainertia = _abody->inertia();
	float binertia = _bbody->inertia();

	Vector2D avelocity = _abody->velocity();
	Vector2D bvelocity = _bbody->velocity();

	float aavelocity = _abody->angularVelocity();
	float bavelocity = _bbody->angularVelocity();

	Vector2D ra[2];
	Vector2D rb[2];
	Vector2D rv[2];
	for (unsigned int i = _m.contact_count - 1; i != -1; --i)
	{
		// Calculate radii from COM to contact
		ra[i] = _m.contacts[i] - _apos;
		rb[i] = _m.contacts[i] - _bpos;

		// Relative velocity
		rv[i] = bvelocity + vector2d::cross(aavelocity, rb[i]) -
			avelocity - vector2d::cross(bavelocity, ra[i]);

		// Determine if we should perform _aBody resting collision or not
		// The idea is if the only thing moving this object is gravity,
		// then the collision should be performed without any restitution
		if (vector2d::lenSqr(rv[i]) < (_glength + EPSILON))
		{
			_m.e = 0.0f;
		}
	}

	float e = _m.e;
	Vector2D & nor = _m.normal;

	for (unsigned int i = _m.contact_count - 1; i != -1; --i)
	{
		// Relative velocity along the normal
		float contactVel = vector2d::dot(rv[i], nor);

		// Do not resolve if velocities are separating
		if (contactVel > 0)
		{
			return;
		}

		float raCrossN = vector2d::cross(ra[i], nor);
		float rbCrossN = vector2d::cross(rb[i], nor);
		float invMassSum =
			amass + bmass +
			(raCrossN * raCrossN * ainertia) +
			(rbCrossN * rbCrossN * binertia);

		// Calculate impulse scalar
		float j = -(1.0f + e) * contactVel;
		j /= invMassSum;
		j /= float(_m.contact_count);


		// Apply impulse
		Vector2D impulse = nor * j;
		_abody->impulse(-impulse, ra[i]);
		_bbody->impulse(impulse, rb[i]);

		// Friction impulse
		rv[i] = bvelocity + vector2d::cross(bvelocity, rb[i]) -
			avelocity - vector2d::cross(avelocity, ra[i]);

		Vector2D t = rv[i] - (nor * vector2d::dot(rv[i], nor));
		vector2d::normalize(t, t);

		// j tangent magnitude
		float jt = -vector2d::dot(rv[i], t);
		jt /= invMassSum;
		jt /= float(_m.contact_count);

		// Don't apply tiny friction impulses
		if (abs(jt) <= EPSILON)
		{
			return;
		}

		// Coulumb's law
		Vector2D tangentImpulse;
		if (std::abs(jt) < j * _m.sf)
		{
			tangentImpulse = t * jt;
		}
		else
		{
			tangentImpulse = t * -j * _m.df;
		}

		// Apply friction impulse
		_abody->impulse(-tangentImpulse, ra[i]);
		_bbody->impulse(tangentImpulse, rb[i]);
	}
}