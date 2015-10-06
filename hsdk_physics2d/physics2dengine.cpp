#include <physics2d/interface/terrain2d.h>
#include <physics2d/interface/physics2dobject.h>
#include <physics2d/interface/collision2dlistener.h>
#include <physics2d/interface/boundary2dlistener.h>
#include <physics2d/physics2dengine.h>
#include <physics2d/collider2dcircle.h>
#include <physics2d/Collider2dpolygon.h>
#include <physics2d/matrix2d.h>
#include <Windows.h>
#include <assert.h>



using namespace hsdk;
using namespace physics2d;


// grobal function
REALIZE_FUNC_T(unsigned int, clip)(
	/* [w] */ Vector2D(&_in_out)[2],
	/* [r] */ const Vector2D & _n,
	/* [r] */ float _c)
{
	unsigned int sp = 0;
	Vector2D out[2] = {
		_in_out[0],
		_in_out[1]
	};

	// Retrieve distances from each endpoint to the line
	// d = ax + by - c
	float d1 = vector2d::dot(_n, _in_out[0]) - _c;
	float d2 = vector2d::dot(_n, _in_out[1]) - _c;

	// If negative (behind plane) clip
	if (d1 <= 0.0f)
	{
		out[sp++] = _in_out[0];
	}	
	else if (d2 <= 0.0f)
	{
		out[sp++] = _in_out[1];
	}

	// If the points are on different sides of the plane
	if (d1 * d2 < 0.0f) // less than to ignore -0.0f
	{
		// Push interesection point
		float alpha = d1 / (d1 - d2);
		out[sp] = _in_out[0] + (_in_out[1] - _in_out[0]) * alpha;
		++sp;
	}

	// Assign our new converted values
	_in_out[0] = out[0];
	_in_out[1] = out[1];

	assert(sp != 2);

	return sp;
}

//--------------------------------------------------------------------------------------
std::hash_map<unsigned int, std::vector<Physics2DEngine::My_SOURCE_DESC>> Physics2DEngine::RESOURCES;

//-------------------------------------------------------------------------------------- 
CLASS_REALIZE_CONSTRUCTOR(Physics2DEngine::My_SOURCE_DESC, My_SOURCE_DESC)(void)
: object(nullptr), listener(nullptr)
{

}

//-------------------------------------------------------------------------------------- 
CLASS_REALIZE_CONSTRUCTOR(Physics2DEngine::My_SOURCE_DESC, My_SOURCE_DESC)(
	/* [r] */ i::i_Physics2DObject * _object,
	/* [r] */ i::i_Collision2DListener * _listner,
	/* [r] */ long _valid)
	: object(_object), listener(_listner), valid(_valid)
{

}

//-------------------------------------------------------------------------------------- 
CLASS_REALIZE_DESTRUCTOR(Physics2DEngine::My_SOURCE_DESC, My_SOURCE_DESC)(void)
{
	if (valid < 0)
	{
		if (IS_FLAG(valid, 0x01))
		{
			if (object)
			{
				delete object;
			}
		}

		if (IS_FLAG(valid, 0x02))
		{
			if (listener)
			{
				delete listener;
			}
		}
	}
}

//-------------------------------------------------------------------------------------- 
CLASS_REALIZE_FUNC_T(Physics2DEngine::My_SOURCE_DESC, void, operator =)(
	/* [r] */ const SOURCE_DESC & _desc)
{
	new(this) My_SOURCE_DESC(
		_desc.object, this->listener, _desc.valid);
}

//-------------------------------------------------------------------------------------- 
CLASS_REALIZE_FUNC_T(Physics2DEngine, void, find)(
	/* [r] */ i::i_Physics2DObject * _object,
	/* [r] */ i::i_Boundary2DListener * _listener,
	/* [r] */ float _distance,
	/* [r] */ long _sort)
{

}

