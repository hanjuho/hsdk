#pragma once



#include <hsdk/win/framework.h>
#include <hsdk/win/framework_camera.h>

#include <hsdk/win/frame/container.h>
#include <hsdk/win/frame/buttoncompo.h>
#include <hsdk/win/frame/inputeventhelper.h>
#include <hsdk/win/frame/rendertargetcontainer.h>
#include <hsdk/win/frame/layout/borderlayout.h>
#include <hsdk/win/frame/layout/flowlayout.h>

#include <hsdk/win/direct3d/d3d10_factory.h>
#include <hsdk/win/direct3d/d3d10_mesh.h>
#include <hsdk/win/direct3d/d3d10_renderer.h>
#include <hsdk/win/direct3d/d3d10_rendertarget.h>

#include <hsdk/win/sound/fmod_sounddevice.h>



using namespace hsdk;



namespace common
{

	// 설명 : 
	inline DECL_FUNC(initialize_Common)(
		_X_ void);

	// 설명 : 
	inline DECL_FUNC_T(void, destroy_Common)(
		_X_ void);

	// 설명 : 
	inline DECL_FUNC(OnD3D10SwapchainResized)(
		_In_ ID3D10Device * _d3dDevice,
		_In_ IDXGISwapChain * _swapChain,
		_In_ const DXGI_SURFACE_DESC & _backBufferSurfaceDesc,
		_Inout_ void * _userContext);



	// 설명 : mouse event를 받을 수 있는 인터페이스를 추상화.
	DECL_INTERFACE(MouseableAdapter)
		: public i::frame::i_Mouseable
	{

		// 가상 소멸자
		CLASS_DECL_DESTRUCTOR(MouseableAdapter)(void)
		{

		}

		// 설명 :
		INTERFACE_DECL_FUNC_T(void, onMouse_Enter)(
			_In_ int _x,
			_In_ int _y){}

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, onMouse_Exit)(
			_In_ int _x,
			_In_ int _y){}

		// 설명 : mouse의 버튼이 눌렸을 때 발생하는 event.
		INTERFACE_DECL_FUNC_T(void, onClick_Down)(
			_In_ i::frame::MOUSE_BUTTON _button,
			_In_ int _x,
			_In_ int _y){}

		// 설명 : mouse의 버튼이 눌렀다가 띄우면 발생하는 event.
		INTERFACE_DECL_FUNC_T(void, onClick_Up)(
			_In_ i::frame::MOUSE_BUTTON _button,
			_In_ int _x,
			_In_ int _y){}

		// 설명 : mouse의 버튼을 누른 채 커서를 이동하면 발생하는 event.
		INTERFACE_DECL_FUNC_T(void, onDrag)(
			_In_ i::frame::MOUSE_BUTTON _button,
			_In_ int _x,
			_In_ int _y){}

		// 설명 : mouse의 커서를 이동시키면 발생하는 event.
		INTERFACE_DECL_FUNC_T(void, onMove)(
			_In_ int _x,
			_In_ int _y){}

		// 설명 : mouse의 wheel을 조작하면 발생하는 event.
		INTERFACE_DECL_FUNC_T(void, onWheel)(
			_In_ int _x,
			_In_ int _y,
			_In_ int _w){}

	};

}


#include "common.inl"