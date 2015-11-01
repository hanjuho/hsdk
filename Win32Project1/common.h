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

	// ���� : 
	inline DECL_FUNC(initialize_Common)(
		_X_ void);

	// ���� : 
	inline DECL_FUNC_T(void, destroy_Common)(
		_X_ void);

	// ���� : 
	inline DECL_FUNC(OnD3D10SwapchainResized)(
		_In_ ID3D10Device * _d3dDevice,
		_In_ IDXGISwapChain * _swapChain,
		_In_ const DXGI_SURFACE_DESC & _backBufferSurfaceDesc,
		_Inout_ void * _userContext);



	// ���� : mouse event�� ���� �� �ִ� �������̽��� �߻�ȭ.
	DECL_INTERFACE(MouseableAdapter)
		: public i::frame::i_Mouseable
	{

		// ���� �Ҹ���
		CLASS_DECL_DESTRUCTOR(MouseableAdapter)(void)
		{

		}

		// ���� :
		INTERFACE_DECL_FUNC_T(void, onMouse_Enter)(
			_In_ int _x,
			_In_ int _y){}

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, onMouse_Exit)(
			_In_ int _x,
			_In_ int _y){}

		// ���� : mouse�� ��ư�� ������ �� �߻��ϴ� event.
		INTERFACE_DECL_FUNC_T(void, onClick_Down)(
			_In_ i::frame::MOUSE_BUTTON _button,
			_In_ int _x,
			_In_ int _y){}

		// ���� : mouse�� ��ư�� �����ٰ� ���� �߻��ϴ� event.
		INTERFACE_DECL_FUNC_T(void, onClick_Up)(
			_In_ i::frame::MOUSE_BUTTON _button,
			_In_ int _x,
			_In_ int _y){}

		// ���� : mouse�� ��ư�� ���� ä Ŀ���� �̵��ϸ� �߻��ϴ� event.
		INTERFACE_DECL_FUNC_T(void, onDrag)(
			_In_ i::frame::MOUSE_BUTTON _button,
			_In_ int _x,
			_In_ int _y){}

		// ���� : mouse�� Ŀ���� �̵���Ű�� �߻��ϴ� event.
		INTERFACE_DECL_FUNC_T(void, onMove)(
			_In_ int _x,
			_In_ int _y){}

		// ���� : mouse�� wheel�� �����ϸ� �߻��ϴ� event.
		INTERFACE_DECL_FUNC_T(void, onWheel)(
			_In_ int _x,
			_In_ int _y,
			_In_ int _w){}

	};

}


#include "common.inl"