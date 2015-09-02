#include <physics2d/polygon.h>
#include <physics2d/rigidbody.h>
#include <assert.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Polygon, Polygon)(
	/* [in] */ float _hw,
	/* [in] */ float _hh,
	/* [in] */ bool _adjustment)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Polygon, Polygon)(
	/* [in] */ const Vector2D * _vertices,
	/* [in] */ unsigned int _count)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Polygon, i::i_Collider::COLLIDER_TYPE, get_Type)(
	/* [none] */ void)const
{
	return i::i_Collider::COLLIDER_POLYGON;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Polygon, void, set_Orient)(
	/* [in] */ float _radians)
{
	matrix2d::rotate(matrix, _radians);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Polygon, void, apply_Mass)(
	/* [in] */ float _density)
{
	// Calculate centroid and moment of interia
	Vector2D c(0.0f, 0.0f); // centroid	

	area = 0.0f;
	inertia = 0.0f;
	const float k_inv3 = 1.0f / 3.0f;

	for (unsigned int i1 = vertices.size() - 1; i1 != -1; --i1)
	{
		// Triangle vertices, third vertex implied as (0, 0)
		Vector2D & p1 = vertices[i1];
		unsigned int i2 = i1 + 1 < vertices.size() ? i1 + 1 : 0;
		Vector2D & p2 = vertices[i2];

		float D = p1.x * p2.y - p1.y * p2.x;
		float triangleArea = 0.5f * D;

		area += triangleArea;

		// Use area to weight the centroid average, not just vertex position
		c += p1 * triangleArea * k_inv3 + p2;


		float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
		float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
		inertia += (0.25f * k_inv3 * D) * (intx2 + inty2);
	}

	c *= 1.0f / area;

	// Translate vertices to centroid (make the centroid (0, 0)
	// for the polygon in model space)
	// Not really necessary, but I like doing this anyway
	// XMVECTOR store;
	// for (unsigned int i1 = vertices.size() - 1; i1 != -1; --i1)
	// {
	// 	store = XMLoadFloat2(&vertices[i1]);
	// 	store -= c;
	// 	XMStoreFloat2(&vertices[i1], store);
	// }

	density = _density;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Polygon, void, apply_Body)(
	/* [out] */ RigidBody * _body,
	/* [in] */ bool _inverse)
{
	if (_inverse)
	{
		_body->m -= density * area;
		_body->im -= (_body->m) ? 1.0f / _body->m : 0.0f;
		_body->I -= inertia * density;
		_body->iI -= _body->I ? 1.0f / _body->I : 0.0f;
	}
	else
	{
		_body->m += density * area;
		_body->im += (_body->m) ? 1.0f / _body->m : 0.0f;
		_body->I += inertia * density;
		_body->iI += _body->I ? 1.0f / _body->I : 0.0f;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Polygon, i::i_Collider *, clone)(
	/* [none] */ void)const
{
	return new Polygon(&vertices[0], vertices.size());
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Polygon, void, set_Box)(
	/* [in] */ float _hw,
	/* [in] */ float _hh,
	/* [in] */ bool _adjustment)
{
	vertices.resize(4);
	normals.resize(4);

	if (_adjustment)
	{
		float hw;
		hw = _hw * 0.5f;

		vertices[0] = Vector2D(-hw, 0.0f);
		vertices[1] = Vector2D(hw, 0.0f);
		vertices[2] = Vector2D(hw, _hh);
		vertices[3] = Vector2D(-hw, _hh);
	}
	else
	{
		float hw, hh;
		hw = _hw * 0.5f;
		hh = _hh *0.5f;

		vertices[0] = Vector2D(-hw, -hh);
		vertices[1] = Vector2D(hw, -hh);
		vertices[2] = Vector2D(hw, hh);
		vertices[3] = Vector2D(-hw, hh);
	}

	normals[0] = Vector2D(0.0f, -1.0f);
	normals[1] = Vector2D(1.0f, 0.0f);
	normals[2] = Vector2D(0.0f, 1.0f);
	normals[3] = Vector2D(-1.0f, 0.0f);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Polygon, void, set_Polygon)(
	/* [in] */ const Vector2D * _vertices,
	/* [in] */ unsigned int _count)
{
	// No hulls with less than 3 vertices (ensure actual polygon)
	assert(_count > 2);

	// Find the right most point on the hull
	unsigned int rightMost = 0;
	float highestXCoord = _vertices[0].x;
	for (unsigned int i = 1; i < _count; ++i)
	{
		float x = _vertices[i].x;
		if (x > highestXCoord)
		{
			highestXCoord = x;
			rightMost = i;
		}

		// If matching x then take farthest negative y
		else if (x == highestXCoord)
		{
			if (_vertices[i].y < _vertices[rightMost].y)
			{
				rightMost = i;
			}
		}
	}

	std::vector<unsigned int> hull(_count);
	unsigned int outCount = 0;
	unsigned int indexHull = rightMost;

	for (;;)
	{
		hull[outCount] = indexHull;

		// Search for next index that wraps around the hull
		// by computing cross products to find the most counter-clockwise
		// vertex in the set, given the previos hull index
		unsigned int nextHullIndex = 0;
		for (unsigned int i = 1; i < _count; ++i)
		{
			// Skip if same coordinate as we need three unique
			// points in the set to perform a cross product
			if (nextHullIndex == indexHull)
			{
				nextHullIndex = i;
				continue;
			}

			// Cross every set of three unique vertices
			// Record each counter clockwise third vertex and add
			// to the output hull
			// See : http://www.oocities.org/pcgpe/math2d.html
			Vector2D e1 = _vertices[nextHullIndex] - _vertices[hull[outCount]];
			Vector2D e2 = _vertices[i] - _vertices[hull[outCount]];
			float c = vector2d::cross(e1, e2);
			if (c < 0.0f)
			{
				nextHullIndex = i;
			}

			// Cross product is zero then e vectors are on same line
			// therefor want to record vertex farthest along that line
			if (c == 0.0f && vector2d::lenSqr(e2) > vector2d::lenSqr(e1))
			{
				nextHullIndex = i;
			}
		}

		++outCount;
		indexHull = nextHullIndex;

		// Conclude algorithm upon wrap-around
		if (nextHullIndex == rightMost)
		{
			break;
		}
	}

	vertices.resize(outCount);
	normals.resize(outCount);

	// Copy vertices into shape's vertices
	for (unsigned int i = 0; i < outCount; ++i)
	{
		vertices[i] = _vertices[hull[i]];
	}

	// Compute face normals
	for (unsigned int i1 = 0; i1 < outCount; ++i1)
	{
		unsigned int i2 = (i1 + 1) % outCount;
		Vector2D face = vertices[i2] - vertices[i1];

		// Ensure no zero-length edges, because that's bad
		assert(vector2d::lenSqr(face) > EPSILON * EPSILON);

		// Calculate normal with 2D cross product between vector and scalar
		vector2d::normalize(normals[i1], Vector2D(face.y, -face.x));
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Polygon, Vector2D, get_Support)(
	/* [in] */ const Vector2D & _dir)const
{
	float bestProjection = -FLT_MAX;
	const Vector2D * bestVertex = NULL;

	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		const Vector2D & v = vertices[i];
		float projection = vector2d::dot(v, _dir);

		if (projection > bestProjection)
		{
			bestVertex = &v;
			bestProjection = projection;
		}
	}

	return *bestVertex;
}