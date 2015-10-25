#include "common.h"
#include "entry.h"

// main
int CALLBACK wWinMain(HINSTANCE _hInstance, HINSTANCE, LPWSTR, int)
{
	// Set DXUT callbacks
	framework::g_Framework_Callbacks.windowMsgFunc = entry::OnMsgProc;
	framework::g_Framework_Callbacks.mouseFunc = entry::OnMouse;
	framework::g_Framework_Callbacks.keyboardFunc = entry::OnKeyboard;
	framework::g_Framework_Callbacks.frameMoveFunc = entry::OnFrameMove;
	framework::g_Framework_Callbacks.modifyDevice10SettingsFunc = entry::ModifyDeviceSettings;
	framework::g_Framework_Callbacks.isD3D10DeviceAcceptableFunc = entry::IsD3D10DeviceAcceptable;
	framework::g_Framework_Callbacks.d3d10DeviceCreatedFunc = entry::OnD3D10CreateDevice;
	framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc = entry::OnD3D10DestroyDevice;
	framework::g_Framework_Callbacks.d3d10FrameRenderFunc = entry::OnD3D10FrameRender;

	framework::g_Framework_Callbacks.d3d10SwapChainResizedFunc = common::OnD3D10SwapchainResized;

	HRESULT hr = S_OK;
	hr = ADD_FLAG(framework::g_Framework.setup0_Window(L"Skinning10", 0, 0), hr);
	hr = ADD_FLAG(framework::g_Framework.setup1_DeviceFactory(new framework::Framework_DeviceFactory()), hr);
	hr = ADD_FLAG(framework::g_Framework.setup2_Device10(framework::D3D10_DEVICE_DESC(true, 1440, 900)), hr);

	IF_SUCCEEDED(hr)
	{
		// Enter into the DXUT render loop
		framework::g_Framework.mainLoop();
	}

	return 0;
}