#include <hsdk/win/frame/d3d11.h>



int main(void)
{

}
/*


R_Boundary boundary(goA, goB);
FLOAT distance = vector2d::len(goA->position - goB->position);
boundary.distance = distance;


// ��� ������Ʈ�� ���� ��輱�� ���µ� ���� ���� ������Ʈ�� ã�Ƴ�.
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

	// ���ӵ� ���
	angularVelocity += ((torque * iI) *	(_dt * 0.5f));

	// �߷� ���
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

	// �����Ӵ� ���ӵ� ���
	_obj->move_Orient(angularVelocity * _dt);

	// �����Ӵ� �ӵ� ���
	_obj->move_Position(velocity * _dt);

	// �߷� ����
	integrate_Forces(_gravity, _dt);
}

// ���� :
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