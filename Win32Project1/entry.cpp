#include "entry.h"



//--------------------------------------------------------------------------------------
// Grobal Variable
//--------------------------------------------------------------------------------------

// 설명 : 
frame::Container g_GUI_Background0;

// 설명 : 
frame::inputEventHelper g_GUI_Helper0(&g_GUI_Background0);

// 설명 :
FMOD::Sound * g_Sound_Background0;

// 설명 : 
const D3DXMATRIX g_World = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f };

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
		g_GUI_Background0.set_W((float)LOWORD(_lParam));
		g_GUI_Background0.set_H((float)HIWORD(_lParam));
		g_GUI_Background0.reform();
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
		// gui
		g_GUI_Background0.graphics()->set_image(L"image/background/entry.png");
		g_GUI_Background0.set_X(0.0f);
		g_GUI_Background0.set_Y(0.0f);
		g_GUI_Background0.set_W((float)_backBufferSurfaceDesc.Width);
		g_GUI_Background0.set_H((float)_backBufferSurfaceDesc.Height);
		g_GUI_Background0.reform();
		g_GUI_Background0.set_Visible(true);

		// sound
		sound::g_FMOD_SoundDevice.load_WaveFile(
			L"sound/main_title.wav",
			&g_Sound_Background0);
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

	g_GUI_Background0.render();
}

//--------------------------------------------------------------------------------------
DECL_FUNC_T(void, entry::OnD3D10DestroyDevice)(
	_Inout_ void * _userContext)
{
	g_GUI_Background0.clear();
}