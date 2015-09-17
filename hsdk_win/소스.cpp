#include <hsdk/win/frame/d3d11.h>



int main(void)
{

}
/*


R_Boundary boundary(goA, goB);
FLOAT distance = vector2d::len(goA->position - goB->position);
boundary.distance = distance;


// 모든 오브젝트는 원형 경계선을 갖는데 선을 넘은 오브젝트를 찾아냄.
if (distance < goA->boundary)
{
boundary.boundary_A = true;
}

if (distance < goB->boundary)
{
boundary.boundary_B = true;
}

if (boundary.boundary_A || boundary.boundary_B)
{
_rb.emplace_back(boundary);
}


//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, void, integrate_Forces)(
/* [in] */ const Vector2D & _gravity,
/* [in] */ float _dt)
{
	if (im == 0.0f)
	{
		return;
	}

	// 각속도 계산
	angularVelocity += ((torque * iI) *	(_dt * 0.5f));

	// 중력 계산
	velocity += (force * im + _gravity) * (_dt * 0.5f);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(RigidBody2D, void, integrate_Velocity)(
	/* [out] */ Object * _obj,
	/* [in] */ const Vector2D & _gravity,
	/* [in] */ float _dt)
{
	if (im == 0.0f)
	{
		return;
	}

	// 프레임당 각속도 계산
	_obj->move_Orient(angularVelocity * _dt);

	// 프레임당 속도 계산
	_obj->move_Position(velocity * _dt);

	// 중력 적용
	integrate_Forces(_gravity, _dt);
}

// 설명 :
DLL_DECL_FUNC_T(void, positional_Correction)(
/* [in] */ i::i_Physics2DObject * _aobj,
/* [in] */ i::i_Physics2DObject * _bobj,
/* [in] */ const Manifold2D & _m,
/* [in] */ const i::i_RigidBody2D * _abody,
/* [in] */ const i::i_RigidBody2D * _bbody,
/* [in] */ float _glength);


//--------------------------------------------------------------------------------------
DLL_DECL_FUNC_T(void, hsdk::physics2d::manifold::positional_Correction)(
	/* [in] */ i::i_Physics2DObject * _aobj,
	/* [in] */ i::i_Physics2DObject * _bobj,
	/* [in] */ const Manifold2D & _m,
	/* [in] */ const i::i_RigidBody2D * _abody,
	/* [in] */ const i::i_RigidBody2D * _bbody,
	/* [in] */ float _glength)
{
	
}

*/