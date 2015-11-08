#include "common.h"
#include "entry.h"
#include "hsdk/win/frame/multicontainer.h"

//--------------------------------------------------------------------------------------
frame::inputEventHelper common::g_GUIHelper(nullptr);

//--------------------------------------------------------------------------------------
int CALLBACK wWinMain(HINSTANCE _hInstance, HINSTANCE, LPWSTR, int)
{
	// Set DXUT callbacks
	framework::g_Framework_Callbacks.windowMsgFunc = entry::OnMsgProc;
	framework::g_Framework_Callbacks.mouseFunc = entry::OnMouse;
	framework::g_Framework_Callbacks.keyboardFunc = entry::OnKeyboard;
	framework::g_Framework_Callbacks.frameMoveFunc = entry::OnFrameMove;

	framework::g_Framework_Callbacks.d3d10DeviceCreatedFunc = entry::OnD3D10CreateDevice;
	framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc = entry::OnD3D10DestroyDevice;
	framework::g_Framework_Callbacks.d3d10FrameRenderFunc = entry::OnD3D10FrameRender;

	framework::g_Framework_Callbacks.d3d10SwapChainResizedFunc = common::OnD3D10SwapchainResized;

	HRESULT hr = S_OK;
	hr = ADD_FLAG(hr, framework::g_Framework.setup0_Window(L"Colosseum", 0, 0));
	hr = ADD_FLAG(hr, framework::g_Framework.setup1_DeviceFactory(new framework::Framework_DeviceFactory()));
	hr = ADD_FLAG(hr, framework::g_Framework.setup2_Device10(framework::D3D10_DEVICE_DESC(true, 1440, 900)));

	IF_SUCCEEDED(hr)
	{
		// Enter into render loop
		framework::g_Framework.mainLoop();
	}
	
	sound::g_FMOD_SoundDevice.destroy();
	direct3d::g_D3D10_Renderer.destroy();
	framework::g_Framework.destroy();

	return 0;
}