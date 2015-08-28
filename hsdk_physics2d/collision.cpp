#include <physics2d/collision.h>
#include <physics2d/circle.h>
#include <physics2d/polygon.h>
#include <physics2d/manifold.h>
#include <assert.h>



using namespace hsdk;
using namespace physics2d;
using namespace collision;


//--------------------------------------------------------------------------------------
CollisionCallback hsdk::physics2d::collision::Collider_Dispatch
[i::i_Collider::COLLIDER_COUNT][i::i_Collider::COLLIDER_COUNT] =
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
	/* [in] */ const Vector2D & _aPos,
	/* [in] */ const Vector2D & _bPos,
	/* [in] */ const i::i_Collider * _ac,
	/* [in] */ const i::i_Collider * _bc,
	/* [out] */ Manifold & _m)
{
	const Circle * ac = reinterpret_cast<const Circle *>(_ac);
	const Circle * bc = reinterpret_cast<const Circle *>(_bc);

	// Calculate translational vector, which is normal
	Vector2D avec = _aPos + ac->position;
	Vector2D bvec = _bPos + bc->position;
	Vector2D dir = bvec - avec;

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
		_m.contacts[0] = avec;
	}
	else
	{
		_m.penetration = radius - distance;
		// Faster than using Normalized since we already performed sqrt
		_m.normal = dir / distance;
		_m.contacts[0] = (_m.normal * ac->radius) + avec;
	}
}

//--------------------------------------------------------------------------------------
DLL_DECL_FUNC_T(void, hsdk::physics2d::collision::CircleToPolygon)(
	/* [in] */ const Vector2D & _aPos,
	/* [in] */ const Vector2D & _bPos,
	/* [in] */ const i::i_Collider * _ac,
	/* [in] */ const i::i_Collider * _bc,
	/* [out] */ Manifold & _m)
{
	const Circle * ac = reinterpret_cast<const Circle *>(_ac);
	const Polygon * bc = reinterpret_cast<const Polygon *>(_bc);

	_m.contact_count = 0;

	// Transform circle center to Polygon model space
	Matrix2D mat;

	Vector2D apos = (_aPos + ac->position);
	Vector2D center = matrix2d::transpose(bc->matrix, mat) * (_bPos - apos);

	// Find edge with minimum penetration
	// Exact concept as using support points in Polygon vs Polygon
	float separation = -FLT_MAX;
	unsigned int faceNormal = 0;
	unsigned int vetex_count = bc->vertices.size();
	for (unsigned int i = 0; i < vetex_count; ++i)
	{
		float s = vector2d::dot(
			bc->normals[i],
			center - bc->vertices[i]);

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
	Vector2D v1 = bc->vertices[faceNormal];
	unsigned int i2 = faceNormal + 1;
	i2 = i2 < vetex_count ? i2 : 0;
	Vector2D v2 = bc->vertices[i2];

	// Check to see if center is within polygon
	if (separation < EPSILON)
	{
		_m.contact_count = 1;
		_m.normal = -(bc->matrix * bc->normals[faceNormal]);
		_m.contacts[0] = _m.normal * ac->radius + apos;
		_m.penetration = ac->radius;
		return;
	}

	// Determine which voronoi region of the edge center of circle lies within
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
		vector2d::normalize(bc->matrix * dir, _m.normal);

		// 접점.
		_m.contacts[0] = bc->matrix * v1 + _bPos;
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
		v2 = bc->matrix * v2 + _bPos;

		//
		_m.contacts[0] = v2;

		//
		vector2d::normalize(bc->matrix * dir, _m.normal);
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
		n = bc->matrix * n;

		//
		_m.normal = -n;

		//
		_m.contacts[0] = _m.normal * ac->radius + apos;

		//
		_m.contact_count = 1;
	}
}