//-------------------------------------------------------------------------------------- 
CLASS_REALIZE_FUNC_T(Physics2DEngine, void, operate)(
	/* [r] */ i::i_Physics2DObject * _objA,
	/* [r] */ i::i_Physics2DObject * _objB,
	/* [r] */ const i::i_Terrain2D * _terrain,
	/* [r] */ void * _param)
{
	Manifold2D & m =
		*(Manifold2D *)(_param);
	m.contact_count = 0;

	if (_objA->group_id() == _objB->group_id())
	{
		return;
	}

	IF_FALSE(_objA->is_Collision())
	{
		return;
	}

	IF_FALSE(_objB->is_Collision())
	{
		return;
	}

	i::i_Collider2D * colA = _objA->collider();
	i::i_Collider2D * colB = _objB->collider();

	const unsigned int typeA = colA->type();
	const unsigned int typeB = colB->type();

	// 최소 충돌 검사
	const Vector2D g_apos = _objA->position();
	const Vector2D g_bpos = _objB->position();
	const Vector2D g_dir = g_bpos - g_apos;

	const float g_distance = vector2d::len(g_dir);
	const float g_sum_radius = colA->radius() + colB->radius();

	if (g_distance < g_sum_radius)
	{
		return;
	}

	// 최대 충돌 검사
	switch (typeA + typeB)
	{
	case 0:
	{
			  m.contact_count = 1;

			  if (g_distance == 0.0f)
			  {
				  m.penetration = colA->radius();
				  m.normal = Vector2D(1.0f, 0.0f);
				  m.contacts[0] = g_apos;
			  }
			  else
			  {
				  m.penetration = g_sum_radius - g_distance;
				  // Faster than using Normalized since we already performed sqrt
				  m.normal = g_dir / g_distance;
				  m.contacts[0] = (m.normal * colA->radius()) + g_apos;
			  }
	}
		return;
	case 1:
	{
			  const Collider2DCircle * ac;
			  const Collider2DPolygon * bc;

			  // Transform Collider2DCircle center to Collider2DPolygon model space
			  Matrix2D mat;

			  if (typeA)
			  {
				  ac = reinterpret_cast<const Collider2DCircle *>(colB);
				  bc = reinterpret_cast<const Collider2DPolygon *>(colA);

				  mat = _objA->matrix();
			  }
			  else
			  {
				  ac = reinterpret_cast<const Collider2DCircle *>(colA);
				  bc = reinterpret_cast<const Collider2DPolygon *>(colB);

				  mat = _objB->matrix();
			  }

			  float radius = ac->radius();

			  //
			  Matrix2D brmat;
			  const Vector2D center = matrix2d::transpose(brmat, mat) * (g_dir);

			  // Find edge with minimum penetration
			  // Exact concept as using support points in Collider2DPolygon vs Collider2DPolygon
			  float separation = -FLT_MAX;
			  unsigned int faceNormal = 0;

			  const Vector2D * vertices = bc->vertices();
			  const Vector2D * normals = bc->normals();
			  unsigned int vetex_count = bc->numOfVerties();
			  for (unsigned int i = 0; i < vetex_count; ++i)
			  {
				  float s = vector2d::dot(
					  normals[i], center - vertices[i]);

				  if (s > radius)
				  {
					  return;
				  }

				  if (s > separation)
				  {
					  separation = s;
					  faceNormal = i;
				  }
			  }

			  // Grab face's vertices
			  Vector2D v1 = vertices[faceNormal];
			  unsigned int i2 = faceNormal + 1;
			  i2 = i2 < vetex_count ? i2 : 0;
			  Vector2D v2 = vertices[i2];

			  // Check to see if center is within Collider2DPolygon
			  if (separation < EPSILON)
			  {
				  m.contact_count = 1;
				  m.normal = -(mat * normals[faceNormal]);
				  m.contacts[0] = m.normal * radius + g_apos;
				  m.penetration = radius;
				  return;
			  }

			  // Determine which voronoi region of the edge center of Collider2DCircle lies within
			  float dot1 = vector2d::dot(center - v1, v2 - v1);
			  float dot2 = vector2d::dot(center - v2, v1 - v2);
			  m.penetration = radius - separation;

			  // Closest to v1
			  if (dot1 <= 0.0f)
			  {
				  if (vector2d::distSqr(center, v1) > (radius * radius))
				  {
					  return;
				  }

				  m.contact_count = 1;

				  // dir = 원의 중심 - 사각형의 중심.
				  Vector2D dir = v1 - center;

				  // dir에 사각형의 매트릭스 적용, 노말 계산.
				  vector2d::normalize(m.normal, mat * dir);

				  // 접점.
				  m.contacts[0] = mat * v1 + g_bpos;
			  }

			  // Closest to v2
			  else if (dot2 <= 0.0f)
			  {
				  if (vector2d::distSqr(center, v2) > (radius * radius))
				  {
					  return;
				  }

				  m.contact_count = 1;

				  //
				  v2 = (mat * v2);
				  vector2d::normalize(m.normal, v2 - center);
				  m.contacts[0] = v2 + g_bpos;
			  }

			  // Closest to face
			  else
			  {
				  Vector2D n = bc->normals()[faceNormal];
				  if (vector2d::dot(center - v1, n) > radius)
				  {
					  return;
				  }

				  m.contact_count = 1;

				  //
				  m.normal = -(mat * n);
				  m.contacts[0] = m.normal * radius + g_apos;
			  }
	}
		return;
	case 2:
	{
			  const Collider2DPolygon * ac = reinterpret_cast<const Collider2DPolygon *>(colA);
			  const Collider2DPolygon * bc = reinterpret_cast<const Collider2DPolygon *>(colB);
			  m.contact_count = 0;

			  const Matrix2D amat = _objA->matrix();
			  const Matrix2D bmat = _objB->matrix();

			  // Check for a separating axis with A's face planes
			  float penetrationA = 0;
			  unsigned int faceA;

			  const Vector2D * vertices = ac->vertices();
			  const Vector2D * nomals = ac->normals();
			  unsigned int vetext_count = ac->numOfVerties();
			  for (unsigned int i = 0; i < vetext_count; ++i)
			  {
				  // Retrieve a face normal from A
				  Vector2D n = nomals[i];
				  Vector2D nw = amat * n;

				  // Transform face normal into B's model space
				  Matrix2D buT;
				  matrix2d::transpose(buT, bmat);
				  n = buT * nw;

				  // Retrieve support point from B along -n
				  Vector2D s = bc->support(-n);

				  // Retrieve vertex on face from A, transform into
				  // B's model space
				  Vector2D v = vertices[i];
				  v = amat * v + g_apos;
				  v -= g_bpos;
				  v = buT * v;

				  // Compute penetration distance (in B's model space)
				  float d = vector2d::dot(n, s - v);

				  // Store greatest distance
				  if (d > penetrationA)
				  {
					  penetrationA = d;
					  faceA = i;
				  }
			  }

			  if (penetrationA >= 0.0f)
			  {
				  return;
			  }

			  // Check for a separating axis with B's face planes
			  float penetrationB = 0;
			  unsigned int faceB;

			  vertices = bc->vertices();
			  nomals = bc->normals();
			  vetext_count = bc->numOfVerties();
			  for (unsigned int i = 0; i < vetext_count; ++i)
			  {
				  // Retrieve a face normal from A
				  Vector2D n = nomals[i];
				  Vector2D nw = bmat * n;

				  // Transform face normal into B's model space
				  Matrix2D buT;
				  matrix2d::transpose(buT, amat);
				  n = buT * nw;

				  // Retrieve support point from B along -n
				  Vector2D s = ac->support(-n);

				  // Retrieve vertex on face from A, transform into
				  // B's model space
				  Vector2D v = vertices[i];
				  v = bmat * v + g_bpos;
				  v -= g_apos;
				  v = buT * v;

				  // Compute penetration distance (in B's model space)
				  float d = vector2d::dot(n, s - v);

				  // Store greatest distance
				  if (d > penetrationB)
				  {
					  penetrationB = d;
					  faceB = i;
				  }
			  }

			  if (penetrationB >= 0.0f)
			  {
				  return;
			  }

			  // Always point from a to b
			  bool flip;

			  // World space incident face
			  Vector2D incidentFace[2];
			  Vector2D v1;
			  Vector2D v2;

			  // Determine which shape contains reference face
			  if (vector2d::biasGreaterThan(penetrationA, penetrationB))
			  {
				  //        y
				  //        ^  ->n       ^
				  //      +---c ------posPlane--
				  //  x < | i |\
				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  										//      +---+ c-----negPlane--
				  //             \       v
				  //              r
				  //
				  //  r : reference face
				  //  i : incident poly
				  //  c : clipped point
				  //  n : incident normal
				  flip = false;

				  Vector2D referenceNormal = ac->normal(faceA);

				  // Calculate normal in incident's frame of reference
				  // To world space
				  referenceNormal = amat * referenceNormal;
				  // To incident's model space
				  Matrix2D mat;
				  referenceNormal = matrix2d::transpose(mat, bmat) * referenceNormal;

				  // Find most anti-normal face on incident Collider2DPolygon
				  unsigned int incident = 0;
				  float minDot = FLT_MAX;

				  const Vector2D * vertices = bc->vertices();
				  const Vector2D * nomals = bc->normals();
				  unsigned int vertex_count = bc->numOfVerties();
				  for (unsigned int i = 0; i < vertex_count; ++i)
				  {
					  float dot = vector2d::dot(referenceNormal, nomals[i]);

					  if (dot < minDot)
					  {
						  minDot = dot;
						  incident = i;
					  }
				  }

				  // Assign face vertices for incidentFace
				  incidentFace[0] = bmat * vertices[incident] + g_bpos;
				  incident = (incident + 1) % vertex_count;
				  incidentFace[1] = bmat * vertices[incident] + g_bpos;

				  // Setup reference face vertices
				  v1 = amat * ac->vertex(faceA) + g_apos;
				  faceA = (faceA + 1) % ac->numOfVerties();
				  v2 = amat * ac->vertex(faceA) + g_apos;
			  }
			  else
			  {
				  //        y
				  //        ^  ->n       ^
				  //      +---c ------posPlane--
				  //  x < | i |\
				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  				  		//      +---+ c-----negPlane--
				  //             \       v
				  //              r
				  //
				  //  r : reference face
				  //  i : incident poly
				  //  c : clipped point
				  //  n : incident normal
				  flip = true;

				  Vector2D referenceNormal = bc->normal(faceB);

				  // Calculate normal in incident's frame of reference
				  // To world space
				  referenceNormal = bmat * referenceNormal;
				  // To incident's model space
				  Matrix2D mat;
				  referenceNormal = matrix2d::transpose(mat, amat) * referenceNormal;

				  // Find most anti-normal face on incident Collider2DPolygon
				  unsigned int incident = 0;
				  float minDot = FLT_MAX;

				  const Vector2D * vertices = ac->vertices();
				  const Vector2D * normals = ac->normals();
				  unsigned int vertex_count = ac->numOfVerties();
				  for (unsigned int i = 0; i < vertex_count; ++i)
				  {
					  float dot = vector2d::dot(referenceNormal, normals[i]);

					  if (dot < minDot)
					  {
						  minDot = dot;
						  incident = i;
					  }
				  }

				  // Assign face vertices for incidentFace
				  incidentFace[0] = amat * vertices[incident] + g_apos;
				  incident = (incident + 1) % vertex_count;
				  incidentFace[1] = amat * vertices[incident] + g_apos;

				  // Setup reference face vertices
				  v1 = amat * bc->vertex(faceB) + g_bpos;
				  faceB = (faceB + 1) % bc->numOfVerties();
				  v2 = amat * bc->vertex(faceB) + g_bpos;
			  }

			  // Calculate reference face side normal in world space
			  Vector2D sidePlaneNormal = (v2 - v1);
			  vector2d::normalize(sidePlaneNormal, sidePlaneNormal);

			  // Orthogonalize
			  Vector2D refFaceNormal(
				  sidePlaneNormal.y,
				  -sidePlaneNormal.x);

			  // ax + by = c
			  // c is distance from origin
			  float refC = vector2d::dot(refFaceNormal, v1);
			  float negSide = -vector2d::dot(sidePlaneNormal, v1);
			  float posSide = vector2d::dot(sidePlaneNormal, v2);

			  // Clip incident face to reference face side planes
			  if (clip(incidentFace, -sidePlaneNormal, negSide) < 2)
			  {
				  // Due to floating point error, possible to not have required points
				  return;
			  }

			  if (clip(incidentFace, sidePlaneNormal, posSide) < 2)
			  {
				  // Due to floating point error, possible to not have required points
				  return;
			  }

			  // Flip
			  m.normal = flip ? -refFaceNormal : refFaceNormal;

			  // Keep points behind reference face
			  unsigned int cp = 0; // clipped points behind reference face
			  float separation = vector2d::dot(refFaceNormal, incidentFace[0]) - refC;
			  if (separation <= 0.0f)
			  {
				  m.contacts[cp] = incidentFace[0];
				  m.penetration = -separation;
				  ++cp;
			  }
			  else
			  {
				  m.penetration = 0;
			  }

			  separation = vector2d::dot(refFaceNormal, incidentFace[1]) - refC;
			  if (separation <= 0.0f)
			  {
				  m.contacts[cp] = incidentFace[1];

				  m.penetration += -separation;
				  ++cp;

				  // Average penetration
				  m.penetration /= (float)cp;
			  }

			  m.contact_count = cp;
	}
		return;
	}
}

