#include <physics2d/matrix2d.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Matrix2D, Matrix2D)(void) :
m00(0.0f),
m01(0.0f),
m10(0.0f),
m11(0.0f)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Matrix2D, Matrix2D)(
	/* [in] */ float _radians)
{
	float c = std::cos(_radians);
	float s = std::sin(_radians);

	m00 = c;
	m01 = -s;
	m10 = s;
	m11 = c;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Matrix2D, Matrix2D)(
	/* [in] */ float _a,
	/* [in] */ float _b,
	/* [in] */ float _c,
	/* [in] */ float _d) :
	m00(_a),
	m01(_b),
	m10(_c),
	m11(_d)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Matrix2D, const Vector2D, operator*)(
	/* [in] */ const Vector2D & _rhs) const
{
	return Vector2D(
		m00 * _rhs.x + m01 * _rhs.y, 
		m10 * _rhs.x + m11 * _rhs.y);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Matrix2D, const Matrix2D, operator*)(
	/* [in] */ const Matrix2D & _rhs) const
{
	return Matrix2D(
		m[0][0] * _rhs.m[0][0] + m[0][1] * _rhs.m[1][0],
		m[0][0] * _rhs.m[0][1] + m[0][1] * _rhs.m[1][1],
		m[1][0] * _rhs.m[0][0] + m[1][1] * _rhs.m[1][0],
		m[1][0] * _rhs.m[0][1] + m[1][1] * _rhs.m[1][1]);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Matrix2D, void, operator=)(
	/* [in] */ const Matrix2D & _rhs)
{
	m00 = _rhs.m00;
	m01 = _rhs.m01;
	m10 = _rhs.m10;
	m11 = _rhs.m11;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Matrix2D, void, operator=)(
	/* [in] */ float _radians)
{
	float c = std::cos(_radians);
	float s = std::sin(_radians);

	m00 = c;
	m01 = -s;
	m10 = s;
	m11 = c;
}