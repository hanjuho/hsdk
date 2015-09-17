#include <physics2d/Collider2dcircle.h>
#include <physics2d/vector2d.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Collider2DCircle, Collider2DCircle)(
	/* [in] */ float _radius,
	/* [in] */ float _density)
	: my_Radius(_radius), my_Density(_density)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Collider2DCircle, unsigned int, type)(
	/* [none] */ void)const
{
	return 0;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Collider2DCircle, float, density)(
	/* [none] */ void)const
{
	return my_Density;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Collider2DCircle, float, area)(
	/* [none] */ void)const
{
	return my_Radius * PI * 2;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Collider2DCircle, float, radius)(
	/* [none] */ void)const
{
	return my_Radius;
}