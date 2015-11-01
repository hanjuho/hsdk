#include <hsdk/win/direct3d/d3d10_terrain.h>
#include <fstream>


//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, hsdk::direct3d::terrain::build_Terrain)(
	_Out_ D3D10_Terrain & _terrain,
	_In_ unsigned long _width,
	_In_ unsigned long _height,
	_In_ unsigned long _row,
	_In_ unsigned long _column)
{
	_terrain.verticsPerRow = _row;
	_terrain.verticsPerCol = _column;
	_terrain.vertices = _terrain.verticsPerRow * _terrain.verticsPerCol;
	_terrain.cellsPerRow = _row - 1;
	_terrain.cellsPerCol = _column - 1;
	_terrain.triangles = _terrain.cellsPerRow * _terrain.cellsPerCol * 2;
	_terrain.xCellSpacing = double(_width) / double(_terrain.cellsPerRow);
	_terrain.zCellSpacing = double(_height) / double(_terrain.cellsPerCol);
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(hsdk::direct3d::terrain::load_RawFromFile)(
	_Out_ float * _buffer,
	_In_ const wchar_t * _fileName,
	_In_ unsigned int _size)
{
	// A height for each vertex
	std::vector<BYTE> in(_size);

	std::ifstream inFile(_fileName, std::ios_base::binary);
	IF_FALSE(inFile)
	{
		return ADD_FLAG(HSDK_FAIL, GetLastError());
	}

	inFile.read(
		(LPSTR)&in[0], // buffer
		in.size());// number of bytes to read into buffer

	inFile.close();

	// 높이값을 채워넣음
	unsigned int size = min(_size, in.size());
	for (unsigned long index = 0; index < size; ++index)
	{
		_buffer[index] = (float)in[index];
	}

	return S_OK;
}