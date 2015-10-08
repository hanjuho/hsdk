#include <physics2d/matrix2d.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Matrix2D, Matrix2D)(void) :
m00(0.0f),
m01(0.0f),
m10(0.0f),
m11(0.0f)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Matrix2D, Matrix2D)(
	/* [r] */ float _radians)
{
	float c = std::cos(_radians);
	float s = std::sin(_radians);

	m00 = c;
	m01 = -s;
	m10 = s;
	m11 = c;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Matrix2D, Matrix2D)(
	/* [r] */ float _a,
	/* [r] */ float _b,
	/* [r] */ float _c,
	/* [r] */ float _d) :
	m00(_a),
	m01(_b),
	m10(_c),
	m11(_d)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Matrix2D, const Vector2D, operator*)(
	/* [r] */ const Vector2D & _rhs) const
{
	return Vector2D(
		m00 * _rhs.x + m01 * _rhs.y, 
		m10 * _rhs.x + m11 * _rhs.y);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Matrix2D, const Matrix2D, operator*)(
	/* [r] */ const Matrix2D & _rhs) const
{
	return Matrix2D(
		m[0][0] * _rhs.m[0][0] + m[0][1] * _rhs.m[1][0],
		m[0][0] * _rhs.m[0][1] + m[0][1] * _rhs.m[1][1],
		m[1][0] * _rhs.m[0][0] + m[1][1] * _rhs.m[1][0],
		m[1][0] * _rhs.m[0][1] + m[1][1] * _rhs.m[1][1]);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Matrix2D, void, operator=)(
	/* [r] */ const Matrix2D & _rhs)
{
	m00 = _rhs.m00;
	m01 = _rhs.m01;
	m10 = _rhs.m10;
	m11 = _rhs.m11;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Matrix2D, void, operator=)(
	/* [r] */ float _radians)
{
	float c = std::cos(_radians);
	float s = std::sin(_radians);

	m00 = c;
	m01 = -s;
	m10 = s;
	m11 = c;
}