#include <physics2d/terrain2d.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Terrain2D, Terrain2D)(
	/* [in] */ float _posX,
	/* [in] */ float _minY,
	/* [in] */ unsigned int _width,
	/* [in] */ unsigned int _numOfCells)
	: posX(0.0f),
	minY(_minY),
	width(_width),
	numOfCells(_numOfCells),
	cellSpace((float)(_width) / (float)(_numOfCells)),
	my_HeightMap(_numOfCells + 1)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Terrain2D, apply_Raw_File)(
	/* [in] */ const wchar_t * _fileName,
	/* [in] */ float _intensity)
{
	return HSDK_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Terrain2D, void, apply_Raw_User)(
	/* [in] */ std::vector<float> & _map)
{
	unsigned int max = std::min(_map.size(), my_HeightMap.size());
	for (unsigned int index = 0; index < max; ++index)
	{
		my_HeightMap[index] = _map[index];
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Terrain2D, unsigned int, index_Cell)(
	/* [in] */ float _x)const
{
	return (unsigned int)(::floorf((_x - posX) / cellSpace));
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Terrain2D, float, get_Height)(
	/* [in] */ unsigned int _index)const
{
	if (_index < my_HeightMap.size())
	{
		return my_HeightMap[_index];
	}
	else
	{
		return 0xffffffff;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Terrain2D, float, compute_Height)(
	/* [in] */ float _x,
	/* [in] */ float * _dy)const
{
	float dx = _x - posX;
	float cx = dx / cellSpace;
	float cf = ::floorf(cx);

	long index = (long)(cf);
	float ay = get_Height(index);
	float by = get_Height(index + 1);

	float dy = (by - ay);
	float h = ay + (dy * (cx - cf));

	if (_dy)
	{
		(*_dy) = dy;
	}

	return minY < h ? h : 0xffffffff;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Terrain2D, void, set_Height)(
	/* [in] */ unsigned int _index,
	/* [in] */ float _height)
{
	my_HeightMap[_index] = _height;
}