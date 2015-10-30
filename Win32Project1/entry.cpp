#include "entry.h"
#include "game.h"
#include <hsdk/win/frame/rendertargetcontainer.h>
#include <hsdk/win/frame/layout/borderlayout.h>


class ToGameButtonEvent :
	public common::MouseableAdapter
{
public:

	INTERFACE_DECL_FUNC_T(void, onClick_Up)(
		_In_ i::frame::MOUSE_BUTTON _button,
		_In_ int _x,
		_In_ int _y)
	{
		game::initialize();
	}

};

//--------------------------------------------------------------------------------------
// Grobal Variable
//--------------------------------------------------------------------------------------

// 설명 : 
frame::Container g_GUI_Entry;

// 설명 : 
frame::inputEventHelper g_GUIHelper_Entry(&g_GUI_Entry);

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, entry::initialize)(
	_X_ void)
{
	if (framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc)
	{
		framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc(
			framework::g_Framework_Callbacks.d3d10DeviceDestroyedFuncUserContext);
	}

	framework::g_Framework_Callbacks.windowMsgFunc = entry::OnMsgProc;
	framework::g_Framework_Callbacks.mouseFunc = entry::OnMouse;
	framework::g_Framework_Callbacks.keyboardFunc = entry::OnKeyboard;
	framework::g_Framework_Callbacks.frameMoveFunc = entry::OnFrameMove;

	framework::g_Framework_Callbacks.d3d10DeviceCreatedFunc = entry::OnD3D10CreateDevice;
	framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc = entry::OnD3D10DestroyDevice;
	framework::g_Framework_Callbacks.d3d10FrameRenderFunc = entry::OnD3D10FrameRender;

	if (framework::g_Framework_Callbacks.d3d10DeviceCreatedFunc)
	{
		IF_FAILED(framework::g_Framework_Callbacks.d3d10DeviceCreatedFunc(
			framework::g_Framework_Device.d3d10Device,
			framework::g_Framework_Device.dxgiBackBuffer_Desc,
			framework::g_Framework_Callbacks.d3d10DeviceCreatedFuncUserContext))
		{
			MessageBox(framework::g_Framework_Window.hwnd, L"초기화에 실패했습니다.\n코드를 확인해주세요", L"Error", MB_OK);
			framework::g_Framework.shutdown();
		}
	}
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(LRESULT, entry::OnMsgProc)(
	_Out_opt_ BOOL * _bNoFurtherProcessing,
	_In_ HWND _hWnd,
	_In_ UINT _uMsg,
	_In_ WPARAM _wParam,
	_In_ LPARAM _lParam,
	_Inout_ void * _userContext)
{
	switch (_uMsg)
	{
	case WM_SIZE:
		g_GUI_Entry.set_W((float)LOWORD(_lParam));
		g_GUI_Entry.set_H((float)HIWORD(_lParam));
		g_GUI_Entry.reform();
	}

	return 0;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, entry::OnMouse)(
	_In_ const short * _buttonsDown,
	_In_ unsigned int _buttonCount,
	_In_ int _mouseWheelDelta,
	_In_ int _xPos,
	_In_ int _yPos,
	_Inout_ void * _userContext)
{
	if (_buttonsDown[FRAMEWORK_LEFTBUTTON] == 1)
	{
		g_GUI_Entry.onClick_Down(i::frame::LBUTTON, _xPos, _yPos);
	}
	else if (_buttonsDown[FRAMEWORK_LEFTBUTTON] == 2)
	{
		g_GUI_Entry.onClick_Up(i::frame::LBUTTON, _xPos, _yPos);
	}

	if (_buttonsDown[FRAMEWORK_MIDDLEBUTTON])
	{
		g_GUI_Entry.onClick_Down(i::frame::WBUTTON, _xPos, _yPos);
	}
	else if (_buttonsDown[FRAMEWORK_MIDDLEBUTTON] == 2)
	{
		g_GUI_Entry.onClick_Up(i::frame::LBUTTON, _xPos, _yPos);
	}

	if (_buttonsDown[FRAMEWORK_RIGHTBUTTON])
	{
		g_GUI_Entry.onClick_Down(i::frame::RBUTTON, _xPos, _yPos);
	}
	else if (_buttonsDown[FRAMEWORK_RIGHTBUTTON] == 2)
	{
		g_GUI_Entry.onClick_Up(i::frame::LBUTTON, _xPos, _yPos);
	}
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, entry::OnKeyboard)(
	_In_ unsigned char _nKey,
	_In_ short _bKeyDown,
	_In_ short _bAltDown,
	_Inout_ void * _userContext)
{

}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, entry::OnFrameMove)(
	_In_ double _fTime,
	_In_ float _fElapsedTime,
	_Inout_ void * _userContext)
{
	sound::g_FMOD_SoundDevice.play();
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(entry::OnD3D10CreateDevice)(
	_In_ ID3D10Device * _d3dDevice,
	_In_ const DXGI_SURFACE_DESC & _backBufferSurfaceDesc,
	_Inout_ void * _userContext)
{
	HRESULT hr = E_FAIL;

	IF_SUCCEEDED(hr = common::initialize_Common())
	{
		float w = (float)_backBufferSurfaceDesc.Width;
		float h = (float)_backBufferSurfaceDesc.Height;

		// layout
		{
			// 메인 컨테이너
			frame::layout::BorderLayout * borderLayout = new frame::layout::BorderLayout();
			


			g_GUI_Entry.set_Layout(borderLayout);
			g_GUI_Entry.set_Visible(true);
			g_GUI_Entry.set_X(0.0f);
			g_GUI_Entry.set_Y(0.0f);
			g_GUI_Entry.set_W(w);
			g_GUI_Entry.set_H(h);
			g_GUI_Entry.graphics()->set_image(L"image/background/entry.png");

			// 상태 정보 창
			frame::RenderTargetContainer stateLayout(0.0f, 0.0f);

		}
		// gui
		g_GUI_Background0.reform();

		frame::Component * pannel = new frame::Component(
			0, 0, w * 0.6f, h * 0.6f);
		pannel->graphics()->set_image(L"image/layout/notepad.png");
		pannel->set_Visible(true);

		frame::Component * button = new frame::ButtonCompo(
			0, 0, w * 0.15f, h * 0.07f);

		button->set_Mouseable(new ToGameButtonEvent());
		button->graphics()->set_image(L"image/layout/button.png");
		button->set_Visible(true);

		g_GUI_Background0.reform();

		container.add_Component(pannel);
		container.add_Component(button);
		container.reform();
		container.set_Visible(true);

		// sound
		IF_SUCCEEDED(hr = sound::g_FMOD_SoundDevice.load_WaveFile(
			&g_Sound_Background0,
			&g_Sound_Controller0,
			L"sound/entry_background.wav"))
		{
			FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
			FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

			g_Sound_Controller0->setMode(FMOD_LOOP_NORMAL);
			g_Sound_Controller0->set3DAttributes(&pos, &vel);
		}
	}

	return hr;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, entry::OnD3D10FrameRender)(
	_In_ ID3D10Device * _d3dDevice,
	_In_ double _fTime,
	_In_ float _fElapsedTime,
	_Inout_ void * _userContext)
{
	// Clear the render target and depth stencil
	float ClearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	ID3D10RenderTargetView * pRTV = framework::g_Framework_Device.d3d10RTV;
	_d3dDevice->ClearRenderTargetView(pRTV, ClearColor);
	ID3D10DepthStencilView * pDSV = framework::g_Framework_Device.d3d10DSV;
	_d3dDevice->ClearDepthStencilView(pDSV, D3D10_CLEAR_DEPTH, 1.0, 0);

	container.render();
	// g_GUI_Background0.render();
}

//--------------------------------------------------------------------------------------
DECL_FUNC_T(void, entry::OnD3D10DestroyDevice)(
	_Inout_ void * _userContext)
{
	common::destroy_Common();

	container.clear();
	g_GUI_Background0.clear();
}