#include <physics2d/circle.h>
#include <physics2d/rigidbody.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Circle, Circle)(
	/* [in] */ float _radius)
	: radius(_radius)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Circle, i::i_Collider::COLLIDER_TYPE, get_Type)(
	/* [none] */ void)const
{
	return i::i_Collider::COLLIDER_CIRCLE;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Circle, void, set_Orient)(
	/* [in] */ float _radians)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Circle, void, apply_Mass)(
	/* [in] */ float _density)
{
	density = _density;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Circle, void, apply_Body)(
	/* [out] */ RigidBody * (&_body),
	/* [in] */ bool _inverse)
{
	if (_inverse)
	{
		_body->m -= PI * radius * radius * density;
		_body->im -= (_body->m) ? 1.0f / _body->m : 0.0f;
		_body->I -= _body->m * radius * radius;
		_body->iI -= (_body->I) ? 1.0f / _body->I : 0.0f;
	}
	else
	{
		_body->m += PI * radius * radius * density;
		_body->im += (_body->m) ? 1.0f / _body->m : 0.0f;
		_body->I += _body->m * radius * radius;
		_body->iI += (_body->I) ? 1.0f / _body->I : 0.0f;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Circle, i::i_Collider *, clone)(
	/* [none] */ void)const
{
	return new Circle(radius);
}