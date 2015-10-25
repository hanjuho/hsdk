#include "common.h"
#include "entry.h"

// main
int CALLBACK wWinMain(HINSTANCE _hInstance, HINSTANCE, LPWSTR, int)
{
	// Set DXUT callbacks
	g_Framework_Callbacks.windowMsgFunc = entry::OnMsgProc;
	g_Framework_Callbacks.mouseFunc = entry::OnMouse;
	g_Framework_Callbacks.keyboardFunc = entry::OnKeyboard;
	g_Framework_Callbacks.frameMoveFunc = entry::OnFrameMove;
	g_Framework_Callbacks.modifyDevice10SettingsFunc = entry::ModifyDeviceSettings;
	g_Framework_Callbacks.isD3D10DeviceAcceptableFunc = entry::IsD3D10DeviceAcceptable;
	g_Framework_Callbacks.d3d10DeviceCreatedFunc = entry::OnD3D10CreateDevice;
	g_Framework_Callbacks.d3d10DeviceDestroyedFunc = entry::OnD3D10DestroyDevice;
	g_Framework_Callbacks.d3d10FrameRenderFunc = entry::OnD3D10FrameRender;

	g_Framework_Callbacks.d3d10SwapChainResizedFunc = common::OnD3D10SwapchainResized;

	HRESULT hr = S_OK;
	hr = ADD_FLAG(g_Framework.setup0_Window(L"Skinning10", 0, 0), hr);
	hr = ADD_FLAG(g_Framework.setup1_DeviceFactory(new Framework_DeviceFactory()), hr);
	hr = ADD_FLAG(g_Framework.setup2_Device10(D3D10_DEVICE_DESC(true, 1440, 900)), hr);

	IF_SUCCEEDED(hr)
	{
		// Enter into the DXUT render loop
		g_Framework.mainLoop();
	}

	return 0;
}