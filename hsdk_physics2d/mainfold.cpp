#include <physics2d/manifold.h>
#include <physics2d/rigidbody.h>



using namespace hsdk;
using namespace physics2d;
using namespace manifold;


//--------------------------------------------------------------------------------------
DLL_DECL_FUNC_T(void, hsdk::physics2d::manifold::initialize)(
	/* [in] */ RigidBody * _aBody,
	/* [in] */ RigidBody * _bBody,
	/* [in/out] */ Manifold & _m)
{
		// Calculate average restitution
	_m.e = std::min(_aBody->restitution, _bBody->restitution);

	// Calculate static and dynamic friction
	_m.sf = std::sqrt(_aBody->staticFriction * _bBody->staticFriction);
	_m.df = std::sqrt(_aBody->dynamicFriction * _bBody->dynamicFriction);
}

//--------------------------------------------------------------------------------------
DLL_DECL_FUNC_T(void, hsdk::physics2d::manifold::impulse_Apply)(
	/* [in] */ float _glength,
	/* [in] */ Vector2D & _aPos,
	/* [in] */ Vector2D & _bPos,
	/* [in] */ RigidBody * _aBody,
	/* [in] */ RigidBody * _bBody,
	/* [out] */ Manifold & _m)
{
	// Early out and positional correct if both objects have infinite mass
	if (abs(_aBody->im + _bBody->im) <= EPSILON)
	{
		_aBody->velocity.x = 0.0f;
		_aBody->velocity.y = 0.0f;

		_bBody->velocity.x = 0.0f;
		_bBody->velocity.y = 0.0f;

		return;
	}

	Vector2D ra[2];
	Vector2D rb[2];
	Vector2D rv[2];
	for (unsigned int i = _m.contact_count - 1; i != -1; --i)
	{
		// Calculate radii from COM to contact
		ra[i] = _m.contacts[i] - _aPos;
		rb[i] = _m.contacts[i] - _bPos;

		// Relative velocity
		rv[i] = _bBody->velocity + vector2d::cross(_bBody->angularVelocity, rb[i]) -
			_aBody->velocity - vector2d::cross(_aBody->angularVelocity, ra[i]);

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
			_aBody->im + _bBody->im +
			(raCrossN * raCrossN * _aBody->iI) +
			(rbCrossN * rbCrossN * _bBody->iI);

		// Calculate impulse scalar
		float j = -(1.0f + e) * contactVel;
		j /= invMassSum;
		j /= float(_m.contact_count);


		// Apply impulse
		Vector2D impulse = nor * j;
		_aBody->apply_impulse(-impulse, ra[i]);
		_bBody->apply_impulse(impulse, rb[i]);

		// Friction impulse
		rv[i] = _bBody->velocity + vector2d::cross(_bBody->angularVelocity, rb[i]) -
			_aBody->velocity - vector2d::cross(_aBody->angularVelocity, ra[i]);

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
		_aBody->apply_impulse(-tangentImpulse, ra[i]);
		_bBody->apply_impulse(tangentImpulse, rb[i]);
	}
}

//--------------------------------------------------------------------------------------
DLL_DECL_FUNC_T(void, hsdk::physics2d::manifold::positional_Correction)(
	/* [in] */ float _glength,
	/* [in] */ RigidBody * _aBody,
	/* [in] */ RigidBody * _bBody,
	/* [in] */ Manifold & _m,
	/* [out] */ Vector2D & _aPos,
	/* [out] */ Vector2D & _bPos)
{
	const float k_slop = 0.05f; // Penetration allowance
	const float percent = 0.4f; // Penetration percentage to correct

	float alpha = std::max(_m.penetration - k_slop, 0.0f) / (_aBody->im + _bBody->im);
	Vector2D correction = _m.normal * percent * alpha;
	_aPos -= correction * _aBody->im;
	_bPos += correction * _bBody->im;
}