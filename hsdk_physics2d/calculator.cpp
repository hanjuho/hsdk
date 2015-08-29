#include <physics2d/calculator.h>
#include <physics2d/collision.h>
#include <Windows.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
std::hash_map<unsigned int, Calculator::Resource> Calculator::RESOURCES;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Calculator, void, set_Resource)(
	/* [in/ref] */ Object ** _stream,
	/* [in] */ unsigned int _offset,
	/* [in] */ unsigned int _size)
{
	Resource & res = RESOURCES[GetCurrentThreadId()];
	res.stream = _stream;
	res.offset = _offset;
	res.size = _size;
}

//-------------------------------------------------------------------------------------- 
CLASS_REALIZE_FUNC_T(Calculator, void, calculate)(
	/* [in] */ const Vector2D _gravity,
	/* [in] */ float _dt,
	/* [out] */ RESULT_BOUNDARY & _rb,
	/* [out] */ RESULT_CONTACT & _rc)
{
	_rb.clear();
	_rc.clear();

	Resource & res = RESOURCES[GetCurrentThreadId()];
	if (res.stream)
	{
		// ���� ������Ʈ.
		Object * goA = res.stream[res.offset], * goB;
		Object * const goEnd = goA + (res.offset + res.size);
		
		// ��ü.
		RigidBody * rigA;
		RigidBody * rigB;

		// �ݶ��̴�.
		i::i_Collider * colA;
		i::i_Collider * colB;

		// �浹 �˻�.
		for (; goA != goEnd; ++goA)
		{
			// Object A�� ��ȿ�˻�.
			if (goA && 
				(colA = goA->get_Collider()) && 
				(rigA = goA->get_RigidBody()))
			{
				/*
				������ �� �ʱ�ȭ goB = goA.
				++goB �� ���� ������Ʈ�� �ǹ�
				*/
				for (goB = goA + 1; goB != goEnd; ++goB)
				{
					// Object B�� ��ȿ�˻�.
					if (goB &&
						(colB = goB->get_Collider()) && 
						(rigB = goB->get_RigidBody()))
					{
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

						//
						IF_FALSE((goA->bWarnCompel | goB->bWarnCompel))
						{
							//
							if (goA->group == goB->group)
							{
								continue;
							}

							//
							IF_FALSE(goA->bCollider)
							{
								continue;
							}

							//
							IF_FALSE(goB->bCollider)
							{
								continue;
							}
						}
						
						//
						IF_FALSE(rigA->im + rigB->im)
						{
							continue;
						}

						R_Manifold manifold(goA, goB, rigA, rigB);
						Manifold & m = manifold.m;

						// �ݶ��̴� �浹 �˻�.
						collision::Collider_Dispatch[colA->get_Type()][colB->get_Type()](
							goA->position,
							goB->position,
							colA,
							colB,
							m);

						// �浹���� �ϳ� �̻��̶�� �ݶ��̴� �浹 ���ۿ� �ݶ��̴� �浹 ���� �߰�.
						if (m.contact_count)
						{
							//
							_rc.emplace_back(
								manifold);
						}
					}
				}						
			}
		}

		// gravity
		Vector2D g = _gravity * 1 / 60;
		FLOAT glength = vector2d::lenSqr(g);
		
		// Integrate forces
		for (goA = res.stream[res.offset]; goA != goEnd; ++goA)
		{
			if (goA && (rigA = goA->get_RigidBody()))
			{
				rigA->integrate_Forces(g, _dt);
			}
		}

		for (unsigned int i = _rc.size() - 1; i != -1; --i)
		{
			R_Manifold & rm = _rc[i];

			// Initialize collision
			manifold::initialize(
				rm.rig_A,
				rm.rig_B,
				rm.m);

			// Solve collisions
			manifold::impulse_Apply(
				glength,
				rm.obj_A->position,
				rm.obj_B->position,
				rm.rig_A,
				rm.rig_B,
				rm.m);
		}

		// Integrate velocities
		for (goA = res.stream[res.offset]; goA != goEnd; ++goA)
		{
			if (goA && (rigA = goA->get_RigidBody()))
			{
				rigA->integrate_Velocity(g, _dt, goA);
			}
		}

		// Correct positions
		for (unsigned int i = _rc.size() - 1; i != -1; --i)
		{
			R_Manifold & rm = _rc[i];

			manifold::positional_Correction(
				glength,
				rm.rig_A,
				rm.rig_B,
				rm.m,
				rm.obj_A->position,
				rm.obj_B->position);
		}

		// ptr_ObjBuffer ���� ������Ʈ �ʱ�ȭ
		for (goA = res.stream[res.offset]; goA != goEnd; ++goA)
		{
			if (goA && (rigA = goA->get_RigidBody()))
			{
				rigA->force.x = 0.0f;
				rigA->force.y = 0.0f;
				rigA->torque = 0;
			}
		}
	}
}