#include <hsdk/win/frame/d3d11.h>



using namespace hsdk;
using namespace win::frame;


//grobal
float bg_color[] = { 0.2f, 0.2f, 0.2f, 1.0f };
unsigned int panel_stride = sizeof(unsigned int);
unsigned int panel_offset = 0;


//--------------------------------------------------------------------------------------
AutoRelease<ID3D11Device> D3D11::DEVICE;
AutoRelease<ID3D11DeviceContext> D3D11::CONTEXT;
AutoRelease<IDXGISwapChain> D3D11::CHAIN;
AutoRelease<ID3D11RenderTargetView> D3D11::RTVIEW;
AutoRelease<ID3D11DepthStencilView> D3D11::DSVIEW;
D3D_DRIVER_TYPE D3D11::DRIVERTYPE;
D3D_FEATURE_LEVEL D3D11::FEATURELEVEL;
XMMATRIX D3D11::PROJ;
XMMATRIX D3D11::VIEW;
XMMATRIX D3D11::WORLD;
std::hash_map<D3D11::SAMPLER, AutoRelease<ID3D11SamplerState>> D3D11::SAMPLER_CONTAINER;
std::hash_map<std::wstring, AutoRelease<ID3D11ShaderResourceView>> D3D11::TEXTURE_CONTAINER;
AutoRelease<ID3D11Buffer> D3D11::PANEL_IN_BUFFER;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(D3D11, initialize)(
	/* [in] */ HWND _hwnd,
	/* [in] */ const wchar_t * _modename)
{
	long hr = S_OK;

	RECT rc;
	GetClientRect(_hwnd, &rc);
	unsigned int width = rc.right - rc.left;
	unsigned int height = rc.bottom - rc.top;

	unsigned int createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	unsigned int numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	unsigned int numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	for (unsigned int driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		DRIVERTYPE = driverTypes[driverTypeIndex];
		IF_SUCCEEDED(hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			driverType,
			NULL,
			createDeviceFlags,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&sd,
			&CHAIN,
			&DEVICE,
			&FEATURELEVEL,
			&CONTEXT))
		{
			break;
		}
	}

	IF_FAILED(hr)
	{
		return hr;
	}

	// Create a render target view
	AutoRelease<ID3D11Texture2D> pBackBuffer;
	IF_FAILED(hr = CHAIN->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&pBackBuffer))
	{
		return hr;
	}

	IF_FAILED(hr = DEVICE->CreateRenderTargetView(pBackBuffer, NULL, &RTVIEW))
	{
		return hr;
	}

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	AutoRelease<ID3D11Texture2D> pDepthStencil;
	IF_FAILED(hr = DEVICE->CreateTexture2D(&descDepth, NULL, &pDepthStencil))
	{
		return hr;
	}

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	IF_FAILED(hr = DEVICE->CreateDepthStencilView(pDepthStencil, &descDSV, &DSVIEW))
	{
		return hr;
	}

	CONTEXT->OMSetRenderTargets(1, &RTVIEW, NULL);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	CONTEXT->RSSetViewports(1, &vp);

	{
		// Create index buffer
		WORD indices[] =
		{
			3, 1, 0,
			2, 1, 3
		};

		D3D11_BUFFER_DESC ibDesc;
		ibDesc.ByteWidth = sizeof(indices);
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		ibDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG(0);
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = sizeof(unsigned short);

		// 인덱스 버퍼 내용 추가
		D3D11_SUBRESOURCE_DATA ibSd;
		ibSd.pSysMem = indices;
		ibSd.SysMemPitch = 0;
		ibSd.SysMemSlicePitch = 0;

		// 인덱스 버퍼 생성
		IF_FAILED(hr = DEVICE->CreateBuffer(&ibDesc, &ibSd, &PANEL_IN_BUFFER))
		{
			return hr;
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11, void, destroy)(
	/* [none] */ void)
{
	TEXTURE_CONTAINER.clear();
	SAMPLER_CONTAINER.clear();

	PANEL_IN_BUFFER.~AutoRelease();
	
	DSVIEW.~AutoRelease();
	RTVIEW.~AutoRelease();
	CHAIN.~AutoRelease();
	CONTEXT.~AutoRelease();
	DEVICE.~AutoRelease();
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11, void, clear_Backbuffer)(
	/* [none] */ void)
{
	// 버퍼 클리어
	CONTEXT->ClearRenderTargetView(RTVIEW, bg_color);

	// z 버퍼 클리어
	CONTEXT->ClearDepthStencilView(DSVIEW, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11, void, swap_Backbuffer)(
	/* [none] */ void)
{
	// 버퍼 스왑
	CHAIN->Present(1, 0);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(D3D11, get_Texture)(
	/* [in] */  const wchar_t * _directory,
	/* [in] */ ID3D11ShaderResourceView * (&_texture))
{
	std::hash_map<std::wstring, AutoRelease<ID3D11ShaderResourceView>>::iterator iter = 
		TEXTURE_CONTAINER.find(_directory);

	if (iter != TEXTURE_CONTAINER.end())
	{
		_texture = iter->second;
	}
	else
	{
		HRESULT hr;
		IF_FAILED(hr = D3DX11CreateShaderResourceViewFromFile(
			DEVICE,
			_directory,
			NULL,
			NULL,
			&_texture,
			NULL))
		{
			return hr;
		}

		TEXTURE_CONTAINER.insert(
			std::hash_map<std::wstring, AutoRelease<ID3D11ShaderResourceView>>::value_type(
			_directory,
			AutoRelease<ID3D11ShaderResourceView>(_texture)));
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(D3D11, get_Sampler)(
	/* [in] */ SAMPLER _state,
	/* [in] */ ID3D11SamplerState * (&_sampler))
{
	std::hash_map<SAMPLER, AutoRelease<ID3D11SamplerState>>::iterator iter =
		SAMPLER_CONTAINER.find(_state);

	if (iter != SAMPLER_CONTAINER.end())
	{
		_sampler = iter->second;
	}
	else
	{
		D3D11_SAMPLER_DESC samplerDesc;
		switch (_state)
		{
		case SAMPLER_DEFAULT:
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			samplerDesc.BorderColor[0] = 0;
			samplerDesc.BorderColor[1] = 0;
			samplerDesc.BorderColor[2] = 0;
			samplerDesc.BorderColor[3] = 0;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			break;
		}

		HRESULT hr;
		IF_FAILED(hr = DEVICE->CreateSamplerState(
			&samplerDesc,
			&_sampler))
		{
			return hr;
		}

		SAMPLER_CONTAINER.insert(
			std::hash_map<SAMPLER, AutoRelease<ID3D11SamplerState>>::value_type(
			_state,
			AutoRelease<ID3D11SamplerState>(_sampler)));
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(D3D11, create_Panel)(
	/* [in] */ XMFLOAT2(&_uvs)[4],
	/* [in] */ D3D11_USAGE _usage,
	/* [out] */ ID3D11Buffer * (&_buffer))
{
	//
	panel_UV verties[4] =
	{
		{ { -1.0f, -1.0f, 0.0f }, _uvs[0] },
		{ { 1.0f, -1.0f, 0.0f }, _uvs[1] },
		{ { 1.0f, 1.0f, 0.0f }, _uvs[2] },
		{ { -1.0f, 1.0f, 0.0f }, _uvs[3] },
	};

	D3D11_BUFFER_DESC vbDesc;
	// x,y,z and u,v
	vbDesc.ByteWidth = sizeof(verties);
	vbDesc.Usage = _usage;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG(0);
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = sizeof(panel_UV);

	switch (_usage)
	{
	case D3D11_USAGE_DYNAMIC:
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	}

	// 버텍스 버퍼 내용 추가
	D3D11_SUBRESOURCE_DATA vbSd;
	vbSd.pSysMem = verties;
	vbSd.SysMemPitch = 0;
	vbSd.SysMemSlicePitch = 0;

	// 버텍스 버퍼 생성
	return DEVICE->CreateBuffer(&vbDesc, &vbSd, &_buffer);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11, void, render_Panel)(
	/* [in] */ ID3D11Buffer * _buffer)
{
	// 버텍스 버퍼 적용
	CONTEXT->IASetVertexBuffers(0, 1,
		&_buffer,
		&panel_stride,
		&panel_offset);

	// 인덱스 버퍼 적용
	CONTEXT->IASetIndexBuffer(
		PANEL_IN_BUFFER, DXGI_FORMAT_R16_UINT, 0);

	// 출력
	CONTEXT->DrawIndexed(6, 0, 0);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(D3D11, create_ContantBuffers)(
	/* [in] */ unsigned long _size,
	/* [in] */ D3D11_USAGE _usage,
	/* [out] */ ID3D11Buffer * (&_buffer))
{
	// 상수 버퍼 파라미터 생성
	D3D11_BUFFER_DESC cbDescs;

	// 셰이더 상수 세부 설정
	cbDescs.ByteWidth = max(_size, 16);
	cbDescs.Usage = _usage;
	cbDescs.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDescs.MiscFlags = (D3D11_RESOURCE_MISC_FLAG)0;
	cbDescs.StructureByteStride = 0;
	cbDescs.CPUAccessFlags = (D3D11_CPU_ACCESS_FLAG)0;

	switch (_usage)
	{
	case D3D11_USAGE_DYNAMIC:
		cbDescs.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	}

	// 셰이더 상수 버퍼 생성
	return DEVICE->CreateBuffer(&cbDescs, NULL, &_buffer);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(D3D11, create_inputLayoutForHeader)(
	/* [in] */	const char * _shadername,
	/* [in] */	unsigned int _shadersize,
	/* [in] */	D3D11inputEachFormat * _formats,
	/* [in] */	unsigned int _formatsize,
	/* [in] */	D3D11_INPUT_CLASSIFICATION _InputSlotClass,
	/* [out] */ ID3D11InputLayout * (&_layout))
{
	// 인풋 객체 시멘틱스 파라미터 생성
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputDescs(_formatsize);
	for (unsigned int index = 0; index < _formatsize; ++index)
	{
		// 인풋 객체 시멘틱스 설정
		inputDescs[index].SemanticName = _formats[index].semantic;
		inputDescs[index].SemanticIndex = 0;
		inputDescs[index].Format = _formats[index].format;
		inputDescs[index].InputSlot = 0;
		inputDescs[index].AlignedByteOffset = _formats[index].offset;
		inputDescs[index].InputSlotClass = _InputSlotClass;
		inputDescs[index].InstanceDataStepRate = 0;
	}

	return DEVICE->CreateInputLayout(
		&inputDescs[0],
		inputDescs.size(),
		_shadername,
		_shadersize,
		&_layout);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(D3D11, create_VertexShaderForHeader)(
	/* [in] */	const char * _name,
	/* [in] */	unsigned int _size,
	/* [out] */ ID3D11VertexShader * (&_shader))
{
	return DEVICE->CreateVertexShader(
		_name,
		_size,
		NULL,
		&_shader);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(D3D11, create_PixelShaderForHeader)(
	/* [in] */	const char * _name,
	/* [in] */	unsigned int _size,
	/* [out] */ ID3D11PixelShader * (&_shader))
{
	return DEVICE->CreatePixelShader(
		_name,
		_size,
		NULL,
		&_shader);
}