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
	/* [out] */ RESULT_BOUNDARY & _rb,
	/* [out] */ RESULT_CONTACT & _rc,
	/* [in] */ const Terrain2D * _terrain,
	/* [in] */ const Vector2D _gravity,
	/* [in] */ float _dt)
{
	_rb.clear();
	_rc.clear();

	Resource & res = RESOURCES[GetCurrentThreadId()];
	if (res.stream)
	{
		// 게임 오브젝트.
		Object * goA = res.stream[res.offset], *goB;
		Object * const goEnd = goA + (res.offset + res.size);

		// 강체.
		RigidBody * rigA;
		RigidBody * rigB;

		// 콜라이더.
		i::i_Collider * colA;
		i::i_Collider * colB;

		// 충돌 검사.
		for (; goA != goEnd; ++goA)
		{
			// Object A의 유효검사.
			if (goA &&
				(colA = goA->get_Collider()) &&
				(rigA = goA->get_RigidBody()))
			{
				/*
				이전에 값 초기화 goB = goA.
				++goB 는 다음 오브젝트를 의미
				*/
				for (goB = goA + 1; goB != goEnd; ++goB)
				{
					// Object B의 유효검사.
					if (goB &&
						(colB = goB->get_Collider()) &&
						(rigB = goB->get_RigidBody()))
					{
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

						// 콜라이더 충돌 검사.
						collision::Collider_Dispatch[colA->get_Type()][colB->get_Type()](
							m,
							colA,
							colB,
							goA->position,
							goB->position);

						// 충돌점이 하나 이상이라면 콜라이더 충돌 버퍼에 콜라이더 충돌 정보 추가.
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
				rm.m,
				rm.rig_A,
				rm.rig_B);

			// Solve collisions
			manifold::impulse_Apply(
				rm.m,
				rm.rig_A,
				rm.rig_B,
				rm.obj_A->position,
				rm.obj_B->position,
				glength);
		}

		// Integrate velocities
		for (goA = res.stream[res.offset]; goA != goEnd; ++goA)
		{
			if (goA && (rigA = goA->get_RigidBody()))
			{
				rigA->integrate_Velocity(goA, g, _dt);
			}
		}

		// Correct positions
		for (unsigned int i = _rc.size() - 1; i != -1; --i)
		{
			R_Manifold & rm = _rc[i];

			manifold::positional_Correction(
				rm.obj_A->position,
				rm.obj_B->position,
				rm.m,
				rm.rig_A,
				rm.rig_B,
				glength);
		}

		// ptr_ObjBuffer 버퍼 오브젝트 초기화
		for (goA = res.stream[res.offset]; goA != goEnd; ++goA)
		{
			if (goA && (rigA = goA->get_RigidBody()))
			{
				rigA->force.x = 0.0f;
				rigA->force.y = 0.0f;
				rigA->torque = 0;
			}
		}

		// 지면 충돌 검사
		if (_terrain)
		{
			for (goA = res.stream[res.offset]; goA != goEnd; ++goA)
			{
				if (goA == nullptr)
				{
					break;
				}

				float yy;
				float dy;

				yy = _terrain->compute_Height(
					goA->position.x,
					&dy);

				if (yy == 0xfffffff)
				{
					break;
				}

				if (!(yy < goA->position.y))
				{
					goA->position.y = yy;
					if (rigA = goA->get_RigidBody())
					{
						// 지면과 충돌했음으로 속도를 감소.
						rigA->velocity;
					}
				}
			}
		}
	}
}