#include <physics2d/terrain2d.h>



using namespace hsdk;
using namespace physics2d;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Terrain2D, Terrain2D)(
	_In_ float _posX,
	_In_ float _minY,
	_In_ unsigned int _width,
	_In_ unsigned int _numOfCells)
	: posX(0.0f),
	minY(_minY),
	width(_width),
	numOfCells(_numOfCells),
	cellSpace((float)(_width) / (float)(_numOfCells)),
	my_HeightMap(_numOfCells + 1)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Terrain2D, initialize)(
	_In_ const wchar_t * _fileName,
	_In_ float _intensity)
{
	return HSDK_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Terrain2D, void, initialize)(
	_In_ const float * _map,
	_In_ unsigned int _offset,
	_In_ unsigned int _size)
{
	unsigned int max = my_HeightMap.size();
	max = _size < max ? _size : max;

	for (unsigned int index = 0; index < max; ++index)
	{
		my_HeightMap[index] = _map[index];
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Terrain2D, void, set_Height)(
	_In_ unsigned int _index,
	_In_ float _height)
{
	my_HeightMap[_index] = _height;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Terrain2D, float, get_Height)(
	_In_ unsigned int _index)const
{
	if (_index < my_HeightMap.size())
	{
		return my_HeightMap[_index];
	}
	else
	{
		return -FLT_MAX;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Terrain2D, unsigned int, indexOfCell)(
	_In_ float _x)const
{
	return (unsigned int)(::floorf((_x - posX) / cellSpace));
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Terrain2D, float, height)(
	_In_ float _x,
	_In_ float * _dy)const
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