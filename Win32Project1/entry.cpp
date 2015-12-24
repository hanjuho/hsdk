#include "entry.h"
#include "entrycompofactory.h"
#include "game.h"


//--------------------------------------------------------------------------------------
// Grobal Variable
//--------------------------------------------------------------------------------------

// 설명 : 
common::GUI_Target g_GUI_Entry;

// 설명 :
FMOD::Sound * g_Sound_Background0;
FMOD::Channel * g_Sound_Controller0;

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, entryloop::OnFrameMove)(
	_In_ double _fTime,
	_In_ float _fElapsedTime,
	_Inout_ void * _userContext)
{
	sound::g_FMOD_SoundDevice.play();
	g_GUI_Entry.update();
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, entryloop::OnD3D10FrameRender)(
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

	g_GUI_Entry.render();
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(LRESULT, entryloop::OnMsgProc)(
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
		g_GUI_Entry.redraw();
	}

	return 0;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, entryloop::OnMouse)(
	_In_ const short * _buttonsDown,
	_In_ unsigned int _buttonCount,
	_In_ int _mouseWheelDelta,
	_In_ int _xPos,
	_In_ int _yPos,
	_Inout_ void * _userContext)
{
	common::OnMouse_GUI(
		_buttonsDown,
		_buttonCount,
		_mouseWheelDelta,
		_xPos, _yPos,
		_userContext);
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, entryloop::OnKeyboard)(
	_In_ unsigned char _nKey,
	_In_ short _bKeyDown,
	_In_ short _bAltDown,
	_Inout_ void * _userContext)
{

}

//--------------------------------------------------------------------------------------
IMPL_FUNC(entryloop::OnD3D10CreateDevice)(
	_In_ ID3D10Device * _d3dDevice,
	_In_ const DXGI_SURFACE_DESC & _backBufferSurfaceDesc,
	_Inout_ void * _userContext)
{
	HRESULT hr = E_FAIL;
	IF_SUCCEEDED(hr = common::initialize_Common(&g_GUI_Entry))
	{
		// layout
		EntryCompoFactory factory;

		factory.build_EntryGUI(&g_GUI_Entry,
			(float)_backBufferSurfaceDesc.Width,
			(float)_backBufferSurfaceDesc.Height);

		g_GUI_Entry.reform();

		// sound
		IF_FAILED(hr = sound::g_FMOD_SoundDevice.load_WaveFile(
			&g_Sound_Background0,
			&g_Sound_Controller0,
			L"sound/entry_background.wav"))
		{
			return hr;
		}

		FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

		g_Sound_Controller0->setMode(FMOD_LOOP_NORMAL);
		g_Sound_Controller0->set3DAttributes(&pos, &vel);
	}

	return hr;
}

//--------------------------------------------------------------------------------------
DECL_FUNC_T(void, entryloop::OnD3D10DestroyDevice)(
	_Inout_ void * _userContext)
{
	common::destroy_Common();
	g_GUI_Entry.reset();
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(entryloop::initialize_Entry)(
	_In_ void * _context)
{
	if (framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc)
	{
		framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc(
			framework::g_Framework_Callbacks.d3d10DeviceDestroyedFuncUserContext);
	}

	framework::g_Framework_Callbacks.windowMsgFunc = entryloop::OnMsgProc;
	framework::g_Framework_Callbacks.mouseFunc = entryloop::OnMouse;
	framework::g_Framework_Callbacks.keyboardFunc = entryloop::OnKeyboard;
	framework::g_Framework_Callbacks.frameMoveFunc = entryloop::OnFrameMove;
	framework::g_Framework_Callbacks.d3d10DeviceCreatedFunc = entryloop::OnD3D10CreateDevice;
	framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc = entryloop::OnD3D10DestroyDevice;
	framework::g_Framework_Callbacks.d3d10FrameRenderFunc = entryloop::OnD3D10FrameRender;

	if (framework::g_Framework_Callbacks.d3d10DeviceCreatedFunc)
	{
		IF_FAILED(framework::g_Framework_Callbacks.d3d10DeviceCreatedFunc(
			framework::g_Framework_Device.d3d10Device,
			framework::g_Framework_Device.dxgiBackBuffer_Desc,
			_context))
		{
			MessageBox(framework::g_Framework_Window.hwnd, L"초기화에 실패했습니다.\n코드를 확인해주세요", L"Error", MB_OK);
			return E_FAIL;
		}
	}

	return S_OK;
}