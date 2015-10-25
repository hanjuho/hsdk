#include "common.h"



//--------------------------------------------------------------------------------------
IMPL_FUNC(common::initialize_Common)(
	_X_ void)
{
	return g_Direct8_SoundDevice.initialize() | g_D3D10_MeshRenderer.initialize(L"");
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, common::destroy_Common)(
	_X_ void)
{
	g_Direct8_SoundDevice.destroy();
	g_D3D10_MeshRenderer.destroy();
	g_D3D10_Factory.destroy();
	g_Framework.destroy();
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
		0.1f, 1000.0f);

	return S_OK;
}