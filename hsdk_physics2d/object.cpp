#include <physics2d/object.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Object, void, move_Orient)(
	/* [in] */ float _dRadians)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Object, void, move_Position)(
	/* [in] */ const Vector2D & _dPos)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Object, i::i_Collider *, get_Collider)(
	/* [none] */ void)
{
	return my_Shape;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Object, void, set_Collider)(
	/* [include] */ i::i_Collider * _collider)
{
	my_Shape = _collider;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Object, RigidBody *, get_RigidBody)(
	/* [none] */ void)
{
	return my_Rigid;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Object, void, set_RigidBody)(
	/* [include] */ RigidBody * _body)
{
	my_Rigid = _body;
}