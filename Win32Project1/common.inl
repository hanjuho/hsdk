#include "common.h"

//--------------------------------------------------------------------------------------
IMPL_FUNC(common::initialize_Common)(
	_X_ void)
{
	HRESULT hr = sound::g_FMOD_SoundDevice.initialize() | direct3d::g_D3D10_Renderer.initialize(L"shader/");
	IF_SUCCEEDED(hr)
	{
		float pos[3] = { 0.0f, 0.0f, 0.0f };
		sound::g_FMOD_SoundDevice.move(pos);
	}

	return hr;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, common::destroy_Common)(
	_X_ void)
{
	sound::g_FMOD_SoundDevice.clear();
	direct3d::g_D3D10_Factory.clear();
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(common::OnD3D10SwapchainResized)(
	_In_ ID3D10Device * _d3dDevice,
	_In_ IDXGISwapChain * _swapChain,
	_In_ const DXGI_SURFACE_DESC & _backBufferSurfaceDesc,
	_Inout_ void * _userContext)
{
	// Initialize the projection matrix
	D3DXMatrixPerspectiveFovLH(
		&direct3d::g_D3D10_ProjMatrix,
		(float)D3DX_PI * 0.25f,
		(float)_backBufferSurfaceDesc.Width /
		(float)_backBufferSurfaceDesc.Height,
		0.1f, 10000.0f);

	return S_OK;
}