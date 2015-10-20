#pragma comment (lib, "hsdk_win.lib")



#include <hsdk/win/frame/frame.h>
#include <hsdk/win/frame/direct3d/d3d10_master.h>
#include <hsdk/win/frame/direct3d/d3d10_mesh.h>
#include <hsdk/win/frame/direct3d/d3d10_meshrenderer.h>
#include <hsdk/win/frame/direct3d/direct3d_camera.h>



using namespace hsdk;
using namespace win::frame;
using namespace direct3d;


//--------------------------------------------------------------------------------------
// Grobal Variable
//--------------------------------------------------------------------------------------
// 설명 : 
Frame g_Window;

// 설명 : 
Direct3D_Camera g_Camera;

// 설명 : 
D3D10_Mesh g_Mesh;

//
Graphics graphics_0;
Graphics graphics_1;

D3DXMATRIX g_World;
D3DXMATRIX g_View;
D3DXMATRIX g_Projection;

const wchar_t * image[6] = {
	L"1.jpg", L"2.jpg", L"3.jpg", L"4.jpg", L"5.jpg", L"6.jpg" };

//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc(
	BOOL * pbNoFurtherProcessing,
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam,
	void * pUserContext)
{
	g_Window.message_Proc(uMsg, wParam, lParam);

	return 0;
}

void CALLBACK OnMouse(
	/* [r] */ const BOOL * _buttonsDown,
	/* [r] */ unsigned int _buttonCount,
	/* [r] */ int _mouseWheelDelta,
	/* [r] */ int _xPos,
	/* [r] */ int _yPos,
	/* [r/w] */ void * _userContext)
{

}

void CALLBACK OnKeyboard(
	UINT nChar,
	BOOL bKeyDown,
	BOOL bAltDown,
	void * pUserContext)
{

}

void CALLBACK OnFrameMove(
	double fTime,
	float fElapsedTime,
	void * pUserContext)
{

}

BOOL CALLBACK ModifyDeviceSettings(
	const D3D10_DEVICE_DESC & pDeviceSettings,
	void * pUserContext)
{
	return true;
}

BOOL CALLBACK IsD3D10DeviceAcceptable(
	/* [r] */ unsigned int _adapter,
	/* [r] */ unsigned int _output,
	/* [r] */ D3D10_DRIVER_TYPE _deviceType,
	/* [r] */ DXGI_FORMAT _backBufferFormat,
	/* [r] */ BOOL _windowed,
	/* [r/w] */ void * _userContext)
{
	return true;
}

HRESULT CALLBACK OnD3D10CreateDevice(
	ID3D10Device * pd3dDevice,
	const DXGI_SURFACE_DESC & pBackBufferSurfaceDesc,
	void * pUserContext)
{
	D3DXVECTOR3 Eye(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 At(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);

	// Initialize the view matrix
	D3DXMatrixLookAtLH(&g_View, &Eye, &At, &Up);

	// Initialize the projection matrix
	D3DXMatrixPerspectiveFovLH(
		&g_Projection,
		(float)D3DX_PI * 0.25f,
		pBackBufferSurfaceDesc.Width / pBackBufferSurfaceDesc.Height,
		0.1f,
		1000.0f);

	HRESULT hr;

	hr = S_OK;
	hr = ADD_FLAG(g_D3D10_Master.initialize(pd3dDevice), hr);
	hr = ADD_FLAG(g_D3D10_MeshRenderer.initialize(pd3dDevice), hr);

	ID3D10ShaderResourceView * view = nullptr;
	
	return hr;
}

void CALLBACK OnD3D10FrameRender(
	ID3D10Device * pd3dDevice,
	double fTime,
	float fElapsedTime,
	void * pUserContext)
{
	// Clear the render target and depth stencil
	float ClearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	ID3D10RenderTargetView * pRTV = g_D3D.get_Device()->d3d10RTV;
	pd3dDevice->ClearRenderTargetView(pRTV, ClearColor);
	ID3D10DepthStencilView * pDSV = g_D3D.get_Device()->d3d10DSV;
	pd3dDevice->ClearDepthStencilView(pDSV, D3D10_CLEAR_DEPTH, 1.0, 0);
	

}

void CALLBACK OnD3D10DestroyDevice(
	void * pUserContext)
{

}

// main
int CALLBACK wWinMain(HINSTANCE _hInstance, HINSTANCE, LPWSTR, int)
{
	// Set DXUT callbacks
	g_D3D.callbacks()->windowMsgFunc = MsgProc;
	g_D3D.callbacks()->mouseFunc = OnMouse;
	g_D3D.callbacks()->keyboardFunc = OnKeyboard;
	g_D3D.callbacks()->frameMoveFunc = OnFrameMove;
	g_D3D.callbacks()->modifyDevice10SettingsFunc = ModifyDeviceSettings;

	g_D3D.callbacks()->isD3D10DeviceAcceptableFunc = IsD3D10DeviceAcceptable;
	g_D3D.callbacks()->d3d10DeviceCreatedFunc = OnD3D10CreateDevice;
	g_D3D.callbacks()->d3d10DeviceDestroyedFunc = OnD3D10DestroyDevice;
	g_D3D.callbacks()->d3d10FrameRenderFunc = OnD3D10FrameRender;

	HRESULT hr = S_OK;
	hr = ADD_FLAG(g_D3D.setup0_Window(L"Skinning10", 800, 800), hr);
	hr = ADD_FLAG(g_D3D.setup1_DeviceFactory(new direct3d::Direct3D_DeviceFactory()), hr);
	hr = ADD_FLAG(g_D3D.setup2_Device10(D3D10_DEVICE_DESC(false, 1600, 1500)), hr);
	
	g_D3D.transform(true, 0, 0, true);

	IF_SUCCEEDED(hr)
	{
		g_D3D.mainLoop(); // Enter into the DXUT render loop
	}

	g_D3D10_MeshRenderer.destroy();
	g_D3D10_Master.destroy();
	g_D3D.destroy();

	return 0;
}