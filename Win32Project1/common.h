#pragma once



#pragma comment(lib, "hsdk_win.lib")
#pragma comment(lib, "hsdk_game.lib")



#include "hsdk/win/framework.h"
#include "hsdk/win/framework_camera.h"

#include "hsdk/win/frame/container.h"
#include "hsdk/win/frame/multicontainer.h"
#include "hsdk/win/frame/buttoncompo.h"
#include "hsdk/win/frame/inputeventhelper.h"
#include "hsdk/win/frame/rendertargetcontainer.h"
#include "hsdk/win/frame/layout/borderlayout.h"
#include "hsdk/win/frame/layout/gridlayout.h"

#include "hsdk/win/direct3d/d3d10_factory.h"
#include "hsdk/win/direct3d/d3d10_mesh.h"
#include "hsdk/win/direct3d/d3d10_renderer.h"
#include "hsdk/win/direct3d/d3d10_rendertarget.h"

#include "hsdk/win/sound/fmod_sounddevice.h"



using namespace hsdk;



namespace common
{

	// 설명 :
	typedef frame::Container GUI_Target;

	// 설명 : 
	inline DECL_FUNC(initialize_Common)(
		_In_ GUI_Target * _guiTarget);

	// 설명 : 
	inline DECL_FUNC_T(void, destroy_Common)(
		_X_ void);

	// 설명 : 
	inline DECL_FUNC(OnD3D10SwapchainResized)(
		_In_ ID3D10Device * _d3dDevice,
		_In_ IDXGISwapChain * _swapChain,
		_In_ const DXGI_SURFACE_DESC & _backBufferSurfaceDesc,
		_Inout_ void * _userContext);

	// 설명 : 
	inline IMPL_FUNC_T(void, OnMouse_GUI)(
		_In_ const short * _buttonsDown,
		_In_ unsigned int _buttonCount,
		_In_ int _mouseWheelDelta,
		_In_ int _xPos,
		_In_ int _yPos,
		_Inout_ void * _userContext);

	// 설명 : 
	extern frame::inputEventHelper g_GUIHelper;

}


#include "common.inl"