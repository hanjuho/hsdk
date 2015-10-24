#include <physics2d/Collider2dcircle.h>
#include <physics2d/vector2d.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Collider2DCircle, Collider2DCircle)(
	_In_ float _radius,
	_In_ float _density)
	: my_Radius(_radius), my_Density(_density)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DCircle, unsigned int, type)(
	_X_ void)const
{
	return 0;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DCircle, unsigned int, numOfVerties)(
	_X_ void)const
{
	return 0;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DCircle, Vector2D, vertex)(
	_In_ unsigned int _index)const
{
	return Vector2D(0.0f, 0.0f);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DCircle, const Vector2D *, vertices)(
	_X_ void)const
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DCircle, Vector2D, normal)(
	_In_ unsigned int _index)const
{
	return Vector2D(0.0f, 0.0f);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DCircle, const Vector2D *, normals)(
	_X_ void)const
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DCircle, float, density)(
	_X_ void)const
{
	return my_Density;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DCircle, float, area)(
	_X_ void)const
{
	return my_Radius * PI * 2;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DCircle, float, radius)(
	_X_ void)const
{
	return my_Radius;
}