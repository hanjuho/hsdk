#include <hsdk/win/frame/d3d11.h>



int main(void)
{

}
/*


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
	const float k_slop = 0.05f; // Penetration allowance
	const float percent = 0.4f; // Penetration percentage to correct

	float amass = _abody->mass();
	float bmass = _bbody->mass();

	float alpha = std::max(_m.penetration - k_slop, 0.0f) / (amass + bmass);
	Vector2D correction = _m.normal * percent * alpha;

	_aobj->positional_Correction(-correction * amass);
	_bobj->positional_Correction(correction * bmass);
}

*/