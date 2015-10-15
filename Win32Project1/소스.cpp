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
D3D10_Master g_Master;

// 설명 : 
Frame g_Window;

// 설명 : 
Direct3D_Camera g_Camera;

// 설명 : 
D3D10_Mesh g_MeshSkyBox;

// 설명 : 
D3D10_MeshRenderer g_MeshRenderer;

D3DXMATRIX g_World;
D3DXMATRIX g_View;
D3DXMATRIX g_Projection;

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
	Direct3D_DeviceDescs & pDeviceSettings,
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
	D3DXVECTOR3 Eye(1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 At(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);

	// Initialize the view matrix
	D3DXMatrixLookAtLH(&g_View, &Eye, &At, &Up);

	// Initialize the projection matrix
	D3DXMatrixPerspectiveFovLH(&g_Projection, (float)D3DX_PI * 0.25f, 640.0f / 480.0f, 0.1f, 100.0f);

	return true;
}

HRESULT CALLBACK OnD3D10CreateDevice(
	const Direct3D_Device & pd3dDevice,
	const DXGI_SURFACE_DESC & pBackBufferSurfaceDesc,
	void * pUserContext)
{
	HRESULT hr;

	hr = g_Window.initialize(&g_Master);
	hr = ADD_FLAG(g_MeshRenderer.initialize(pd3dDevice.d3d10Device), hr);
	hr = ADD_FLAG(g_Master.create_MeshSkyBox(g_MeshSkyBox, 1.0f), hr);

	ID3D10ShaderResourceView * view;

	g_Master.get_Texture(&view, L"1.jpg");
	g_MeshSkyBox.setup_Texture(0, 0, view);

	g_Master.get_Texture(&view, L"2.jpg");
	g_MeshSkyBox.setup_Texture(1, 0, view);

	g_Master.get_Texture(&view, L"3.jpg");
	g_MeshSkyBox.setup_Texture(2, 0, view);

	g_Master.get_Texture(&view, L"4.jpg");
	g_MeshSkyBox.setup_Texture(3, 0, view);

	g_Master.get_Texture(&view, L"5.jpg");
	g_MeshSkyBox.setup_Texture(4, 0, view);

	g_Master.get_Texture(&view, L"6.jpg");
	g_MeshSkyBox.setup_Texture(5, 0, view);

	return hr;
}

void CALLBACK OnD3D10FrameRender(
	const Direct3D_Device & pd3dDevice,
	double fTime,
	float fElapsedTime,
	void * pUserContext)
{
	// Update our time
	static float t = 0.0f;
	static DWORD dwTimeStart = 0;
	DWORD dwTimeCur = GetTickCount();
	if (dwTimeStart == 0)
		dwTimeStart = dwTimeCur;
	t = (dwTimeCur - dwTimeStart) / 1000.0f;

	// Rotate cube around the origin
	D3DXMatrixRotationY(&g_World, 0.0f);

	// Clear the render target and depth stencil
	float ClearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	ID3D10RenderTargetView * pRTV = pd3dDevice.d3d10RTV;
	pd3dDevice.d3d10Device->ClearRenderTargetView(pRTV, ClearColor);
	ID3D10DepthStencilView * pDSV = pd3dDevice.d3d10DSV;
	pd3dDevice.d3d10Device->ClearDepthStencilView(pDSV, D3D10_CLEAR_DEPTH, 1.0, 0);

	g_MeshRenderer.render_SkyBox(g_World * g_View * g_Projection, g_MeshSkyBox);
}

void CALLBACK OnD3D10DestroyDevice(
	void * pUserContext)
{

}

// main
int CALLBACK wWinMain(HINSTANCE _hInstance, HINSTANCE, LPWSTR, int)
{
	// Set DXUT callbacks
	g_Master.set_Callback_MsgProc(MsgProc);
	g_Master.set_Callback_Mouse(OnMouse);
	g_Master.set_Callback_Keyboard(OnKeyboard);
	g_Master.set_Callback_FrameMove(OnFrameMove);
	g_Master.set_Callback_DeviceChanging(ModifyDeviceSettings);

	g_Master.set_Callback_D3D10_DeviceAcceptable(IsD3D10DeviceAcceptable);
	g_Master.set_Callback_D3D10_DeviceCreated(OnD3D10CreateDevice);
	g_Master.set_Callback_D3D10_DeviceDestroyed(OnD3D10DestroyDevice);
	g_Master.set_Callback_D3D10_FrameRender(OnD3D10FrameRender);

	// Parse the command line, show msgboxes on error, no extra command line params
	HRESULT hr = g_Master.initialize_Default(true, true, NULL);
	hr = ADD_FLAG(g_Master.initialize_Window(L"Skinning10", 100, 100), hr);
	hr = ADD_FLAG(g_Master.initialize_Device(false, 640, 480), hr);

	IF_SUCCEEDED(hr)
	{
		g_Master.mainLoop(); // Enter into the DXUT render loop
	}

	g_MeshRenderer.destroy();
	g_MeshSkyBox.destroy();
	g_Window.destroy();
	g_Master.destroy_Master();

	return g_Master.get_ExitCode();
}