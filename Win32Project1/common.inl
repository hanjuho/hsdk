#include "common.h"

//--------------------------------------------------------------------------------------
IMPL_FUNC(common::initialize_Common)(
	_In_ frame::Container * _container)
{
	HRESULT hr = sound::g_FMOD_SoundDevice.initialize() | direct3d::g_D3D10_Renderer.initialize(L"shader/");
	IF_SUCCEEDED(hr)
	{
		float pos[3] = { 0.0f, 0.0f, 0.0f };
		sound::g_FMOD_SoundDevice.move(pos);

		g_GUIHelper.restore(_container);
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

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, common::OnMouse)(
	_In_ const short * _buttonsDown,
	_In_ unsigned int _buttonCount,
	_In_ int _mouseWheelDelta,
	_In_ int _xPos,
	_In_ int _yPos,
	_Inout_ void * _userContext)
{
	// Ό³Έν :
	static int mouse_X = _xPos, mouse_Y = _yPos;

	int dx = _xPos - mouse_X;
	int dy = _yPos - mouse_Y;
	bool moved = false;

	if (_buttonsDown[FRAMEWORK_LEFTBUTTON] == 1)
	{
		g_GUIHelper.onClick_Down(i::frame::LBUTTON, _xPos, _yPos);

		if (dx || dy)
		{
			moved = true;
			g_GUIHelper.onDrag(i::frame::LBUTTON, dx, dy);
		}
	}
	else if (_buttonsDown[FRAMEWORK_LEFTBUTTON] == 2)
	{
		g_GUIHelper.onClick_Up(i::frame::LBUTTON, _xPos, _yPos);
	}

	if (_buttonsDown[FRAMEWORK_MIDDLEBUTTON])
	{
		g_GUIHelper.onClick_Down(i::frame::WBUTTON, _xPos, _yPos);
	}
	else if (_buttonsDown[FRAMEWORK_MIDDLEBUTTON] == 2)
	{
		g_GUIHelper.onClick_Up(i::frame::LBUTTON, _xPos, _yPos);
	}

	if (_buttonsDown[FRAMEWORK_RIGHTBUTTON])
	{
		g_GUIHelper.onClick_Down(i::frame::RBUTTON, _xPos, _yPos);
	}
	else if (_buttonsDown[FRAMEWORK_RIGHTBUTTON] == 2)
	{
		g_GUIHelper.onClick_Up(i::frame::LBUTTON, _xPos, _yPos);
	}

	if (_mouseWheelDelta)
	{
		g_GUIHelper.onWheel(_xPos, _yPos, _mouseWheelDelta);
	}

	IF_FALSE(moved)
	{
		g_GUIHelper.onMove(dx, dy);
	}

	mouse_X = _xPos;
	mouse_Y = _yPos;
}