#include <physics2d/collider2dpolygon.h>
#include <physics2d/vector2d.h>
#include <assert.h>



using namespace hsdk;
using namespace physics2d;


// grobal function
IMPL_FUNC_T(float, compute_Area)(
	_In_ const std::vector<Vector2D> & _vertices,
	_Out_ float * _mostLength)
{
	// Calculate centroid and moment of interia
	// centroid
	// Vector2D c(0.0f, 0.0f); 
	float area = 0.0f;
	float length = 0.0f;

	unsigned int size = _vertices.size();
	for (unsigned int i1 = 0; i1 < size; ++i1)
	{
		// Triangle vertices, third vertex implied as (0, 0)
		Vector2D p1(_vertices[i1]);
		unsigned int i2 = i1 + 1 < size ? i1 + 1 : 0;
		Vector2D p2(_vertices[i2]);

		float L = vector2d::len(p1);
		length = length < L ? L : length;

		float D = vector2d::cross(p1, p2);
		float triangleArea = 0.5f * D;

		area += triangleArea;
	}
	
	if (_mostLength)
	{
		(*_mostLength) = length;
	}

	return area;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Collider2DPolygon, Collider2DPolygon)(
	_In_ float _density)
	: m_Density(_density), m_Area(0.0f), m_Radius(0.0f)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DPolygon, unsigned int, type)(
	/* [x] */ void)const
{
	return 1;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DPolygon, unsigned int, numOfVerties)(
	/* [x] */ void)const
{
	return my_Vertices.size();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DPolygon, Vector2D, vertex)(
	_In_ unsigned int _index)const
{
	return my_Vertices[_index];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DPolygon, const Vector2D *, vertices)(
	/* [x] */ void)const
{
	return &my_Vertices[0];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DPolygon, Vector2D, normal)(
	_In_ unsigned int _index)const
{
	return my_Normals[_index];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DPolygon, const Vector2D *, normals)(
	/* [x] */ void)const
{
	return &my_Normals[0];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DPolygon, float, density)(
	/* [x] */ void)const
{
	return m_Density;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DPolygon, float, area)(
	/* [x] */ void)const
{
	return m_Area;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DPolygon, float, radius)(
	/* [x] */ void)const
{
	return m_Radius;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DPolygon, void, set_Box)(
	_In_ float _hw,
	_In_ float _hh)
{
	my_Vertices.resize(4);
	my_Normals.resize(4);

	m_Area = abs(_hw * _hh);

	_hw *= 0.5f;
	_hh *= 0.5f;

	m_Radius = vector2d::len(Vector2D(_hw, _hh));

	my_Vertices[0] = Vector2D(-_hw, -_hh);
	my_Vertices[1] = Vector2D(_hw, -_hh);
	my_Vertices[2] = Vector2D(_hw, _hh);
	my_Vertices[3] = Vector2D(-_hw, _hh);

	my_Normals[0] = Vector2D(0.0f, -1.0f);
	my_Normals[1] = Vector2D(1.0f, 0.0f);
	my_Normals[2] = Vector2D(0.0f, 1.0f);
	my_Normals[3] = Vector2D(-1.0f, 0.0f);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DPolygon, void, set_Polygon)(
	_In_ const Vector2D * _vertices,
	_In_ unsigned int _size)
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
	}

	m_Area = compute_Area(my_Vertices, &m_Radius);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Collider2DPolygon, Vector2D, support)(
	_In_ const Vector2D & _dir)const
{
	float bestProjection = -FLT_MAX;
	const Vector2D * bestVertex = nullptr;

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