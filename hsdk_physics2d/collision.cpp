#include <physics2d/collision.h>
#include <physics2d/collider2dcircle.h>
#include <physics2d/Collider2dpolygon.h>
#include <physics2d/manifold2d.h>
#include <physics2d/matrix2d.h>
#include <assert.h>



using namespace hsdk;
using namespace physics2d;
using namespace collision;


//--------------------------------------------------------------------------------------
CollisionCallback hsdk::physics2d::collision::COLLISION_DISPATCH
[COLLIDER_COUNT][COLLIDER_COUNT] =
{
	{
		hsdk::physics2d::collision::CircleToCircle, hsdk::physics2d::collision::CircleToPolygon
	},
	{
		hsdk::physics2d::collision::PolygonToCircle, hsdk::physics2d::collision::PolygonToPolygon
	}
};

//--------------------------------------------------------------------------------------
DLL_DECL_FUNC_T(void, hsdk::physics2d::collision::CircleToCircle)(
	/* [out] */ Manifold2D & _m,
	/* [in] */ const i::i_Physics2DObject * _aobj,
	/* [in] */ const i::i_Physics2DObject * _bobj,
	/* [in] */ const i::i_Collider2D * _acollider,
	/* [in] */ const i::i_Collider2D * _bcollider)
{
	const Collider2DCircle * ac = reinterpret_cast<const Collider2DCircle *>(_acollider);
	const Collider2DCircle * bc = reinterpret_cast<const Collider2DCircle *>(_bcollider);

	// Calculate translational vector, which is normal
	const Vector2D apos = _aobj->position();
	const Vector2D dir = _bobj->position() - apos;

	float dist_sqr = vector2d::lenSqr(dir);
	float radius = ac->radius + bc->radius;

	// Not in contact
	if (dist_sqr >= radius * radius)
	{
		_m.contact_count = 0;
		return;
	}

	float distance = std::sqrt(dist_sqr);
	_m.contact_count = 1;

	if (distance == 0.0f)
	{
		_m.penetration = ac->radius;
		_m.normal = Vector2D(1.0f, 0.0f);
		_m.contacts[0] = apos;
	}
	else
	{
		_m.penetration = radius - distance;
		// Faster than using Normalized since we already performed sqrt
		_m.normal = dir / distance;
		_m.contacts[0] = (_m.normal * ac->radius) + apos;
	}
}

//--------------------------------------------------------------------------------------
DLL_DECL_FUNC_T(void, hsdk::physics2d::collision::CircleToPolygon)(
	/* [out] */ Manifold2D & _m,
	/* [in] */ const i::i_Physics2DObject * _aobj,
	/* [in] */ const i::i_Physics2DObject * _bobj,
	/* [in] */ const i::i_Collider2D * _acollider,
	/* [in] */ const i::i_Collider2D * _bcollider)
{
	const Collider2DCircle * ac = reinterpret_cast<const Collider2DCircle *>(_acollider);
	const Collider2DPolygon * bc = reinterpret_cast<const Collider2DPolygon *>(_bcollider);
	_m.contact_count = 0;

	// Transform Collider2DCircle center to Collider2DPolygon model space
	const Matrix2D mat = _bobj->matrix();
	const Vector2D apos = _aobj->position();
	const Vector2D bpos = _bobj->position();

	Matrix2D brmat;
	const Vector2D center = matrix2d::transpose(brmat, mat) * (bpos - apos);

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

		if (s > ac->radius)
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
		_m.contact_count = 1;
		_m.normal = -(mat * normals[faceNormal]);
		_m.contacts[0] = _m.normal * ac->radius + apos;
		_m.penetration = ac->radius();
		return;
	}

	// Determine which voronoi region of the edge center of Collider2DCircle lies within
	float dot1 = vector2d::dot(center - v1, v2 - v1);
	float dot2 = vector2d::dot(center - v2, v1 - v2);
	_m.penetration = ac->radius - separation;

	// Closest to v1
	if (dot1 <= 0.0f)
	{
		if (vector2d::distSqr(center, v1) > (ac->radius * ac->radius))
		{
			return;
		}

		_m.contact_count = 1;

		// dir = 원의 중심 - 사각형의 중심.
		Vector2D dir = v1 - center;

		// dir에 사각형의 매트릭스 적용, 노말 계산.
		vector2d::normalize(_m.normal, mat * dir);

		// 접점.
		_m.contacts[0] = mat * v1 + bpos;
	}

	// Closest to v2
	else if (dot2 <= 0.0f)
	{
		if (vector2d::distSqr(center, v2) > (ac->radius * ac->radius))
		{
			return;
		}

		_m.contact_count = 1;

		//
		Vector2D dir = v2 - center;

		//
		v2 = mat * v2 + bpos;

		//
		_m.contacts[0] = v2;

		//
		vector2d::normalize(_m.normal, mat * dir);
	}

	// Closest to face
	else
	{
		Vector2D n = bc->normals[faceNormal];
		if (vector2d::dot(center - v1, n) > ac->radius)
		{
			return;
		}

		//
		n = mat * n;

		//
		_m.normal = -n;

		//
		_m.contacts[0] = _m.normal * ac->radius() + apos;

		//
		_m.contact_count = 1;
	}
}

