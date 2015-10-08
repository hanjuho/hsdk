#pragma comment (lib, "hsdk_win.lib")



#include <hsdk/win/frame/direct3d/direct3d.h>



using namespace hsdk;
using namespace direct3d;


//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc(BOOL* pbNoFurtherProcessing, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void* pUserContext)
{
	return 0;
}

void CALLBACK OnKeyboard(UINT nChar, BOOL bKeyDown, BOOL bAltDown, void* pUserContext)
{

}

void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{

}

BOOL CALLBACK ModifyDeviceSettings(Direct3D_DeviceDescs & pDeviceSettings, void* pUserContext)
{
	return true;
}

BOOL CALLBACK IsD3D9DeviceAcceptable(
	/* [r] */ const D3DCAPS9 & _caps,
	/* [r] */ D3DFORMAT _adapterFormat,
	/* [r] */ D3DFORMAT _backBufferFormat,
	/* [r] */ BOOL _windowed,
	/* [r/w] */ void * _userContext)
{
	return true;
}

HRESULT CALLBACK OnD3D9CreateDevice(
	IDirect3DDevice9* pd3dDevice,
	const D3DSURFACE_DESC & pBackBufferSurfaceDesc,
	void* pUserContext)
{
	return S_OK;
}

void CALLBACK OnD3D9FrameRender(
	IDirect3DDevice9* pd3dDevice, 
	double fTime, 
	float fElapsedTime,
	void* pUserContext)
{

}

void CALLBACK OnD3D9DestroyDevice(
	void* pUserContext)
{

}

// main
int CALLBACK wWinMain(HINSTANCE _hInstance, HINSTANCE, LPWSTR, int)
{
	Direct3D direct3d;

	// Set DXUT callbacks
	direct3d.set_Callback_MsgProc(MsgProc);
	direct3d.set_Callback_Keyboard(OnKeyboard);
	direct3d.set_Callback_FrameMove(OnFrameMove);
	direct3d.set_Callback_DeviceChanging(ModifyDeviceSettings);

	direct3d.set_Callback_D3D9_DeviceAcceptable(IsD3D9DeviceAcceptable);
	direct3d.set_Callback_D3D9_DeviceCreated(OnD3D9CreateDevice);
	direct3d.set_Callback_D3D9_DeviceDestroyed(OnD3D9DestroyDevice);
	direct3d.set_Callback_D3D9_FrameRender(OnD3D9FrameRender);
	
	// Parse the command line, show msgboxes on error, no extra command line params
	HRESULT hr = direct3d.initialize_Default(true, true, NULL); 
	hr = ADD_FLAG(direct3d.initialize_Window(L"Skinning10"), hr);
	hr = ADD_FLAG(direct3d.initialize_Device(true, 640, 480), hr);

	IF_SUCCEEDED(hr)
	{
		direct3d.mainLoop(); // Enter into the DXUT render loop
	}

	return direct3d.get_ExitCode();
}