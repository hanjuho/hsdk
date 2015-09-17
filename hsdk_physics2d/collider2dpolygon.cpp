#include <physics2d/collider2dpolygon.h>
#include <physics2d/vector2d.h>
#include <assert.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Collider2DPolygon, Collider2DPolygon)(
	/* [in] */ float _hw,
	/* [in] */ float _hh,
	/* [in] */ float _density)
	: my_Density(_density)
{
	set_Box(_hw, _hh);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Collider2DPolygon, Collider2DPolygon)(
	/* [in] */ const Vector2D * _vertices,
	/* [in] */ unsigned int _size,
	/* [in] */ float _density)
	: my_Density(_density)
{
	set_Polygon(_vertices, _size);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Collider2DPolygon, void, set_Box)(
	/* [in] */ float _hw,
	/* [in] */ float _hh)
{
	my_Vertices.resize(4);
	my_Normals.resize(4);

	float hw, hh;
	hw = _hw * 0.5f;
	hh = _hh *0.5f;

	my_Vertices[0] = Vector2D(-hw, -hh);
	my_Vertices[1] = Vector2D(hw, -hh);
	my_Vertices[2] = Vector2D(hw, hh);
	my_Vertices[3] = Vector2D(-hw, hh);

	my_Normals[0] = Vector2D(0.0f, -1.0f);
	my_Normals[1] = Vector2D(1.0f, 0.0f);
	my_Normals[2] = Vector2D(0.0f, 1.0f);
	my_Normals[3] = Vector2D(-1.0f, 0.0f);
	
	my_Radius = vector2d::len(Vector2D(hw, hh)) * 0.5f;
	my_Area = std::abs(_hw * _hh);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Collider2DPolygon, void, set_Polygon)(
	/* [in] */ const Vector2D * _vertices,
	/* [in] */ unsigned int _size)
{
	// No hulls with less than 3 vertices (ensure actual Collider2DPolygon)
	assert(_size > 2);

	// Find the right most point on the hull
	unsigned int rightMost = 0;
	float highestXCoord = _vertices[0].x;
	for (unsigned int i = 1; i < _size; ++i)
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

	std::vector<unsigned int> hull(_size);
	unsigned int outCount = 0;
	unsigned int indexHull = rightMost;

	for (;;)
	{
		hull[outCount] = indexHull;

		// Search for next index that wraps around the hull
		// by computing cross products to find the most counter-clockwise
		// vertex in the set, given the previos hull index
		unsigned int nextHullIndex = 0;
		for (unsigned int i = 1; i < _size; ++i)
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

	my_Vertices.resize(outCount);
	my_Normals.resize(outCount);
	my_Radius = 0.0f;

	// Copy vertices into shape's vertices
	for (unsigned int i = 0; i < outCount; ++i)
	{
		my_Vertices[i] = _vertices[hull[i]];
	}

	// Compute face normals
	for (unsigned int i1 = 0; i1 < outCount; ++i1)
	{
		unsigned int i2 = (i1 + 1) % outCount;
		Vector2D face = my_Vertices[i2] - my_Vertices[i1];

		// Ensure no zero-length edges, because that's bad
		assert(vector2d::lenSqr(face) > EPSILON * EPSILON);

		// Calculate normal with 2D cross product between vector and scalar
		vector2d::normalize(my_Normals[i1], Vector2D(face.y, -face.x));

		//
		my_Radius = std::max(my_Radius, vector2d::len(my_Vertices[rightMost] - my_Vertices[i1]));
	}

	my_Radius *= 0.5f;
	my_Area = compute_Polygon();
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Collider2DPolygon, Vector2D, support)(
	/* [in] */ const Vector2D & _dir)const
{
	float bestProjection = -FLT_MAX;
	const Vector2D * bestVertex = NULL;

	for (unsigned int i = 0; i < my_Vertices.size(); ++i)
	{
		const Vector2D & v = my_Vertices[i];
		float projection = vector2d::dot(v, _dir);

		if (projection > bestProjection)
		{
			bestVertex = &v;
			bestProjection = projection;
		}
	}

	return *bestVertex;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Collider2DPolygon, unsigned int, type)(
	/* [none] */ void)const
{
	return 1;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Collider2DPolygon, unsigned int, numOfVerties)(
	/* [none] */ void)const
{
	return my_Vertices.size();
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Collider2DPolygon, Vector2D, vertex)(
	/* [in] */ unsigned int _index)const
{
	return my_Vertices[_index];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Collider2DPolygon, Vector2D, normal)(
	/* [in] */ unsigned int _index)const
{
	return my_Normals[_index];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Collider2DPolygon, float, density)(
	/* [none] */ void)const
{
	return my_Density;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Collider2DPolygon, float, area)(
	/* [none] */ void)const
{
	return my_Area;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Collider2DPolygon, float, radius)(
	/* [none] */ void)const
{
	return my_Radius;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Collider2DPolygon, float, compute_Polygon)(
	/* [none] */ void)
{
	// Calculate centroid and moment of interia
	Vector2D c(0.0f, 0.0f); // centroid
	float area = 0.0f;
	float I = 0.0f;
	const float k_inv3 = 1.0f / 3.0f;

	unsigned int size = my_Vertices.size();
	for (unsigned int i1 = 0; i1 < size; ++i1)
	{
		// Triangle vertices, third vertex implied as (0, 0)
		Vector2D p1(my_Vertices[i1]);
		unsigned int i2 = i1 + 1 < size ? i1 + 1 : 0;
		Vector2D p2(my_Vertices[i2]);

		float D = vector2d::cross(p1, p2);
		float triangleArea = 0.5f * D;

		area += triangleArea;

		// Use area to weight the centroid average, not just vertex position
		c += (p1 + p2) * triangleArea * k_inv3;

		float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
		float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
		I += (0.25f * k_inv3 * D) * (intx2 + inty2);
	}

	c *= 1.0f / area;

	// Translate vertices to centroid (make the centroid (0, 0)
	// for the polygon in model space)
	// Not really necessary, but I like doing this anyway
	// for (unsigned int i = 0; i < m_vertexCount; ++i)
	// 	m_vertices[i] -= c;

	return area;
}