//-------------------------------------------------------------------------------------- 
CLASS_REALIZE_FUNC(Physics2DEngine, update)(
	/* [ref] */ SOURCE_DESC * (&_source),
	/* [r] */ unsigned int _size)
{
	if (_size < 2)
	{
		return E_INVALIDARG;
	}

	std::vector<My_SOURCE_DESC> & res = RESOURCES[GetCurrentThreadId()];

	// 클리어를 호출해야 내부에서 My_SOURCE_DESC 소멸자가 호출됨
	res.clear();

	res.resize(_size);
	for (unsigned int index = 0; index < _size; ++index)
	{
		res[index] = _source[index];
	}

	return S_OK;
}

//-------------------------------------------------------------------------------------- 
CLASS_REALIZE_FUNC_T(Physics2DEngine, void, run)(
	/* [r] */ const i::i_Terrain2D * _terrain,
	/* [r] */ const Vector2D & _gravity,
	/* [r] */ float _dt,
	/* [r] */ void * _param)
{
	// 충돌 결과
	RESULT_CONTACT result;

	std::vector<My_SOURCE_DESC> & res =
		RESOURCES[GetCurrentThreadId()];

	// 물리 오브젝트.
	std::vector<My_SOURCE_DESC>::iterator begin = res.begin();
	std::vector<My_SOURCE_DESC>::iterator end = res.end();

	// 충돌 검사.
	for (std::vector<My_SOURCE_DESC>::iterator A = begin; A != end; ++A)
	{
		for (std::vector<My_SOURCE_DESC>::iterator B = A; B != end; ++B)
		{

			// 충돌
			R_Manifold m{ A, B };
			operate(A->object, B->object, _terrain, &m.m);

			if (m.m.contact_count)
			{
				result.push_back(m);
			}
		}
	}

	// 
	for (std::vector<My_SOURCE_DESC>::iterator A = begin; A != end; ++A)
	{
		if (A->object)
		{
			A->object->integrate_Forces(_gravity, 0.0f, _dt);
		}
	}

	// 
	RESULT_CONTACT::iterator r_begin = result.begin();
	RESULT_CONTACT::iterator r_end = result.end();

	//
	for (RESULT_CONTACT::iterator A = r_begin; A != r_end; ++A)
	{
		i::i_Physics2DObject * obj_A = A->a->object;
		i::i_Physics2DObject * obj_B = A->b->object;

		i::i_RigidBody2D * rig_A = obj_A->rigidBody();
		i::i_RigidBody2D * rig_B = obj_B->rigidBody();

		IF_FALSE(rig_A && rig_B)
		{
			A = result.erase(A);
			continue;
		}

		// Initialize collision
		manifold::initialize(
			A->m,
			rig_A,
			rig_B);

		// Solve collisions
		manifold::impulse_Apply(
			A->m,
			obj_A->position(),
			rig_A,
			obj_B->position(),
			rig_B,
			vector2d::len(_gravity));
	}

	// 
	for (std::vector<My_SOURCE_DESC>::iterator A = begin; A != end; ++A)
	{
		i::i_Physics2DObject * obj_A;
		if (obj_A = A->object)
		{
			obj_A->integrate_Velocity(_gravity, _dt);

			i::i_RigidBody2D * rig_A;
			if (rig_A = obj_A->rigidBody())
			{
				rig_A->expended();
			}
		}
	}

	// Correct positions
	for (RESULT_CONTACT::iterator A = r_begin; A != r_end; ++A)
	{
		i::i_Physics2DObject * obj_A = A->a->object;
		i::i_Physics2DObject * obj_B = A->b->object;

		const float k_slop = 0.05f; // Penetration allowance
		const float percent = 0.4f; // Penetration percentage to correct

		float amass = obj_A->rigidBody()->mass();
		float bmass = obj_B->rigidBody()->mass();
		float mass = amass + bmass;
		IF_FALSE(0 < mass)
		{
			continue;
		}

		Manifold2D & m = A->m;
		float alpha = max(m.penetration - k_slop, 0.0f) / mass;
		Vector2D correction = m.normal * percent * alpha;

		obj_A->move(-correction * amass);
		obj_B->move(correction * bmass);
	}

	//// 지면 충돌 검사
	//if (_terrain)
	//{
	//	for (std::vector<My_SOURCE_DESC>::iterator A = begin; A != end; ++A)
	//	{
	//		i::i_Physics2DObject * obj_A;
	//		IF_FALSE(obj_A = A->object)
	//		{
	//			continue;
	//		}

	//		const Vector2D pos = obj_A->position();

	//		float dy = -FLT_MAX;
	//		const float yy = _terrain->height(pos.y, &dy);

	//		// pos.y 의 위치가 지면보다 높음.
	//		if (yy < pos.y)
	//		{
	//			continue;
	//		}

	//		// 미구현
	//	}
	//}
}