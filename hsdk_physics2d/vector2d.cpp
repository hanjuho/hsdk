#include <physics2d/vector2d.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Vector2D, Vector2D)(void)
: x(0.0f), y(0.0f)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Vector2D, Vector2D)(
	/* [in] */ float _x,
	/* [in] */ float _y)
	: x(_x), y(_y)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Vector2D, Vector2D, operator-)(
	/* [none] */ void) const
{
	return Vector2D(-x, -y);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Vector2D, Vector2D, operator+)(
	/* [in] */ const Vector2D & _rhs) const
{
	return Vector2D(x + _rhs.x, y + _rhs.y);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Vector2D, Vector2D, operator-)(
	/* [in] */ const Vector2D & _rhs) const
{
	return Vector2D(x - _rhs.x, y - _rhs.y);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Vector2D, void, operator+=)(
	/* [in] */ const Vector2D & _rhs)
{
	x += _rhs.x;
	y += _rhs.y;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Vector2D, void, operator-=)(
	/* [in] */ const Vector2D & _rhs)
{
	x -= _rhs.x;
	y -= _rhs.y;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Vector2D, Vector2D, operator+)(
	/* [in] */ float _s) const
{
	return Vector2D(x + _s, y + _s);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Vector2D, Vector2D, operator-)(
	/* [in] */ float _s) const
{
	return Vector2D(x - _s, y - _s);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Vector2D, Vector2D, operator*)(
	/* [in] */ float _s) const
{
	return Vector2D(x * _s, y * _s);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Vector2D, Vector2D, operator/)(
	/* [in] */ float _s) const
{
	return Vector2D(x / _s, y / _s);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Vector2D, void, operator+=)(
	/* [in] */ float _s)
{
	x += _s;
	y += _s;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Vector2D, void, operator-=)(
	/* [in] */ float _s)
{
	x -= _s;
	y -= _s;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Vector2D, void, operator*=)(
	/* [in] */ float _s)
{
	x *= _s;
	y *= _s;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Vector2D, void, operator/=)(
	/* [in] */ float _s)
{
	x /= _s;
	y /= _s;
}