#include <hsdk/win/direct3d/d3d10_font.h>
#include <hsdk/win/framework.h>
#include <fstream>



using namespace hsdk::direct3d;


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Font, initialize)(
	_In_ const char * _fontDirecoty,
	_In_ const wchar_t * _fontTableDirecoty)
{
	HRESULT hr;

	AutoRelease<ID3D10ShaderResourceView> texture;
	IF_FAILED(hr = D3DX10CreateShaderResourceViewFromFile(
		framework::g_Framework_Device.d3d10Device,
		_fontTableDirecoty,
		nullptr,
		nullptr,
		&texture,
		nullptr))
	{
		return hr;
	}

	std::ifstream fin;

	// Read in the font size and spacing between chars.
	fin.open(_fontDirecoty);
	if (fin.fail())
	{
		return E_INVALIDARG;
	}

	// Create the font spacing buffer.
	my_DataTable.resize(95);
	D3D10MY_FONTINFO * dispatch = &my_DataTable[0];

	// Read in the 95 used ascii characters for text.
	char temp;
	for (unsigned int index = 0; index < 95; ++index)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> dispatch[index].left;
		fin >> dispatch[index].right;
		fin >> dispatch[index].size;
	}

	// Close the file.
	fin.close();

	my_Texture = texture;

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Font, void, destory)(
	_X_ void)
{
	my_DataTable.clear();
	my_Texture.~AutoRelease();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Font, build_Text)(
	_Out_ D3D10MY_CONTEXT & _context,
	_In_ const char * _text)const
{
	const unsigned int len = strlen(_text);

	if (_context.vertexBuffer)
	{
		if (_context.textSlot == len)
		{

		}
		else
		{
			_context.vertexBuffer.~AutoRelease();
			return build_Text(_context, _text);
		}
	}
	else
	{
		HRESULT hr;
		IF_FAILED(hr = create_ContexBuffer(
			&_context.vertexBuffer,
			len, D3D10_USAGE_DEFAULT))
		{
			return hr;
		}

		_context.textSlot = len;

		return S_OK;
	}

	return E_FAIL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Font, ID3D10ShaderResourceView *, font)(
	_X_ void)const
{
	return my_Texture;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Font, create_ContexBuffer)(
	_Out_ ID3D10Buffer ** _buffer,
	_In_ unsigned int _width,
	_In_ D3D10_USAGE _usage)const
{
	std::vector<D3D10_BasicFormat> format(_width * 4);
	D3D10_BasicFormat * dispatch = &format[0];

	if (_usage == D3D10_USAGE_DEFAULT)
	{
		// Draw each letter onto a quad.
		for (unsigned int W = 0, index = 0; W < _width; ++W, index += 4)
		{
			dispatch[index] = { D3DXVECTOR3(0.0f + W, -1.0f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 1.0f) };
			dispatch[index + 1] = { D3DXVECTOR3(1.0f + W, -1.0f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 1.0f) };
			dispatch[index + 2] = { D3DXVECTOR3(0.0f + W, 0.0f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 0.0f) };
			dispatch[index + 3] = { D3DXVECTOR3(1.0f + W, 0.0f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 0.0f) };
		}
	}

	// Vertex Buffer
	D3D10_BUFFER_DESC vBufferDesc;
	vBufferDesc.ByteWidth = format.size() * sizeof(D3D10_BasicFormat);
	vBufferDesc.Usage = _usage;
	vBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vBufferDesc.CPUAccessFlags = 0;
	vBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = dispatch;

	HRESULT hr;
	IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateBuffer(
		&vBufferDesc,
		&vinitData,
		_buffer))
	{
		return hr;
	}

	return hr;
}