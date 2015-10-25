#pragma once



#pragma comment (lib, "hsdk_win.lib")



#include <hsdk/win/framework.h>
#include <hsdk/win/framework_camera.h>
#include <hsdk/win/frame/buttoncompo.h>
#include <hsdk/win/frame/inputeventhelper.h>
#include <hsdk/win/direct3d/d3d10_factory.h>
#include <hsdk/win/direct3d/d3d10_mesh.h>
#include <hsdk/win/direct3d/d3d10_meshrenderer.h>
#include <hsdk/win/sound/d8_sounddevice.h>
#include <hsdk/win/sound/d8_soundplayer.h>



using namespace hsdk;
using namespace framework;
using namespace frame;
using namespace direct3d;
using namespace sound;



namespace common
{

	// ���� : 
	DECL_FUNC(initialize_Common)(
		_X_ void);

	// ���� : 
	DECL_FUNC_T(void, destroy_Common)(
		_X_ void);

	// ���� : 
	DECL_FUNC(OnD3D10SwapchainResized)(
		_In_ ID3D10Device * _d3dDevice,
		_In_ IDXGISwapChain * _swapChain,
		_In_ const DXGI_SURFACE_DESC & _backBufferSurfaceDesc,
		_Inout_ void * _userContext);
}


#include "common.inl"