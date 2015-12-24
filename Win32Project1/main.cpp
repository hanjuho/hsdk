#include "common.h"
#include "entry.h"
#include "hsdk/embeddedpattern.h"
#include "game/gameobject.h"

//--------------------------------------------------------------------------------------
frame::inputEventHelper common::g_GUIHelper(nullptr);

//--------------------------------------------------------------------------------------
int CALLBACK wWinMain(HINSTANCE _hInstance, HINSTANCE, LPWSTR, int)
{
	// Set DXUT callbacks
	framework::g_Framework_Callbacks.windowMsgFunc = entryloop::OnMsgProc;
	framework::g_Framework_Callbacks.mouseFunc = entryloop::OnMouse;
	framework::g_Framework_Callbacks.keyboardFunc = entryloop::OnKeyboard;
	framework::g_Framework_Callbacks.frameMoveFunc = entryloop::OnFrameMove;
	framework::g_Framework_Callbacks.d3d10DeviceCreatedFunc = entryloop::OnD3D10CreateDevice;
	framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc = entryloop::OnD3D10DestroyDevice;
	framework::g_Framework_Callbacks.d3d10FrameRenderFunc = entryloop::OnD3D10FrameRender;
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