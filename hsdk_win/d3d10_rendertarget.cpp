#include <hsdk/win/direct3d/d3d10_rendertarget.h>
#include <hsdk/win/direct3d/d3d10_renderer.h>
#include <hsdk/win/framework.h>
#include <stack>



using namespace hsdk::direct3d;


// Ό³Έν :
std::stack<const D3D10_RenderTarget *> g_RenderTargetQueue;

//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(D3D10_RenderTarget, D3D10_RenderTarget)(void)
{
	my_Desc.Width = 0;
	my_Desc.Height = 0;
	my_Desc.MipLevels = 1;
	my_Desc.ArraySize = 1;
	my_Desc.SampleDesc.Count = 1;
	my_Desc.SampleDesc.Quality = 0;
	my_Desc.Usage = D3D10_USAGE_DEFAULT;
	my_Desc.CPUAccessFlags = (D3D10_CPU_ACCESS_FLAG)(0);
	my_Desc.MiscFlags = 0;
	
	my_Vp.Width = 0;
	my_Vp.Height = 0;
	my_Vp.MinDepth = 0;
	my_Vp.MaxDepth = 1;
	my_Vp.TopLeftX = 0;
	my_Vp.TopLeftY = 0;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_RenderTarget, initialize)(
	_In_ unsigned int _width,
	_In_ unsigned int _height)
{
	if (_width == 0 || _height == 0)
	{
		return E_INVALIDARG;
	}

	if (my_Desc.Width == _width && my_Desc.Height == _height)
	{
		return S_OK;
	}

	my_Desc.Width = _width;
	my_Desc.Height = _height;

	HRESULT hr;

	my_Desc.Format = DXGI_FORMAT_D32_FLOAT;
	my_Desc.BindFlags = D3D10_BIND_DEPTH_STENCIL;

	// Create depth stencil texture
	AutoRelease<ID3D10Texture2D> ds;
	IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateTexture2D(
		&my_Desc,
		nullptr,
		&ds))
	{
		return hr;
	}

	// Create the depth stencil view
	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;

	descDSV.Format = my_Desc.Format;
	descDSV.ViewDimension = 1 < my_Desc.SampleDesc.Count
		? D3D10_DSV_DIMENSION_TEXTURE2DMS : D3D10_DSV_DIMENSION_TEXTURE2D;

	descDSV.Texture2D.MipSlice = 0;

	AutoRelease<ID3D10DepthStencilView> dsv;
	IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateDepthStencilView(
		ds,
		&descDSV,
		&dsv))
	{
		return hr;
	}

	my_Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	my_Desc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;

	AutoRelease<ID3D10Texture2D> texure2D;
	IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateTexture2D(
		&my_Desc,
		nullptr,
		&texure2D))
	{
		return hr;
	}

	D3D10_RENDER_TARGET_VIEW_DESC descRTV;
	descRTV.Format = my_Desc.Format;
	descRTV.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
	descRTV.Texture2D.MipSlice = 0;

	AutoRelease<ID3D10RenderTargetView> rtv;
	IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateRenderTargetView(
		texure2D,
		&descRTV,
		&rtv))
	{
		return hr;
	}

	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = my_Desc.Format;
	srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	AutoRelease<ID3D10ShaderResourceView> srv;
	IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateShaderResourceView(
		texure2D,
		&srvDesc,
		&srv))
	{
		return hr;
	}
	
	my_Vp.Width = _width;
	my_Vp.Height = _height;

	my_texure2D = texure2D;
	my_DSV = dsv;
	my_RTV = rtv;
	my_SRV = srv;

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_RenderTarget, begin)(
	_In_ const float * _color)
{
	ID3D10Device * refdevice = framework::g_Framework_Device.d3d10Device;
	refdevice->ClearRenderTargetView(my_RTV, _color);
	refdevice->ClearDepthStencilView(my_DSV, D3D10_CLEAR_DEPTH, 1.0, 0);
	refdevice->OMSetRenderTargets(1, &my_RTV, my_DSV);

	// Setup the viewport to match the backbuffer
	refdevice->RSSetViewports(1, &my_Vp);

	//
	g_RenderTargetQueue.push(this);

	return S_OK;
}

///--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_RenderTarget, void, end)(
	_X_ void)
{
	g_RenderTargetQueue.pop();
	if (g_RenderTargetQueue.empty())
	{
		framework::g_Framework_Device.d3d10Device->OMSetRenderTargets(1,
			&framework::g_Framework_Device.d3d10RTV,
			framework::g_Framework_Device.d3d10DSV);
	}
	else
	{
		auto iter = g_RenderTargetQueue.top();
		framework::g_Framework_Device.d3d10Device->OMSetRenderTargets(1,
			&iter->my_RTV,
			iter->my_DSV);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_RenderTarget, void, viewport)(
	_X_ void)
{
	// Setup the viewport to match the backbuffer
	framework::g_Framework_Device.d3d10Device->RSSetViewports(1,
		&framework::g_Framework_Device.d3d10ViewPort);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_RenderTarget, ID3D10ShaderResourceView *, get_View)(
	_X_ void)const
{
	return my_SRV;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_RenderTarget, D3D10_TEXTURE2D_DESC, get_Desc)(
	_X_ void)const
{
	return my_Desc;
}