//--------------------------------------------------------------------------------------
DLL_DECL_FUNC_T(void, hsdk::physics2d::collision::PolygonToCircle)(
	/* [in] */ const Vector2D & _aPos,
	/* [in] */ const Vector2D & _bPos,
	/* [in] */ const i::i_Collider * _ac,
	/* [in] */ const i::i_Collider * _bc,
	/* [out] */ Manifold & _m)
{
	hsdk::physics2d::collision::CircleToPolygon(_aPos, _bPos, _bc, _ac, _m);
	_m.normal = -_m.normal;
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC_T(float, find_Axis_Least_Penetration)(
	/* [in] */ const Vector2D & _aPos,
	/* [in] */ const Vector2D & _bPos,
	/* [in] */ const Polygon * _ac,
	/* [in] */ const Polygon * _bc,
	/* [out] */ unsigned int & _out)
{
	float bestDistance = -FLT_MAX;
	unsigned int bestIndex;
	unsigned int vetext_count = _ac->vertices.size();
	for (unsigned int i = 0; i < vetext_count; ++i)
	{
		// Retrieve a face normal from A
		Vector2D n = _ac->normals[i];
		Vector2D nw = _ac->matrix * n;

		// Transform face normal into B's model space
		Matrix2D buT;
		matrix2d::transpose(_bc->matrix, buT);
		n = buT * nw;

		// Retrieve support point from B along -n
		Vector2D s = _bc->get_Support(-n);

		// Retrieve vertex on face from A, transform into
		// B's model space
		Vector2D v = _ac->vertices[i];
		v = _ac->matrix * v + _aPos;
		v -= _bPos;
		v = buT * v;

		// Compute penetration distance (in B's model space)
		float d = vector2d::dot(n, s - v);

		// Store greatest distance
		if (d > bestDistance)
		{
			bestDistance = d;
			bestIndex = i;
		}
	}

	_out = bestIndex;
	return bestDistance;
}

REALIZE_FUNC_T(void, find_incident_Face)(
	/* [in] */ const Polygon * _refPoly,
	/* [in] */ const Polygon * _incPoly,
	/* [in] */ const Vector2D & _position,
	/* [in] */ unsigned int _reference_index,
	/* [out] */ Vector2D(&_out)[2])
{
	Vector2D referenceNormal = _refPoly->normals[_reference_index];

	// Calculate normal in incident's frame of reference
	// To world space
	referenceNormal = _refPoly->matrix * referenceNormal;
	// To incident's model space
	Matrix2D mat;
	referenceNormal = matrix2d::transpose(_incPoly->matrix, mat) * referenceNormal;

	// Find most anti-normal face on incident polygon
	unsigned int incidentFace = 0;
	float minDot = FLT_MAX;
	unsigned int vertex_count = _incPoly->vertices.size();
	for (unsigned int i = 0; i < vertex_count; ++i)
	{
		float dot = vector2d::dot(referenceNormal, _incPoly->normals[i]);

		if (dot < minDot)
		{
			minDot = dot;
			incidentFace = i;
		}
	}

	// Assign face vertices for incidentFace
	_out[0] = _incPoly->matrix * _incPoly->vertices[incidentFace] + _position;
	incidentFace = (incidentFace + 1) % vertex_count;
	_out[1] = _incPoly->matrix * _incPoly->vertices[incidentFace] + _position;
}

REALIZE_FUNC_T(unsigned int, clip)(
	/* [in] */ const Vector2D & _n,
	/* [in] */ float _c,
	/* [in/out] */ Vector2D(&_in_out)[2])
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
	/* [in] */ const Vector2D & _aPos,
	/* [in] */ const Vector2D & _bPos,
	/* [in] */ const i::i_Collider * _ac,
	/* [in] */ const i::i_Collider * _bc,
	/* [out] */ Manifold & _m)
{
	const Polygon * ac = reinterpret_cast<const Polygon *>(_ac);
	const Polygon * bc = reinterpret_cast<const Polygon *>(_bc);
	_m.contact_count = 0;

	// Check for a separating axis with A's face planes
	unsigned int faceA;
	float penetrationA = find_Axis_Least_Penetration(
		_aPos, 
		_bPos, 
		ac,
		bc, 
		faceA);

	if (penetrationA >= 0.0f)
	{
		return;
	}

	// Check for a separating axis with B's face planes
	unsigned int faceB;
	float penetrationB = find_Axis_Least_Penetration(
		_bPos, 
		_aPos,
		bc,
		ac,
		faceB);

	if (penetrationB >= 0.0f)
	{
		return;
	}

	unsigned int reference_index;
	bool flip; // Always point from a to b

	const Polygon * refPoly; // Reference
	const Polygon * incPoly; // Incident

	const Vector2D * refPos;
	const Vector2D * incPos;

	// Determine which shape contains reference face
	if (vector2d::biasGreaterThan(penetrationA, penetrationB))
	{
		refPoly = ac;
		incPoly = bc;
		reference_index = faceA;
		flip = false;

		refPos = &_aPos;
		incPos = &_bPos;
	}
	else
	{
		refPoly = bc;
		incPoly = ac;
		reference_index = faceB;
		flip = true;

		refPos = &_bPos;
		incPos = &_aPos;
	}

	// World space incident face
	Vector2D incidentFace[2];
	find_incident_Face(
		refPoly, 
		incPoly,
		(*incPos), 
		reference_index, 
		incidentFace);

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

	// Setup reference face vertices
	Vector2D v1 = refPoly->vertices[reference_index];
	reference_index = (reference_index + 1) == refPoly->vertices.size() ? 0 : reference_index + 1;
	Vector2D v2 = refPoly->vertices[reference_index];

	// Transform vertices to world space
	v1 = (refPoly->matrix * v1) + (*refPos);
	v2 = (refPoly->matrix * v2) + (*refPos);

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
	if (clip(-sidePlaneNormal, negSide, incidentFace) < 2)
	{
		// Due to floating point error, possible to not have required points
		return;
	}

	if (clip(sidePlaneNormal, posSide, incidentFace) < 2)
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