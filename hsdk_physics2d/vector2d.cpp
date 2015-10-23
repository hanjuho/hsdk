#include <physics2d/vector2d.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Vector2D, Vector2D)(void)
: x(0.0f), y(0.0f)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Vector2D, Vector2D)(
	_In_ float _x,
	_In_ float _y)
	: x(_x), y(_y)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Vector2D, Vector2D, operator-)(
	/* [x] */ void) const
{
	return Vector2D(-x, -y);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Vector2D, Vector2D, operator+)(
	_In_ const Vector2D & _rhs) const
{
	return Vector2D(x + _rhs.x, y + _rhs.y);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Vector2D, Vector2D, operator-)(
	_In_ const Vector2D & _rhs) const
{
	return Vector2D(x - _rhs.x, y - _rhs.y);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Vector2D, void, operator+=)(
	_In_ const Vector2D & _rhs)
{
	x += _rhs.x;
	y += _rhs.y;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Vector2D, void, operator-=)(
	_In_ const Vector2D & _rhs)
{
	x -= _rhs.x;
	y -= _rhs.y;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Vector2D, Vector2D, operator+)(
	_In_ float _s) const
{
	return Vector2D(x + _s, y + _s);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Vector2D, Vector2D, operator-)(
	_In_ float _s) const
{
	return Vector2D(x - _s, y - _s);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Vector2D, Vector2D, operator*)(
	_In_ float _s) const
{
	return Vector2D(x * _s, y * _s);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Vector2D, Vector2D, operator/)(
	_In_ float _s) const
{
	return Vector2D(x / _s, y / _s);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Vector2D, void, operator+=)(
	_In_ float _s)
{
	x += _s;
	y += _s;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Vector2D, void, operator-=)(
	_In_ float _s)
{
	x -= _s;
	y -= _s;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Vector2D, void, operator*=)(
	_In_ float _s)
{
	x *= _s;
	y *= _s;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Vector2D, void, operator/=)(
	_In_ float _s)
{
	x /= _s;
	y /= _s;
}