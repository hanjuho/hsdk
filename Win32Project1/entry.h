#pragma once



#include "common.h"



namespace entry
{

	// 설명 : 
	DECL_FUNC_T(LRESULT, OnMsgProc)(
		_Out_opt_ BOOL * _bNoFurtherProcessing,
		_In_ HWND _hWnd,
		_In_ UINT _uMsg,
		_In_ WPARAM _wParam,
		_In_ LPARAM _lParam,
		_Inout_ void * _userContext);

	// 설명 : 
	DECL_FUNC_T(void, OnMouse)(
		_In_ const short * _buttonsDown,
		_In_ unsigned int _buttonCount,
		_In_ int _mouseWheelDelta,
		_In_ int _xPos,
		_In_ int _yPos,
		_Inout_ void * _userContext);

	// 설명 : 
	DECL_FUNC_T(void, OnKeyboard)(
		_In_ unsigned char _nKey,
		_In_ short _bKeyDown,
		_In_ short _bAltDown,
		_Inout_ void * _userContext);

	// 설명 : 
	DECL_FUNC_T(void, OnFrameMove)(
		_In_ double _fTime,
		_In_ float _fElapsedTime,
		_Inout_ void * _userContext);

	// 설명 : 
	DECL_FUNC_T(BOOL, ModifyDeviceSettings)(
		_In_ const D3D10_DEVICE_DESC & _deviceSettings,
		_Inout_ void * _userContext);

	// 설명 : 
	DECL_FUNC_T(BOOL, IsD3D10DeviceAcceptable)(
		_In_ unsigned int _adapter,
		_In_ unsigned int _output,
		_In_ D3D10_DRIVER_TYPE _deviceType,
		_In_ DXGI_FORMAT _backBufferFormat,
		_In_ BOOL _windowed,
		_Inout_ void * _userContext);

	// 설명 : 
	DECL_FUNC(OnD3D10CreateDevice)(
		_In_ ID3D10Device * _d3dDevice,
		_In_ const DXGI_SURFACE_DESC & _backBufferSurfaceDesc,
		_Inout_ void * _userContext);
	
	// 설명 : 
	DECL_FUNC_T(void, OnD3D10FrameRender)(
		_In_ ID3D10Device * _d3dDevice,
		_In_ double _fTime,
		_In_ float _fElapsedTime,
		_Inout_ void * _userContext);

	// 설명 : 
	DECL_FUNC_T(void, OnD3D10DestroyDevice)(
		_Inout_ void * _userContext);

}