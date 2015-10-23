#pragma comment (lib, "hsdk_win.lib")



#include <hsdk/win/frame/buttoncompo.h>
#include <hsdk/win/frame/inputeventhelper.h>
#include <hsdk/win/frame/direct3d/direct3d.h>
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
ButtonCompo g_Button(100.0f, 100.0f, 300.0f, 300.0f);
inputEventHelper g_Helper(&g_Button);

// 설명 : 
Direct3D_Camera g_Camera;

// 설명 : 
D3D10_Mesh g_Mesh;
D3D10_MeshAnimation g_MeshAnimation;
std::vector<D3DXMATRIX> g_boneMatrixBuffer;

D3D10_Mesh g_SkyBox;

D3DXMATRIX g_World;
D3DXMATRIX g_View;
D3DXMATRIX g_Projection;

int x = 0, y = 0;

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
	return 0;
}

void CALLBACK OnMouse(
	_In_ const short * _buttonsDown,
	_In_ unsigned int _buttonCount,
	_In_ int _mouseWheelDelta,
	_In_ int _xPos,
	_In_ int _yPos,
	/* [r/w] */ void * _userContext)
{
	if (IS_FLAG(_buttonsDown[Direct3D_LEFTBUTTON], 0x01))
	{
		g_Helper.onClick_Down(i::frame::LBUTTON, _xPos, _yPos);

		int dx;
		if (dx = _xPos - x)
		{
			g_Camera.rotate_YAxis(D3DXToRadian(dx));
		}

		int dy;
		if (dy = _yPos - y)
		{
			g_Camera.rotate_XAxis(D3DXToRadian(dy));
		}
	}
	else if (IS_FLAG(_buttonsDown[Direct3D_LEFTBUTTON], 0x02))
	{
		g_Helper.onClick_Up(i::frame::LBUTTON, _xPos, _yPos);
	}

	x = _xPos;
	y = _yPos;
}

void CALLBACK OnKeyboard(
	unsigned char nChar,
	short bKeyDown,
	short bAltDown,
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
	_In_ unsigned int _adapter,
	_In_ unsigned int _output,
	_In_ D3D10_DRIVER_TYPE _deviceType,
	_In_ DXGI_FORMAT _backBufferFormat,
	_In_ BOOL _windowed,
	/* [r/w] */ void * _userContext)
{
	return true;
}

HRESULT CALLBACK OnD3D10CreateDevice(
	ID3D10Device * pd3dDevice,
	const DXGI_SURFACE_DESC & pBackBufferSurfaceDesc,
	void * pUserContext)
{
	g_Camera.set_Position(D3DXVECTOR3(0.0f, 10.0f, -10.0f));
	g_Camera.compute_ViewMatrix(g_View);

	// Initialize the projection matrix
	D3DXMatrixPerspectiveFovLH(
		&g_Projection,
		(float)D3DX_PI * 0.25f,
		(float)pBackBufferSurfaceDesc.Width /
		(float)pBackBufferSurfaceDesc.Height,
		0.1f,
		1000.0f);

	return S_OK;
}

void CALLBACK OnD3D10FrameRender(
	ID3D10Device * pd3dDevice,
	double fTime,
	float fElapsedTime,
	void * pUserContext)
{
	g_Camera.compute_ViewMatrix(g_View);

	// Clear the render target and depth stencil
	float ClearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	ID3D10RenderTargetView * pRTV = g_Direct3D_Device.d3d10RTV;
	pd3dDevice->ClearRenderTargetView(pRTV, ClearColor);
	ID3D10DepthStencilView * pDSV = g_Direct3D_Device.d3d10DSV;
	pd3dDevice->ClearDepthStencilView(pDSV, D3D10_CLEAR_DEPTH, 1.0, 0);

	g_D3D10_MeshRenderer.render_SkyBox(
		g_View, g_SkyBox);

	g_D3D10_MeshRenderer.render_Skinned(
		g_View * g_Projection,
		g_Mesh,
		&g_boneMatrixBuffer[0],
		g_boneMatrixBuffer.size());

	g_Button.render();
}

void CALLBACK OnD3D10DestroyDevice(
	void * pUserContext)
{

}

// main
int CALLBACK wWinMain(HINSTANCE _hInstance, HINSTANCE, LPWSTR, int)
{
	// Set DXUT callbacks
	g_Direct3D_Callbacks.windowMsgFunc = MsgProc;
	g_Direct3D_Callbacks.mouseFunc = OnMouse;
	g_Direct3D_Callbacks.keyboardFunc = OnKeyboard;
	g_Direct3D_Callbacks.frameMoveFunc = OnFrameMove;
	g_Direct3D_Callbacks.modifyDevice10SettingsFunc = ModifyDeviceSettings;

	g_Direct3D_Callbacks.isD3D10DeviceAcceptableFunc = IsD3D10DeviceAcceptable;
	g_Direct3D_Callbacks.d3d10DeviceCreatedFunc = OnD3D10CreateDevice;
	g_Direct3D_Callbacks.d3d10DeviceDestroyedFunc = OnD3D10DestroyDevice;
	g_Direct3D_Callbacks.d3d10FrameRenderFunc = OnD3D10FrameRender;

	HRESULT hr = S_OK;
	hr = ADD_FLAG(g_Direct3D.setup0_Window(L"Skinning10", 800, 800), hr);
	hr = ADD_FLAG(g_Direct3D.setup1_DeviceFactory(new direct3d::Direct3D_DeviceFactory()), hr);
	hr = ADD_FLAG(g_Direct3D.setup2_Device10(D3D10_DEVICE_DESC(true, 1600, 1500)), hr);

	IF_SUCCEEDED(hr | g_D3D10_MeshRenderer.initialize(L""))
	{
		const Direct3D_Window * windpw = &g_Direct3D_Window;

		g_Button.graphics()->set_Background({ 0.0f, 1.0f, 0.0f, 1.0f });
		g_Button.reform();
		g_Button.set_Visible(true);

		// g_SkyBox
		g_D3D10_Master.create_MeshSkyBox(g_SkyBox, 1.0f);
		g_D3D10_Master.create_MeshFromFile(g_Mesh, nullptr, L"data/DeathwingHuman.X", &g_MeshAnimation);

		g_boneMatrixBuffer.resize(g_MeshAnimation.get_NumOfBones());
		g_MeshAnimation.transbone(&g_boneMatrixBuffer[0], g_boneMatrixBuffer.size(), g_D3D10_ViewMatrix);

		ID3D10ShaderResourceView * texture;
		g_D3D10_Master.create_SkyBoxTexture(&texture, 512, 512,
			L"background/jajalien1_front.jpg",// front
			L"background/jajalien1_back.jpg",// back
			L"background/jajalien1_left.jpg",// left
			L"background/jajalien1_right.jpg",// rignt
			L"background/jajalien1_top.jpg",// top
			//back
			L"");

		g_SkyBox.setup1_Texture(0, 0, texture);

		// Enter into the DXUT render loop
		g_Direct3D.mainLoop();
	}

	g_SkyBox.clear();
	g_MeshAnimation.clear();
	g_Mesh.clear();
	g_D3D10_MeshRenderer.destroy();
	g_D3D10_Master.destroy();
	g_Direct3D.destroy();

	return 0;
}