//--------------------------------------------------------------------------------------
DLL_DECL_FUNC_T(void, hsdk::physics2d::collision::PolygonToCircle)(
	/* [out] */ Manifold2D & _m,
	/* [in] */ const i::i_Physics2DObject * _aobj,
	/* [in] */ const i::i_Physics2DObject * _bobj,
	/* [in] */ const i::i_Collider2D * _acollider,
	/* [in] */ const i::i_Collider2D * _bcollider)
{
	hsdk::physics2d::collision::CircleToPolygon(_m, _aobj, _bobj, _bcollider, _acollider);
	_m.normal = -_m.normal;
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC_T(unsigned int, clip)(
	/* [out] */ Vector2D(&_in_out)[2],
	/* [in] */ const Vector2D & _n,
	/* [in] */ float _c)
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

	if (d2 <= 0.0f)
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

	assert(sp != 3);

	return sp;
}

//--------------------------------------------------------------------------------------
DLL_DECL_FUNC_T(void, hsdk::physics2d::collision::PolygonToPolygon)(
	/* [out] */ Manifold2D & _m,
	/* [in] */ const i::i_Physics2DObject * _aobj,
	/* [in] */ const i::i_Physics2DObject * _bobj,
	/* [in] */ const i::i_Collider2D * _acollider,
	/* [in] */ const i::i_Collider2D * _bcollider)
{
	const Collider2DPolygon * ac = reinterpret_cast<const Collider2DPolygon *>(_acollider);
	const Collider2DPolygon * bc = reinterpret_cast<const Collider2DPolygon *>(_bcollider);
	_m.contact_count = 0;

	const Vector2D apos = _aobj->position();
	const Vector2D bpos = _bobj->position();

	const Matrix2D amat = _aobj->matrix();
	const Matrix2D bmat = _bobj->matrix();

	// Check for a separating axis with A's face planes
	float penetrationA;
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
		v = amat * v + apos;
		v -= bpos;
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
	float penetrationB;
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
		v = bmat * v + bpos;
		v -= apos;
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

	unsigned int reference_index;
	bool flip; // Always point from a to b

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
		incidentFace[0] = bmat * vertices[incident] + bpos;
		incident = (incident + 1) % vertex_count;
		incidentFace[1] = bmat * vertices[incident] + bpos;

		// Setup reference face vertices
		v1 = amat * ac->vertex(faceA) + apos;
		faceA = (faceA + 1) % ac->numOfVerties();
		v2 = amat * ac->vertex(faceA) + apos;
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
		incidentFace[0] = amat * vertices[incident] + apos;
		incident = (incident + 1) % vertex_count;
		incidentFace[1] = amat * vertices[incident] + apos;

		// Setup reference face vertices
		v1 = amat * bc->vertex(faceB) + bpos;
		faceB = (faceB + 1) % bc->numOfVerties();
		v2 = amat * bc->vertex(faceB) + bpos;
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
	_m.normal = flip ? -refFaceNormal : refFaceNormal;

	// Keep points behind reference face
	unsigned int cp = 0; // clipped points behind reference face
	float separation = vector2d::dot(refFaceNormal, incidentFace[0]) - refC;
	if (separation <= 0.0f)
	{
		_m.contacts[cp] = incidentFace[0];
		_m.penetration = -separation;
		++cp;
	}
	else
	{
		_m.penetration = 0;
	}

	separation = vector2d::dot(refFaceNormal, incidentFace[1]) - refC;
	if (separation <= 0.0f)
	{
		_m.contacts[cp] = incidentFace[1];

		_m.penetration += -separation;
		++cp;

		// Average penetration
		_m.penetration /= (float)cp;
	}

	_m.contact_count = cp;
}