#include <hsdk/win/frame/direct3d/direct3d.h>
#include <hsdk/win/frame/direct3d/direct3doutside.h>
#include <hsdk/win/wintimer.h>



using namespace hsdk;
using namespace direct3d;


//--------------------------------------------------------------------------------------
// Grobal variable
//--------------------------------------------------------------------------------------

// 설명 : 
Direct3D g_Direct3D;

// 설명 : 
Direct3D_Outside g_Outside;

//--------------------------------------------------------------------------------------
// Grobal declare
//--------------------------------------------------------------------------------------

// 설명 : 
DECL_STRUCT(Direct3D_TimeStream)
{

	// 설명 : 
	bool pauseTime = false;

	// 설명 : 
	bool pauseRendering = false;

	// 설명 : 
	int pauseTimeCount = 0;

	// 설명 : 
	int pauseRenderingCount = 0;

	// 설명 : 
	bool constantFrameTime = false;

	// 설명 : 
	float constantTimePerFrame = 0.0333f;

	// 설명 :
	win::WINTimer timer;

};

// 설명 : 
DECL_STRUCT(Direct3DState)
{

};

//--------------------------------------------------------------------------------------
// Grobal thread safety
//--------------------------------------------------------------------------------------

// 설명 : 
CRITICAL_SECTION g_cs;

// 설명 : 
bool g_bThreadSafe = true;

//--------------------------------------------------------------------------------------
// Grobal callback variable
//--------------------------------------------------------------------------------------

// 설명 : 
Direct3D_Callbacks g_Callbacks;

// 설명 : 
Direct3D_Callback_UserContexts g_cbUserContext;

//--------------------------------------------------------------------------------------
// initialize task variable
//--------------------------------------------------------------------------------------

// 설명 : 
Direct3D_INIT_DESC g_init;

//--------------------------------------------------------------------------------------
// Grobal control variable
//--------------------------------------------------------------------------------------

// 설명 : 
long g_ExitCode;

// 설명 : array of key state
bool g_Keys[256];

// 설명 : array of last key state
bool g_LastKeys[256];

// 설명 : array of mouse states
bool g_MouseButtons[5];

// 설명 : 
Direct3D_TimeStream g_TimeStream;

//--------------------------------------------------------------------------------------
// Grobal device handle variable
//--------------------------------------------------------------------------------------

// 설명 : 
Direct3D_Window g_Window;

// 설명 : 
Direct3D_Device g_Device;

// 설명 : 
Direct3D_DeviceDescs g_DeviceDescs;

//--------------------------------------------------------------------------------------
// Grobal function declare
//--------------------------------------------------------------------------------------

// 설명 : Creates the 3D environment in g_Device
DECL_FUNC(create_3DEnvironment9)(
	/* [x] */ void);

/*
설명 : Resets the 3D environment in g_Device by:
- Calls the device lost callback
- Resets the device
- Stores the back buffer description
- Sets up the full screen Direct3D cursor if requested
- Calls the device reset callback
*/
DECL_FUNC(reset_3DEnvironment9)(
	/* [x] */ void);

/*
설명: Render the 3D environment in g_Device by :
- Checking if the device is lost and trying to reset it if it is
- Get the elapsed time since the last frame
- Calling the app's framemove and render callback
- Calling Present()
*/
DECL_FUNC_T(void, render3DEnvironment9)(
	/* [x] */ void);

/*
설명: Cleans up the 3D environment in g_Device by :
- Calls the device lost callback
- Calls the device destroyed callback
- Releases the D3D device
*/
DECL_FUNC_T(void, cleanup_3DEnvironment9)(
	/* [r] */ bool _releaseSettings);

// 설명 : Setup cursor based on current settings (window/fullscreen mode, show cursor state, clip cursor state)
DECL_FUNC(setup_DeviceCursor)(
	/* [r] */ DEVICE_VERSION _version);

// 설명 : Gives the D3D device a cursor with image and hotspot from hCursor.
DECL_FUNC(set_D3D9DeviceCursor)(
	IDirect3DDevice9* _d3dDevice,
	HCURSOR _hCursor,
	bool _addWatermark);

// 설명 : Creates the 3D environment
DECL_FUNC(create_3DEnvironment10)(
	/* [x] */ void);

/*
설명 : Resets the 3D environment in g_Device by:
- Calls the device lost callback
- Resets the device
- Stores the back buffer description
- Sets up the full screen Direct3D cursor if requested
- Calls the device reset callback
*/
DECL_FUNC(reset_3DEnvironment10)(
	/* [x] */ void);

/*
설명 : Cleans up the 3D environment in g_Device by:
- Calls the device lost callback
- Calls the device destroyed callback
- Releases the D3D device
*/
DECL_FUNC_T(void, cleanup_3DEnvironment10)(
	/* [r] */ bool _releaseSettings = true);

DECL_FUNC_T(void, render_3DEnvironment10)(
	/* [x] */ void);

// 설명 : Sets the viewport, creates a render target view, and depth scencil texture and view in g_Device.
DECL_FUNC(setup_D3D10Views)(
	/* [r] */ const D3D10_DEVICE_DESC & _desc);

// 설명 : resize back buffer surface desc in g_Device.10
DECL_FUNC_T(void, resize_DXGIBuffers)(
	/* [r] */ unsigned int _width,
	/* [r] */ unsigned int _height,
	/* [r] */ bool _fullScreen);

// 설명 : Stores back buffer surface desc in g_Device
DECL_FUNC(update_BackBufferDesc)(
	/* [r] */ DEVICE_VERSION _version);

void updateD3D9DeviceStats(
	D3DDEVTYPE DeviceType,
	unsigned long BehaviorFlags,
	D3DADAPTER_IDENTIFIER9* pAdapterIdentifier);

HRESULT DXUTFindD3D9AdapterFormat(
	unsigned int AdapterOrdinal,
	D3DDEVTYPE DeviceType,
	D3DFORMAT BackBufferFormat,
	BOOL Windowed,
	D3DFORMAT * pAdapterFormat);


void create_D3D10Counters(ID3D10Device* pd3dDevice);
void destroy_D3D10Counters();
void startPerformanceCounters();
void stopPerformanceCounters();
void updateD3D10CounterStats();

HRESULT setupD3D10Views(ID3D10Device* pd3dDevice, DXUTDeviceSettings* pDeviceSettings);

void updateD3D10DeviceStats(
	D3D10_DRIVER_TYPE DeviceType,
	DXGI_ADAPTER_DESC* pAdapterDesc);

/*
설명 : Checks to see if the HWND changed monitors, and if it did it creates a device 
from the monitor's adapter and recreates the scene.
*/
DECL_FUNC_T(void, checkFor_WindowChangingMonitors)(
	/* [x] */ void);

// 설명 : Returns the HMONITOR attached to an adapter/output
DECL_FUNC_T(HMONITOR, get_MonitorFromAdapter)(
	/* [r] */ const Direct3D_DeviceDescs & _desc);

// 설명 : Look for an adapter ordinal that is tied to a HMONITOR
DECL_FUNC(get_AdapterOrdinalFromMonitor)(
	/* [w] */ unsigned int * _adapterOrdinal,
	/* [r] */ HMONITOR _monitor);

// 설명 : Handles window messages 
LRESULT CALLBACK direct3d_WndProc(
	/* [r] */ HWND _hWnd,
	/* [r] */ unsigned int _uMsg,
	/* [r] */ unsigned int _wParam,
	/* [r] */ long _lParam);

//--------------------------------------------------------------------------------------
// General callbacks
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_FrameMove)(
	/* [ref] */ CALLBACK_FRAMEMOVE _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.frameMoveFunc = _callback;
	g_cbUserContext.frameMoveFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_Keyboard)(
	/* [ref] */ CALLBACK_KEYBOARD _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.keyboardFunc = _callback;
	g_cbUserContext.keyboardFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_Mouse)(
	/* [ref] */ CALLBACK_MOUSE _callback,
	/* [r] */ bool _includeMouseMove,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.mouseFunc = _callback;
	g_cbUserContext.notifyOnMouseMove = _includeMouseMove;
	g_cbUserContext.mouseFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_MsgProc)(
	/* [ref] */ CALLBACK_MSGPROC _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.windowMsgFunc = _callback;
	g_cbUserContext.windowMsgFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_DeviceChanging)(
	/* [ref] */ CALLBACK_MODIFY_DEVICE_SETTINGS _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.modifyDeviceSettingsFunc = _callback;
	g_cbUserContext.modifyDeviceSettingsFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_DeviceRemoved)(
	/* [ref] */ CALLBACK_DEVICE_REMOVED _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.deviceRemovedFunc = _callback;
	g_cbUserContext.deviceRemovedFuncUserContext = _userContext;
}

// Direct3D 9 callbacks

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_D3D9_DeviceAcceptable)(
	/* [ref] */ CALLBACK_IS_D3D9_DEVICE_ACCEPTABLE _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.isD3D9DeviceAcceptableFunc = _callback;
	g_cbUserContext.isD3D9DeviceAcceptableFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_D3D9_DeviceCreated)(
	/* [ref] */ CALLBACK_D3D9_DEVICE_CREATED _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d9DeviceCreatedFunc = _callback;
	g_cbUserContext.d3d9DeviceCreatedFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_D3D9_DeviceReset)(
	/* [ref] */ CALLBACK_D3D9_DEVICE_RESET _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d9DeviceResetFunc = _callback;
	g_cbUserContext.d3d9DeviceResetFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_D3D9_DeviceLost)(
	/* [ref] */ CALLBACK_D3D9_DEVICE_LOST _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d9DeviceLostFunc = _callback;
	g_cbUserContext.d3d9DeviceLostFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_D3D9_DeviceDestroyed)(
	/* [ref] */ CALLBACK_D3D9_DEVICE_DESTROYED _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d9DeviceDestroyedFunc = _callback;
	g_cbUserContext.d3d9DeviceDestroyedFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_D3D9_FrameRender)(
	/* [ref] */ CALLBACK_D3D9_FRAME_RENDER _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d9FrameRenderFunc = _callback;
	g_cbUserContext.d3d9FrameRenderFuncUserContext = _userContext;
}


// Direct3D 10 callbacks

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_D3D10_DeviceAcceptable)(
	/* [ref] */ CALLBACK_IS_D3D10_DEVICE_ACCEPTABLE _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.isD3D10DeviceAcceptableFunc = _callback;
	g_cbUserContext.isD3D10DeviceAcceptableFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_D3D10_DeviceCreated)(
	/* [ref] */ CALLBACK_D3D10_DEVICE_CREATED _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d10DeviceCreatedFunc = _callback;
	g_cbUserContext.d3d10DeviceCreatedFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_D3D10_SwapChainResized)(
	/* [ref] */ CALLBACK_D3D10_SWAPCHAINRESIZED _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d10SwapChainResizedFunc = _callback;
	g_cbUserContext.d3d10SwapChainResizedFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_D3D10_SwapChainReleasing)(
	/* [ref] */ CALLBACK_D3D10_SWAPCHAINRELEASING _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d10SwapChainReleasingFunc = _callback;
	g_cbUserContext.d3d10SwapChainReleasingFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_D3D10_DeviceDestroyed)(
	/* [ref] */ CALLBACK_D3D10_DEVICE_DESTROYED _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d10DeviceDestroyedFunc = _callback;
	g_cbUserContext.d3d10DeviceDestroyedFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_Callback_D3D10_FrameRender)(
	/* [ref] */ CALLBACK_D3D10_FRAME_RENDER _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d10FrameRenderFunc = _callback;
	g_cbUserContext.d3d10FrameRenderFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
// initialize task
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Direct3D, initialize_Default)(
	/* [r] */ bool _parseCommandLine,
	/* [r] */ bool _showMsgBoxOnError,
	/* [r] */ __in_opt wchar_t * _strExtraCommandLineParams,
	/* [r] */ bool _threadSafe)
{
	g_bThreadSafe = _threadSafe;

	// Not always needed, but lets the app create GDI dialogs
	InitCommonControls();

	// Save the current sticky/toggle/filter key settings so DXUT can restore them later
	g_init.startupStickyKeys = { sizeof(STICKYKEYS), 0 };
	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &g_init.startupStickyKeys, 0);

	g_init.startupToggleKeys = { sizeof(TOGGLEKEYS), 0 };
	SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_init.startupToggleKeys, 0);

	g_init.startupFilterKeys = { sizeof(FILTERKEYS), 0 };
	SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &g_init.startupFilterKeys, 0);

	if (_parseCommandLine)
	{
		parse_CommandLine(g_init, GetCommandLine(), true);
	}

	if (_strExtraCommandLineParams)
	{
		parse_CommandLine(g_init, _strExtraCommandLineParams, false);
	}

	// Declare this process to be high DPI aware, and prevent automatic scaling
	// Warning: This is better done as a <dpiaware> manifest element to avoid
	//			problems based on code that has run before this point.
	HINSTANCE hUser32 = LoadLibrary(L"user32.dll");
	if (hUser32)
	{
		typedef BOOL(WINAPI* LPSetProcessDPIAware)(void);
		LPSetProcessDPIAware pSetProcessDPIAware =
			(LPSetProcessDPIAware)GetProcAddress(hUser32, "SetProcessDPIAware");

		if (pSetProcessDPIAware)
		{
			pSetProcessDPIAware();
		}

		FreeLibrary(hUser32);
	}

	// Reset the timer
	g_TimeStream.timer.reset();

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Direct3D, initialize_Window)(
	/* [r] */ const wchar_t * _strWindowTitle = L"Direct3D Window",
	/* [r] */ HINSTANCE _hInstance = nullptr,
	/* [r] */ HICON _hIcon = nullptr,
	/* [r] */ HMENU _hMenu = nullptr,
	/* [r] */ int _x = CW_USEDEFAULT,
	/* [r] */ int _y = CW_USEDEFAULT)
{
	if (_hInstance == nullptr)
	{
		_hInstance = (HINSTANCE)GetModuleHandle(nullptr);
	}

	WCHAR szExePath[MAX_PATH];
	GetModuleFileName(nullptr, szExePath, MAX_PATH);

	// If the icon is nullptr, then use the first one found in the exe
	if (_hIcon == nullptr)
	{
		_hIcon = ExtractIcon(_hInstance, szExePath, 0);
	}

	// Register the windows class
	WNDCLASS wndClass;
	wndClass.style = CS_DBLCLKS;
	wndClass.lpfnWndProc = direct3d_WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = _hInstance;
	wndClass.hIcon = _hIcon;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = L"Direct3DWindowClass";

	if (!RegisterClass(&wndClass))
	{
		unsigned long dwError = GetLastError();
		if (dwError != ERROR_CLASS_ALREADY_EXISTS)
		{
			return ADD_FLAG(HSDK_FAIL, dwError);
		}
	}

	// Override the window's initial & size position if there were cmd line args
	if (g_init.startX != -1)
	{
		_x = g_init.startX;
	}

	if (g_init.startY != -1)
	{
		_y = g_init.startY;
	}

	// Find the window's initial size, but it might be changed later
	long nDefaultWidth = 640;
	long nDefaultHeight = 480;

	if (g_init.width != 0)
	{
		nDefaultWidth = g_init.width;
	}

	if (g_init.height != 0)
	{
		nDefaultHeight = g_init.height;
	}

	RECT rc;
	SetRect(&rc, 0, 0, nDefaultWidth, nDefaultHeight);
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, (_hMenu != nullptr) ? true : false);

	wchar_t * strCachedWindowTitle = g_Window.windowTitle;
	wcscpy_s(strCachedWindowTitle, 256, _strWindowTitle);

	// Create the render window
	HWND hWnd = CreateWindow(
		L"Direct3DWindowClass",
		strCachedWindowTitle,
		WS_OVERLAPPEDWINDOW,
		_x,
		_y,
		(rc.right - rc.left),
		(rc.bottom - rc.top),
		0,
		_hMenu,
		_hInstance,
		0);

	if (hWnd == nullptr)
	{
		unsigned long dwError = GetLastError();
		return ADD_FLAG(HSDK_FAIL, dwError);
	}

	return userSet_Window(hWnd, hWnd, hWnd);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Direct3D, userSet_Window)(
	/* [in] */ HWND _focus,
	/* [in] */ HWND _deviceFullScreen,
	/* [in] */ HWND _deviceWindowed,
	/* [r] */ bool _handleMessages)
{
	// To avoid confusion, we do not allow any HWND to be nullptr here.  The
	// caller must pass in valid HWND for all three parameters.  The same
	// HWND may be used for more than one parameter.
	if (_focus == nullptr || _deviceFullScreen == nullptr || _deviceWindowed == nullptr)
	{
		return E_INVALIDARG;
	}

	// If subclassing the window, set the pointer to the local window procedure
	if (_handleMessages)
	{
		// Switch window procedures
		LONG_PTR nResult = SetWindowLongPtr(_focus, GWLP_WNDPROC, (LONG_PTR)direct3d_WndProc);

		unsigned long dwError = GetLastError();
		if (nResult == 0)
		{
			return ADD_FLAG(HSDK_FAIL, HRESULT_FROM_WIN32(dwError));
		}
	}

	wchar_t * strCachedWindowTitle = g_Window.windowTitle;
	GetWindowText(_focus, strCachedWindowTitle, 255);
	strCachedWindowTitle[255] = 0;

	g_Window.HINSTANCE = (HINSTANCE)(LONG_PTR)GetWindowLongPtr(_focus, GWLP_HINSTANCE);
	g_Window.focus = _focus;
	g_Window.deviceFullScreen = _deviceFullScreen;
	g_Window.deviceWindowed = _deviceWindowed;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Direct3D, dynamic_WndProc)(
	/* [r] */ HWND _hWnd,
	/* [r] */ unsigned int _uMsg,
	/* [r] */ unsigned int _wParam,
	/* [r] */ long _lParam)
{
	return direct3d_WndProc(_hWnd, _uMsg, _wParam, _lParam);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Direct3D, initialize_Device)(
	/* [r] */ bool _windowed,
	/* [r] */ int _suggestedWidth,
	/* [r] */ int _suggestedHeight)
{
	Direct3D_DeviceDescs descs;
	descs.version = D3D9_DEVICE;

	Direct3D_DeviceMatchOptions deviceMatOpt = create_DeviceMatOpt_FrominitDesc(g_init);
	if (deviceMatOpt.eResolution == IGNORE_INPUT)
	{
		if (_windowed || (_suggestedWidth != 0 && _suggestedHeight != 0))
		{
			deviceMatOpt.eResolution = CLOSEST_TO_INPUT;
		}
	}

	// Building D3D9 device settings for mathch options.  These
	// will be converted to D3D10 settings if app can use D3D10	
	descs.d3d9DeviceDesc = create_DeviceDesc_FrominitDesc(
		g_init, _windowed, _suggestedWidth, _suggestedHeight);

	bool bAcceptable = false;
	if (g_init.forceAPI == 9)
	{
		IF_INVALID(get_D3D9())
		{
			direct3D_OutputDebugString(L"Direct3DERR_NODIRECT3D");
			return E_ABORT;
		}

		CALLBACK_IS_D3D9_DEVICE_ACCEPTABLE callback_acceptable =
			g_Callbacks.isD3D9DeviceAcceptableFunc;

		D3DCAPS9 caps;

		HRESULT hr;
		IF_FAILED(hr = get_D3D9_DeviceCaps(caps, descs))
		{
			return hr;
		}

		D3D9_DEVICE_DESC & desc = descs.d3d9DeviceDesc;
		bAcceptable = callback_acceptable(
			caps,
			desc.adapterFormat,
			desc.pp.BackBufferFormat,
			desc.pp.Windowed,
			g_cbUserContext.isD3D9DeviceAcceptableFuncUserContext);
	}
	else if (g_init.forceAPI == 10)
	{
		IF_INVALID(get_DXGIFactory())
		{
			direct3D_OutputDebugString(L"Direct3DERR_NODIRECT3D");
			return E_ABORT;
		}

		descs.version = D3D10_DEVICE;
		convert_DeviceDesc_9to10(descs.d3d10DeviceDesc, descs.d3d9DeviceDesc);

		CALLBACK_IS_D3D10_DEVICE_ACCEPTABLE callback_acceptable =
			g_Callbacks.isD3D10DeviceAcceptableFunc;

		D3D10_DEVICE_DESC & desc = descs.d3d10DeviceDesc;
		bAcceptable = callback_acceptable(
			desc.adapterOrdinal,
			desc.output,
			desc.driverType,
			desc.sd.BufferDesc.Format,
			desc.sd.Windowed,
			g_cbUserContext.isD3D10DeviceAcceptableFuncUserContext);
	}

	IF_FALSE(bAcceptable)
	{
		return E_ABORT;
	}

	// Change to a Direct3D device created from the new device settings.  
	// If there is an existing device, then either reset or recreated the scene
	return userSet_Device(descs);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Direct3D, userSet_Device)(
	/* [r] */ const Direct3D_DeviceDescs & _d3dDescs,
	/* [r] */ bool _preserveInput,
	/* [r] */ bool _clipWindowToSingleAdapter)
{
	const Direct3D_DeviceDescs currentDesc_Temp;;
	memcpy_s((void *)&currentDesc_Temp, 0, &g_DeviceDescs, sizeof(Direct3D_DeviceDescs));

	// Make a copy of the pNewDeviceSettings on the heap
	Direct3D_DeviceDescs newDesc;
	memcpy(&newDesc, &_d3dDescs, sizeof(Direct3D_DeviceDescs));

	HRESULT hr;
	switch (newDesc.version)
	{
	case D3D9_DEVICE:
		IF_INVALID(get_D3D9())
		{
			hr = E_ABORT;
		}
		break;
	case D3D10_DEVICE:
		IF_INVALID(get_DXGIFactory())
		{
			hr = E_ABORT;
		}
		break;
	case NONE_DEVICE:
		hr = E_ABORT;
		break;
	}

	IF_FAILED(hr)
	{
		direct3D_OutputDebugString(L"Direct3DERR_NODIRECT3D");
		return hr;
	}

	// If the ModifyDeviceSettings callback is non-NULL, then call it to let the app 
	// change the settings or reject the device change by returning false.
	CALLBACK_MODIFY_DEVICE_SETTINGS callback_ModifyDeviceSettings = g_Callbacks.modifyDeviceSettingsFunc;
	if (callback_ModifyDeviceSettings)
	{
		bool bContinue = callback_ModifyDeviceSettings(
			newDesc,
			g_cbUserContext.modifyDeviceSettingsFuncUserContext);

		IF_FALSE(bContinue)
		{
			return E_ABORT;
		}
	}

	if (_clipWindowToSingleAdapter)
	{
		if (is_WindowedFromDesc(newDesc))
		{
			// Get the rect of the monitor attached to the adapter
			MONITORINFO miAdapter;
			miAdapter.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(get_MonitorFromAdapter(newDesc), &miAdapter);

			// Get the rect of the monitor attached to the window
			MONITORINFO miWindow;
			miWindow.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(MonitorFromWindow(g_Window.focus, MONITOR_DEFAULTTOPRIMARY), &miWindow);

			// Do something reasonable if the BackBuffer size is greater than the monitor size
			int nAdapterMonitorWidth = miAdapter.rcWork.right - miAdapter.rcWork.left;
			int nAdapterMonitorHeight = miAdapter.rcWork.bottom - miAdapter.rcWork.top;

			int nClientWidth = get_WidthFromDesc(newDesc);
			int nClientHeight = get_HeightFromDesc(newDesc);

			// Get the rect of the window
			RECT rcWindow;
			GetWindowRect(g_Window.deviceWindowed, &rcWindow);

			// Make a window rect with a client rect that is the same size as the backbuffer
			RECT rcResizedWindow;
			rcResizedWindow.left = 0;
			rcResizedWindow.right = nClientWidth;
			rcResizedWindow.top = 0;
			rcResizedWindow.bottom = nClientHeight;

			AdjustWindowRect(
				&rcResizedWindow,
				GetWindowLong(g_Window.deviceWindowed, GWL_STYLE),
				g_Window.menu != nullptr);

			int nWindowWidth = rcResizedWindow.right - rcResizedWindow.left;
			int nWindowHeight = rcResizedWindow.bottom - rcResizedWindow.top;

			if (nWindowWidth > nAdapterMonitorWidth)
			{
				nWindowWidth = nAdapterMonitorWidth;
			}
			if (nWindowHeight > nAdapterMonitorHeight)
			{
				nWindowHeight = nAdapterMonitorHeight;
			}

			switch (newDesc.version)
			{
			case D3D9_DEVICE:
				newDesc.d3d9DeviceDesc.pp.BackBufferWidth = nWindowWidth;
				newDesc.d3d9DeviceDesc.pp.BackBufferHeight = nWindowHeight;
				break;
			case D3D10_DEVICE:
				newDesc.d3d10DeviceDesc.sd.BufferDesc.Width = nWindowWidth;
				newDesc.d3d10DeviceDesc.sd.BufferDesc.Height = nWindowHeight;
				break;
			};
		}
	}

	pause(true, true);
	memcpy_s(&g_DeviceDescs, 0, &newDesc, sizeof(Direct3D_DeviceDescs));

	// If API version, AdapterOrdinal and DeviceType are the same, we can just do a Reset().
	// If they've changed, we need to do a complete device tear down/rebuild.
	// Also only allow a reset if pd3dDevice is the same as the current device 
	if (is_ResetDevice(currentDesc_Temp, newDesc))
	{
		hr = E_FAIL;

		// Reset the Direct3D device and call the app's device callbacks
		switch (g_DeviceDescs.version)
		{
		case D3D9_DEVICE:
			hr = reset_3DEnvironment9();
		case D3D10_DEVICE:
			hr = reset_3DEnvironment10();
		}

		IF_FAILED(hr)
		{
			direct3D_OutputDebugString(L"Direct3DERR_RESETTINGDEVICE : reset_3DEnvironment", hr);
			shutdown();

			return hr;
		}
	}
	else
	{
		// Reset the Direct3D device and call the app's device callbacks
		switch (g_DeviceDescs.version)
		{
		case D3D9_DEVICE:
			hr = create_3DEnvironment9();
		case D3D10_DEVICE:
			hr = create_3DEnvironment10();
		}

		IF_FAILED(hr)
		{
			direct3D_OutputDebugString(L"Direct3DERR_RESETTINGDEVICE : create3DEnvironment", hr);
			pause(false, false);

			return hr;
		}
	}

	const Direct3D_DeviceDescs & currentDesc = g_DeviceDescs;
	if (is_WindowedFromDesc(currentDesc))
	{
		// Make a window rect with a client rect that is the same size as the backbuffer
		RECT rcWindow = { 0 };
		rcWindow.right = (long)(get_WidthFromDesc(currentDesc));
		rcWindow.bottom = (long)(get_HeightFromDesc(currentDesc));

		AdjustWindowRect(
			&rcWindow,
			GetWindowLong(g_Window.deviceWindowed, GWL_STYLE),
			g_Window.menu != nullptr);

		// Resize the window.  It is important to adjust the window size 
		// after resetting the device rather than beforehand to ensure 
		// that the monitor resolution is correct and does not limit the size of the new window.
		int cx = (int)(rcWindow.right - rcWindow.left);
		int cy = (int)(rcWindow.bottom - rcWindow.top);

		SetWindowPos(
			g_Window.deviceWindowed,
			0,
			0,
			0,
			cx,
			cy,
			SWP_NOZORDER | SWP_NOMOVE);

	}

	// Need to resize, so if window is maximized or minimized then restore the window
	if (IsIconic(g_Window.deviceWindowed))
	{
		ShowWindow(g_Window.deviceWindowed, SW_RESTORE);
	}

	// doing the IsIconic() check first also handles the WPF_RESTORETOMAXIMIZED case
	if (IsZoomed(g_Window.deviceWindowed))
	{
		ShowWindow(g_Window.deviceWindowed, SW_RESTORE);
	}

	// Make the window visible
	if (!IsWindowVisible(g_Window.focus))
	{
		ShowWindow(g_Window.focus, SW_SHOW);
	}

	// Ensure that the display doesn't power down when fullscreen but does when windowed
	if (is_WindowedFromDesc(newDesc))
	{
		SetThreadExecutionState(ES_DISPLAY_REQUIRED | ES_CONTINUOUS);
	}
	else
	{
		SetThreadExecutionState(ES_CONTINUOUS);
	}

	pause(false, false);

	return hr;
}

//--------------------------------------------------------------------------------------
// Active task
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Direct3D, MainLoop)(
	/* [r] */ HACCEL hAccel = nullptr);

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, Render3DEnvironment)(
	/* [x] */ void)
{

}

//--------------------------------------------------------------------------------------
// Common tasks 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, pause)(
	/* [r] */ bool _pauseTime,
	/* [r] */ bool _pauseRendering)
{
	int nPauseTimeCount = g_TimeStream.pauseTimeCount;
	if (g_TimeStream.pauseTime)
	{
		++nPauseTimeCount;
	}
	else
	{
		--nPauseTimeCount;
	}

	if (nPauseTimeCount < 0)
	{
		nPauseTimeCount = 0;
	}

	g_TimeStream.pauseTimeCount = nPauseTimeCount;

	int nPauseRenderingCount = g_TimeStream.pauseRenderingCount;
	if (g_TimeStream.pauseRendering)
	{
		nPauseRenderingCount++;
	}
	else
	{
		nPauseRenderingCount--;
	}

	if (nPauseRenderingCount < 0)
	{
		nPauseRenderingCount = 0;
	}

	g_TimeStream.pauseRenderingCount = nPauseRenderingCount;

	if (nPauseTimeCount > 0)
	{
		// Stop the scene from animating
		g_TimeStream.timer.stop();
	}
	else
	{
		// Restart the timer
		g_TimeStream.timer.start();
	}

	g_TimeStream.pauseTime = nPauseTimeCount > 0;
	g_TimeStream.pauseRendering = nPauseRenderingCount > 0;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_ConstantFrameTime)(
	/* [r] */ bool _constantFrameTime,
	/* [r] */ float _fTimePerFrame)
{
	g_TimeStream.constantFrameTime = _constantFrameTime;
	g_TimeStream.constantTimePerFrame = _fTimePerFrame;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_CursorSettings)(
	/* [r] */ bool _showCursorWhenFullScreen,
	/* [r] */ bool _clipCursorWhenFullScreen)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_D3DVersionSupport)(
	/* [r] */ bool _appCanUseD3D9,
	/* [r] */ bool _appCanUseD3D10)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_MultimonSettings)(
	/* [r] */ bool _autoChangeAdapter)
{
	g_DeviceDescs.autoChangeAdapter = _autoChangeAdapter;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_ShortcutKeySettings)(
	/* [r] */ bool _allowWhenFullscreen,
	/* [r] */ bool _allowWhenWindowed);
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Direct3D, set_Timer)(
	/* [r] */ CALLBACK_TIMER _callbackTimer,
	/* [r] */ float _fTimeoutInSecs,
	/* [r] */ unsigned int * _nIDEvent,
	/* [r] */ void * _userContext)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Direct3D, kill_Timer)(
	/* [r] */ unsigned int nIDEvent);

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, reset_FrameworkState)(
	/* [x] */ void);

// 설명 : 
//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, shutdown)(
	/* [r] */ int _exitCode = 0);

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, set_is_in_GammaCorrectMode)(
	/* [r] */ bool _gammaCorrect);

//--------------------------------------------------------------------------------------
// General
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, HINSTANCE, get_HINSTANCE)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, HWND, get_HWND)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, HWND, get_HWND_Focus)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, HWND, get_HWND_DeviceFullScreen)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, HWND, get_HWND_DeviceWindowed)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, RECT, get_Window_ClientRect)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, long, get_Window_Width)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, long, get_Window_Height)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, RECT, get_Window_ClientRect_AtModeChange)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, RECT, get_FullsceenClientRect_AtModeChange)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, double, get_Time)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, float, get_ElapsedTime)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, is_Windowed)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, is_In_GammaCorrectMode)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, float, get_FPS)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, const wchar_t *, get_Window_Title)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, const wchar_t *, get_FrameStats)(
	/* [r] */ bool bIncludeFPS = false)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, const wchar_t *, get_DeviceStats)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, const wchar_t *, get_D3D10_CounterStats)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, is_Vsync_Enabled)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, is_Rendering_Paused)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, is_Time_Paused)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, is_Active)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, int, get_ExitCode)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, get_Show_MsgBox_OnError)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, get_Automation)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, is_KeyDown)(
	/* [r] */ BYTE vKey)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, was_KeyPressed)(
	/* [r] */ BYTE vKey)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, is_MouseButtonDown)(
	/* [r] */ BYTE vButton)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Direct3D, create_State)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, void, destroy_State)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
// State Retrieval  
//--------------------------------------------------------------------------------------

// Direct3D 9

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, IDirect3D9 *, get_D3D9)(
	/* [x] */ void)const
{
	IF_INVALID(g_Device.direct3D9)
	{
		IDirect3D9 * d3d9 = Direct3DCreate9(D3DX_SDK_VERSION);
		g_Device.direct3D9 = AutoRelease<IDirect3D9>(d3d9);
		DEL_COM(d3d9);
	}

	return g_Device.direct3D9;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, IDirect3DDevice9 *, get_D3D9_Device) (
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, D3D9_DEVICE_DESC, get_D3D9_Settings)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, D3DPRESENT_PARAMETERS, get_D3D9_PresentParameters) (
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, const D3DSURFACE_DESC *, get_D3D9_BackBufferSurfaceDesc) (
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Direct3D, get_D3D9_DeviceCaps) (
	/* [w] */ D3DCAPS9 & _caps)const
{
	switch (g_DeviceDescs.version)
	{
	case D3D9_DEVICE:
		return g_Device.direct3D9->GetDeviceCaps(
			g_DeviceDescs.d3d9DeviceDesc.adapterOrdinal,
			g_DeviceDescs.d3d9DeviceDesc.deviceType,
			&_caps);
	case D3D10_DEVICE:
		return g_Device.direct3D9->GetDeviceCaps(
			g_DeviceDescs.d3d10DeviceDesc.adapterOrdinal,
			convert_DriverType_DXGITOD3D9(g_DeviceDescs.d3d10DeviceDesc.driverType),
			&_caps);
	}

	return E_INVALIDARG;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Direct3D, get_D3D9_DeviceCaps)(
	/* [w] */ D3DCAPS9 & _caps,
	/* [r] */ const Direct3D_DeviceDescs & _d3dDesc)const
{
	switch (g_DeviceDescs.version)
	{
	case D3D9_DEVICE:
		return g_Device.direct3D9->GetDeviceCaps(
			_d3dDesc.d3d9DeviceDesc.adapterOrdinal,
			_d3dDesc.d3d9DeviceDesc.deviceType,
			&_caps);
	case D3D10_DEVICE:
		return g_Device.direct3D9->GetDeviceCaps(
			_d3dDesc.d3d10DeviceDesc.adapterOrdinal,
			convert_DriverType_DXGITOD3D9(_d3dDesc.d3d10DeviceDesc.driverType),
			&_caps);
	}

	return E_INVALIDARG;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, does_AppSupport_D3D9)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, is_AppRenderingWithD3D9)(
	/* [x] */ void)const;

// Direct3D 10

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, is_D3D10_Available)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, ID3D10Device *, get_D3D10_Device)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, ID3D10Device1 *, get_D3D10_Device1)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, D3D10DeviceSettings, get_D3D10_Settings)(
	/* [x] */ void)const;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, IDXGIFactory *, get_DXGIFactory)(
	/* [x] */ void)const
{
	IF_INVALID(g_Device.dxgiFactory)
	{
		HRESULT hr;
		IF_FAILED(hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&g_Device.dxgiFactory))
		{
			direct3D_OutputDebugString(L"I don't know this err!! your device maybe not support D3D10...", hr);
		}
	}

	return g_Device.dxgiFactory;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, IDXGISwapChain *, get_DXGISwapChain)(
	/* [x] */ void)const
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, ID3D10RenderTargetView *, get_D3D10_RenderTargetView)(
	/* [x] */ void)const
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, ID3D10DepthStencilView *, get_D3D10_DepthStencilView)(
	/* [x] */ void)const
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, const DXGI_SURFACE_DESC *, get_DXGIBackBufferSurfaceDesc)(
	/* [x] */ void)const
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, does_AppSupport_D3D10)(
	/* [x] */ void)const
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Direct3D, bool, is_AppRenderingWithD3D10)(
	/* [x] */ void)const
{

}

//--------------------------------------------------------------------------------------
REALIZE_FUNC(create_3DEnvironment9)(
	/* [x] */ void)
{
	if (g_Device.d3d9Device)
	{
		return S_FALSE;
	}

	// Try to create the device with the chosen settings
	IDirect3D9 * const d3d = g_Device.direct3D9;

	const D3D9_DEVICE_DESC & deviceDesc =
		g_DeviceDescs.d3d9DeviceDesc;

	// create device
	HRESULT hr = E_FAIL;

	AutoRelease<IDirect3DDevice9> d3d9Device;
	IF_FAILED(hr = d3d->CreateDevice(
		deviceDesc.adapterOrdinal,
		deviceDesc.deviceType,
		g_Window.focus,
		deviceDesc.behaviorFlags,
		(D3DPRESENT_PARAMETERS*)&deviceDesc.pp,
		&d3d9Device))
	{
		return hr;
	}

	// If switching to REF, set the exit code to 10.  If switching to HAL and exit code was 10, then set it back to 0.
	if (deviceDesc.deviceType == D3DDEVTYPE_REF && g_ExitCode == 0)
	{
		g_ExitCode = 10;
	}
	else if (deviceDesc.deviceType == D3DDEVTYPE_HAL && g_ExitCode == 10)
	{
		g_ExitCode = 0;
	}

	D3DSURFACE_DESC backBufferDesc;

	// Update back buffer desc before calling app's device callbacks
	AutoRelease<IDirect3DSurface9> backBuffer;
	IF_SUCCEEDED(d3d9Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer))
	{
		backBuffer->GetDesc(&backBufferDesc);
	}

	// Setup cursor based on current settings (window/fullscreen mode, show cursor state, clip cursor state)
	setup_DeviceCursor(D3D9_DEVICE);

	// Call the app's device created callback if non-NULL
	CALLBACK_D3D9_DEVICE_CREATED pCallbackDeviceCreated =
		g_Callbacks.d3d9DeviceCreatedFunc;

	if (pCallbackDeviceCreated)
	{
		hr = pCallbackDeviceCreated(
			d3d9Device,
			backBufferDesc,
			g_cbUserContext.d3d9DeviceCreatedFuncUserContext);
	}

	IF_SUCCEEDED(hr)
	{
		g_Device.d3d9Device = d3d9Device;
		memcpy_s(&g_Device.backBuffer_Desc, 0, &backBufferDesc, sizeof(D3DSURFACE_DESC));
	}

	return hr;
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC(reset_3DEnvironment9)(
	/* [x] */ void)
{
	IDirect3DDevice9 * const d3d9Device = g_Device.d3d9Device;

	// Call the app's device lost callback
	CALLBACK_D3D9_DEVICE_LOST callback_DeviceLost =
		g_Callbacks.d3d9DeviceLostFunc;

	if (d3d9Device)
	{
		if (callback_DeviceLost)
		{
			callback_DeviceLost(
				g_cbUserContext.d3d9DeviceLostFuncUserContext);
		}
	}

	HRESULT hr;

	// Reset the device
	const Direct3D_DeviceDescs & desc = g_DeviceDescs;
	IF_FAILED(hr = d3d9Device->Reset((D3DPRESENT_PARAMETERS*)&desc.d3d9DeviceDesc.pp))
	{
		if (hr == D3DERR_DEVICELOST)
		{
			// Reset could legitimately fail if the device is lost
			return D3DERR_DEVICELOST;
		}
		else
		{
			return hr;
		}
	}

	// Update back buffer desc before calling app's device callbacks
	update_BackBufferDesc(D3D9_DEVICE);

	// Call the app's OnDeviceReset callback
	CALLBACK_D3D9_DEVICE_RESET callback_DeviceReset
		= g_Callbacks.d3d9DeviceResetFunc;

	if (callback_DeviceReset)
	{
		IF_FAILED(hr = callback_DeviceReset(
			d3d9Device,
			g_Device.backBuffer_Desc,
			g_cbUserContext.d3d9DeviceResetFuncUserContext))
		{

			// If callback failed
			if (hr != Direct3DERR_MEDIANOTFOUND)
			{
				hr = Direct3DERR_RESETTINGDEVICEOBJECTS;
			}

			// cleanup
			cleanup_3DEnvironment9(false);

			return hr;
		}
	}

	// Setup cursor based on current settings (window/fullscreen mode, show cursor state, clip cursor state)
	setup_DeviceCursor(D3D9_DEVICE);

	return hr;
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC_T(void, render_3DEnvironment9)(
	/* [x] */ void)
{
	// If no device created yet because device was lost (ie. another fullscreen exclusive device exists), 
	// then wait and try to create every so often.
	IDirect3DDevice9 * const d3d9Device = g_Device.d3d9Device;

	// Get the app's time, in seconds. Skip rendering if no time elapsed
	double fTime, fAbsTime; float fElapsedTime;
	DXUTGetGlobalTimer()->GetTimeValues(&fTime, &fAbsTime, &fElapsedTime);

	// Store the time for the app
	if (GetDXUTState().GetConstantFrameTime())
	{
		fElapsedTime = GetDXUTState().GetTimePerFrame();
		fTime = DXUTGetTime() + fElapsedTime;
	}

	GetDXUTState().SetTime(fTime);
	GetDXUTState().SetAbsoluteTime(fAbsTime);
	GetDXUTState().SetElapsedTime(fElapsedTime);

	DXUTHandleTimers();

	// Animate the scene by calling the app's frame move callback
	LPDXUTCALLBACKFRAMEMOVE pCallbackFrameMove = GetDXUTState().GetFrameMoveFunc();
	if (pCallbackFrameMove != NULL)
	{
		pCallbackFrameMove(fTime, fElapsedTime, GetDXUTState().GetFrameMoveFuncUserContext());
		pd3dDevice = DXUTGetD3D9Device();
		if (NULL == pd3dDevice) // Handle DXUTShutdown from inside callback
			return;
	}

	if (!GetDXUTState().GetRenderingPaused())
	{
		// Render the scene by calling the app's render callback
		LPDXUTCALLBACKD3D9FRAMERENDER pCallbackFrameRender = GetDXUTState().GetD3D9FrameRenderFunc();
		if (pCallbackFrameRender != NULL)
		{
			pCallbackFrameRender(pd3dDevice, fTime, fElapsedTime,
				GetDXUTState().GetD3D9FrameRenderFuncUserContext());
			pd3dDevice = DXUTGetD3D9Device();
			if (NULL == pd3dDevice) // Handle DXUTShutdown from inside callback
				return;
		}

#if defined(DEBUG) || defined(_DEBUG)
		// The back buffer should always match the client rect 
		// if the Direct3D backbuffer covers the entire window
		RECT rcClient;
		GetClientRect(DXUTGetHWND(), &rcClient);
		if (!IsIconic(DXUTGetHWND()))
		{
			GetClientRect(DXUTGetHWND(), &rcClient);
			assert(DXUTGetD3D9BackBufferSurfaceDesc()->Width == (unsigned int)rcClient.right);
			assert(DXUTGetD3D9BackBufferSurfaceDesc()->Height == (unsigned int)rcClient.bottom);
		}
#endif

		// Show the frame on the primary surface.
		HRESULT hr;
		IF_FAILED(hr = d3d9Device->Present(NULL, NULL, NULL, NULL))
		{
			if (D3DERR_DEVICELOST == hr)
			{
				GetDXUTState().SetDeviceLost(true);
			}

			else if (D3DERR_DRIVERINTERNALERROR == hr)
			{
				// When D3DERR_DRIVERINTERNALERROR is returned from Present(),
				// the application can do one of the following:
				// 
				// - End, with the pop-up window saying that the application cannot continue 
				//   because of problems in the display adapter and that the user should 
				//   contact the adapter manufacturer.
				//
				// - Attempt to restart by calling IDirect3DDevice9::Reset, which is essentially the same 
				//   path as recovering from a lost device. If IDirect3DDevice9::Reset fails with 
				//   D3DERR_DRIVERINTERNALERROR, the application should end immediately with the message 
				//   that the user should contact the adapter manufacturer.
				// 
				// The framework attempts the path of resetting the device
				// 
				GetDXUTState().SetDeviceLost(true);
			}
		}
	}

	// If the app called DXUTWasKeyPressed() then do the work 
	// to store the current state of the keys in bLastKeys
	if (GetDXUTState().GetAppCalledWasKeyPressed())
	{
		bool* bLastKeys = GetDXUTState().GetLastKeys();
		bool* bKeys = GetDXUTState().GetKeys();
		memcpy(bLastKeys, bKeys, sizeof(bool)* 256);
	}

	// Update current frame #
	int nFrame = GetDXUTState().GetCurrentFrameNumber();
	nFrame++;
	GetDXUTState().SetCurrentFrameNumber(nFrame);

	// Check to see if the app should shutdown due to cmdline
	if (GetDXUTState().GetOverrideQuitAfterFrame() != 0)
	{
		if (nFrame > GetDXUTState().GetOverrideQuitAfterFrame())
			DXUTShutdown();
	}

	return;
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC_T(void, cleanup_3DEnvironment9)(
	bool _releaseSettings)
{
	IDirect3DDevice9 * const d3d9Device = g_Device.d3d9Device;

	// Call the app's device lost callback
	CALLBACK_D3D9_DEVICE_LOST callback_DeviceLost =
		g_Callbacks.d3d9DeviceLostFunc;

	CALLBACK_D3D9_DEVICE_DESTROYED callback_DeviceDestroy =
		g_Callbacks.d3d9DeviceDestroyedFunc;

	if (d3d9Device)
	{
		// Call the app's device lost callback
		if (callback_DeviceLost)
		{
			callback_DeviceLost(
				g_cbUserContext.d3d9DeviceLostFuncUserContext);
		}

		// Call the app's device destroyed callback
		if (callback_DeviceDestroy)
		{
			callback_DeviceDestroy(
				g_cbUserContext.d3d9DeviceDestroyedFuncUserContext);
		}
	}

	// Release the D3D device and in debug configs, displays a message box if there 
	// are unrelease objects.
	if (d3d9Device)
	{
		g_Device.d3d9Device.~AutoRelease();
	}

	if (_releaseSettings)
	{
		ZeroMemory(&g_DeviceDescs, sizeof(Direct3D_DeviceDescs));
	}
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC(setup_DeviceCursor)(
	/* [r] */ DEVICE_VERSION _version)
{
	switch (_version)
	{
	case D3D9_DEVICE:
	{
						const D3D9_DEVICE_DESC desc = g_DeviceDescs.d3d9DeviceDesc;

						// Show the cursor again if returning to fullscreen 
						IDirect3DDevice9 * const d3dDevice = g_Device.d3d9Device;
						IF_FALSE(desc.pp.Windowed)
						{
							SetCursor(nullptr); // Turn off Windows cursor in full screen mode
							HCURSOR hCursor = (HCURSOR)(ULONG_PTR)GetClassLongPtr(g_Window.deviceFullScreen, GCLP_HCURSOR);

							set_D3D9DeviceCursor(d3dDevice, hCursor, false);
							d3dDevice->ShowCursor(true);
						}

						// Clip cursor if requested
						if (desc.pp.Windowed)
						{
							ClipCursor(nullptr);
						}
						else
						{
							// Confine cursor to full screen window
							RECT rcWindow;
							GetWindowRect(g_Window.deviceFullScreen, &rcWindow);
							ClipCursor(&rcWindow);
						}
	}
		break;
	case D3D10_DEVICE:
	{
						 const D3D10_DEVICE_DESC desc = g_DeviceDescs.d3d10DeviceDesc;

						 // Clip cursor if requested
						 if (desc.sd.Windowed)
						 {
							 ClipCursor(NULL);
						 }
						 else
						 {
							 HCURSOR hCursor = (HCURSOR)(ULONG_PTR)GetClassLongPtr(g_Window.deviceFullScreen, GCLP_HCURSOR);
							 SetCursor(hCursor); // Reset the cursor in case we turned it off in fullscreen mode d3d9

							 // Confine cursor to full screen window
							 RECT rcWindow;
							 GetWindowRect(g_Window.deviceFullScreen, &rcWindow);
							 ClipCursor(&rcWindow);
						 }
	}
		break;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC(set_D3D9DeviceCursor)(
	IDirect3DDevice9 * _d3dDevice,
	HCURSOR _hCursor,
	bool _addWatermark)
{
	HRESULT hr = E_FAIL;
	ICONINFO iconinfo;
	bool bBWCursor = false;
	LPDIRECT3DSURFACE9 pCursorSurface = nullptr;
	HDC hdcColor = nullptr;
	HDC hdcMask = nullptr;
	HDC hdcScreen = nullptr;
	BITMAP bm;
	unsigned long dwWidth = 0;
	unsigned long dwHeightSrc = 0;
	unsigned long dwHeightDest = 0;
	COLORREF crColor;
	COLORREF crMask;
	unsigned int x;
	unsigned int y;
	BITMAPINFO bmi;
	COLORREF* pcrArrayColor = nullptr;
	COLORREF* pcrArrayMask = nullptr;
	unsigned long* pBitmap;
	HGDIOBJ hgdiobjOld;

	ZeroMemory(&iconinfo, sizeof(iconinfo));
	if (!GetIconInfo(_hCursor, &iconinfo))
		goto End;

	if (0 == GetObject((HGDIOBJ)iconinfo.hbmMask, sizeof(BITMAP), (LPVOID)&bm))
		goto End;
	dwWidth = bm.bmWidth;
	dwHeightSrc = bm.bmHeight;

	if (iconinfo.hbmColor == nullptr)
	{
		bBWCursor = TRUE;
		dwHeightDest = dwHeightSrc / 2;
	}
	else
	{
		bBWCursor = FALSE;
		dwHeightDest = dwHeightSrc;
	}

	// Create a surface for the fullscreen cursor
	IF_FAILED(hr = _d3dDevice->CreateOffscreenPlainSurface(
		dwWidth,
		dwHeightDest,
		D3DFMT_A8R8G8B8,
		D3DPOOL_SCRATCH,
		&pCursorSurface,
		nullptr))
	{
		goto End;
	}

	pcrArrayMask = new unsigned long[dwWidth * dwHeightSrc];

	ZeroMemory(&bmi, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = dwWidth;
	bmi.bmiHeader.biHeight = dwHeightSrc;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	hdcScreen = GetDC(nullptr);
	hdcMask = CreateCompatibleDC(hdcScreen);
	if (hdcMask == nullptr)
	{
		hr = E_FAIL;
		goto End;
	}
	hgdiobjOld = SelectObject(hdcMask, iconinfo.hbmMask);

	GetDIBits(
		hdcMask,
		iconinfo.hbmMask,
		0,
		dwHeightSrc,
		pcrArrayMask,
		&bmi,
		DIB_RGB_COLORS);

	SelectObject(hdcMask, hgdiobjOld);

	if (!bBWCursor)
	{
		pcrArrayColor = new unsigned long[dwWidth * dwHeightDest];
		hdcColor = CreateCompatibleDC(hdcScreen);
		if (hdcColor == nullptr)
		{
			hr = E_FAIL;
			goto End;
		}
		SelectObject(hdcColor, iconinfo.hbmColor);

		GetDIBits(
			hdcColor,
			iconinfo.hbmColor,
			0,
			dwHeightDest,
			pcrArrayColor,
			&bmi,
			DIB_RGB_COLORS);
	}

	// Transfer cursor image into the surface
	D3DLOCKED_RECT lr;
	IF_SUCCEEDED(pCursorSurface->LockRect(&lr, nullptr, 0))
	{
		pBitmap = (unsigned long*)lr.pBits;
		for (y = 0; y < dwHeightDest; y++)
		{
			for (x = 0; x < dwWidth; x++)
			{
				if (bBWCursor)
				{
					crColor = pcrArrayMask[dwWidth * (dwHeightDest - 1 - y) + x];
					crMask = pcrArrayMask[dwWidth * (dwHeightSrc - 1 - y) + x];
				}
				else
				{
					crColor = pcrArrayColor[dwWidth * (dwHeightDest - 1 - y) + x];
					crMask = pcrArrayMask[dwWidth * (dwHeightDest - 1 - y) + x];
				}
				if (crMask == 0)
					pBitmap[dwWidth * y + x] = 0xff000000 | crColor;
				else
					pBitmap[dwWidth * y + x] = 0x00000000;

				// It may be helpful to make the D3D cursor look slightly 
				// different from the Windows cursor so you can distinguish 
				// between the two when developing/testing code.  When
				// bAddWatermark is TRUE, the following code adds some
				// small grey "D3D" characters to the upper-left corner of
				// the D3D cursor image.
				if (_addWatermark && x < 12 && y < 5)
				{
					// 11.. 11.. 11.. .... CCC0
					// 1.1. ..1. 1.1. .... A2A0
					// 1.1. .1.. 1.1. .... A4A0
					// 1.1. ..1. 1.1. .... A2A0
					// 11.. 11.. 11.. .... CCC0

					const WORD wMask[5] = { 0xccc0, 0xa2a0, 0xa4a0, 0xa2a0, 0xccc0 };
					if (wMask[y] & (1 << (15 - x)))
					{
						pBitmap[dwWidth * y + x] |= 0xff808080;
					}
				}
			}
		}
		pCursorSurface->UnlockRect();
	}

	// Set the device cursor
	if (FAILED(hr = _d3dDevice->SetCursorProperties(
		iconinfo.xHotspot,
		iconinfo.yHotspot,
		pCursorSurface)))
	{
		goto End;
	}

	hr = S_OK;

End:

	if (iconinfo.hbmMask != nullptr)
	{
		DeleteObject(iconinfo.hbmMask);
	}

	if (iconinfo.hbmColor != nullptr)
	{
		DeleteObject(iconinfo.hbmColor);
	}

	if (hdcScreen != nullptr)
	{
		ReleaseDC(nullptr, hdcScreen);
	}

	if (hdcColor != nullptr)
	{
		DeleteDC(hdcColor);
	}

	if (hdcMask != nullptr)
	{
		DeleteDC(hdcMask);
	}

	DEL_POINTER_ARRAY(pcrArrayColor);
	DEL_POINTER_ARRAY(pcrArrayMask);
	DEL_COM(pCursorSurface);

	return hr;
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC(create_3DEnvironment10)(
	/* [x] */ void)
{
	if (g_Device.d3d10Device)
	{
		return S_FALSE;
	}

	IDXGIFactory * const dxgiFactory = g_Device.dxgiFactory;
	dxgiFactory->MakeWindowAssociation(nullptr, 0);

	const D3D10_DEVICE_DESC & _desc =
		g_DeviceDescs.d3d10DeviceDesc;

	AutoRelease<ID3D10Device> d3d10Device;
	AutoRelease<ID3D10Device1> d3d10Device1;
	AutoRelease<IDXGISwapChain> dxgiSwapChain;

	HRESULT hr = S_OK;

	// Only create a Direct3D device if one hasn't been supplied by the app
	// Try to create the device with the chosen settings
	HMODULE wrp = nullptr;
	if (_desc.driverType == D3D10_DRIVER_TYPE_SOFTWARE)
	{
		wrp = LoadLibrary(L"D3D10WARP.dll");
	}

	AutoRelease<IDXGIAdapter> dxgiAdapter;
	if (_desc.driverType == D3D10_DRIVER_TYPE_HARDWARE)
	{
		IF_FAILED(hr = dxgiFactory->EnumAdapters(_desc.adapterOrdinal, &dxgiAdapter))
		{
			direct3D_OutputDebugString(L"Direct3DERR_CREATINGDEVICE : dxgiFactory::EnumAdapters", hr);
			return hr;
		}
	}

	// Try creating the D3D10.1 device first
	if (SUCCEEDED(hr = D3D10CreateDevice1(
		dxgiAdapter,
		_desc.driverType,
		(HMODULE)wrp,
		_desc.createFlags,
		D3D10_FEATURE_LEVEL_10_1,
		D3D10_1_SDK_VERSION,
		&d3d10Device1)))
	{
		// Get a D3D10.0 device pointer from the D3D10.1 device
		hr = d3d10Device1->QueryInterface(__uuidof(ID3D10Device), (void**)&d3d10Device);
	}
	else // if( FAILED(hr) )
	{
		// If D3D10.1 doesn't exist, then fallback to D3D10.0
		hr = D3D10CreateDevice(
			dxgiAdapter,
			_desc.driverType,
			(HMODULE)wrp,
			_desc.createFlags,
			D3D10_SDK_VERSION,
			&d3d10Device);
	}

	IF_FAILED(hr)
	{
		direct3D_OutputDebugString(L"Direct3DERR_CREATINGDEVICE : D3D10CreateDevice", hr);
		return hr;
	}

	if (_desc.driverType != D3D10_DRIVER_TYPE_HARDWARE)
	{
		AutoRelease<IDXGIDevice> dxgiDev;
		IF_SUCCEEDED(hr = d3d10Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDev))
		{
			dxgiDev->GetAdapter(&dxgiAdapter);
		}
	}

	// Enumerate its outputs.
	unsigned int iOutputCount;
	for (iOutputCount = 0;; ++iOutputCount)
	{
		AutoRelease<IDXGIOutput> dxgiOutput;
		IF_FAILED(dxgiAdapter->EnumOutputs(iOutputCount, &dxgiOutput))
		{
			break;
		}
	}

	std::vector<AutoRelease<IDXGIOutput>> outputArray(iOutputCount);

	unsigned int iOutput;
	for (iOutput = 0; iOutput < iOutputCount; ++iOutput)
	{
		dxgiAdapter->EnumOutputs(iOutput, &outputArray[iOutput]);
	}

	// Create the swapchain
	IF_FAILED(hr = dxgiFactory->CreateSwapChain(
		d3d10Device,
		(DXGI_SWAP_CHAIN_DESC *)&_desc.sd,
		&dxgiSwapChain))
	{
		direct3D_OutputDebugString(L"Direct3DERR_CREATINGDEVICE : CreateSwapChain", hr);
		return hr;
	}

	// If switching to REF, set the exit code to 10.  If switching to HAL and exit code was 10, then set it back to 0.
	if (_desc.driverType == D3D10_DRIVER_TYPE_REFERENCE && g_ExitCode == 0)
	{
		g_ExitCode = 10;
	}
	else if (_desc.driverType == D3D10_DRIVER_TYPE_HARDWARE && g_ExitCode == 10)
	{
		g_ExitCode = 0;
	}

	g_Device.d3d10Device = d3d10Device;
	g_Device.d3d10Device1 = d3d10Device1;
	g_Device.dxgiSwapChain = dxgiSwapChain;

	// Update back buffer desc before calling app's device callbacks
	update_BackBufferDesc(D3D10_DEVICE);

	// Setup cursor based on current settings (window/fullscreen mode, show cursor state, clip cursor state)
	setup_DeviceCursor(D3D10_DEVICE);

	// Call the app's device created callback if non-nullptr
	const DXGI_SURFACE_DESC & backBufferSurfaceDesc =
		g_Device.dxgiBackBuffer_Desc;

	CALLBACK_D3D10_DEVICE_CREATED callback_DeviceCreated =
		g_Callbacks.d3d10DeviceCreatedFunc;

	if (callback_DeviceCreated != nullptr)
	{
		hr = callback_DeviceCreated(
			d3d10Device,
			backBufferSurfaceDesc,
			g_cbUserContext.d3d10DeviceCreatedFuncUserContext);
	}

	if (FAILED(hr))
	{
		direct3D_OutputDebugString(L"Direct3DERR_CREATINGDEVICE : DeviceCreated callback", hr);
		cleanup_3DEnvironment10(false);

		return hr;
	}

	// Setup the render target view and viewport
	IF_FAILED(hr = setup_D3D10Views(_desc))
	{
		direct3D_OutputDebugString(L"Direct3DERR_CREATINGDEVICEOBJECTS : setup_D3D10Views", hr);
		cleanup_3DEnvironment10(false);

		return hr;
	}

	// Call the app's swap chain reset callback if non-nullptr
	CALLBACK_D3D10_SWAPCHAINRESIZED callback_SwapChainResized =
		g_Callbacks.d3d10SwapChainResizedFunc;

	if (callback_SwapChainResized)
	{
		IF_FAILED(hr = callback_SwapChainResized(
			d3d10Device,
			dxgiSwapChain,
			backBufferSurfaceDesc,
			g_cbUserContext.d3d10SwapChainResizedFuncUserContext))
		{
			direct3D_OutputDebugString(L"Direct3DERR_CREATINGDEVICE : DeviceReset callback", hr);
			cleanup_3DEnvironment10(false);

			return hr;
		}
	}

	return hr;
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC(reset_3DEnvironment10)(
	/* [x] */ void)
{
	IDXGISwapChain * const dxgiSwapChain = g_Device.dxgiSwapChain;
	D3D10_DEVICE_DESC & desc = g_DeviceDescs.d3d10DeviceDesc;

	g_Direct3D.pause(true, true);

	DXGI_SWAP_CHAIN_DESC SCDesc;
	dxgiSwapChain->GetDesc(&SCDesc);

	// Resize backbuffer and target of the swapchain in case they have changed.
	// For windowed mode, use the client rect as the desired size. Unlike D3D9,
	// we can't use 0 for width or height.  Therefore, fill in the values from
	// the window size. For fullscreen mode, the width and height should have
	// already been filled with the desktop resolution, so don't change it.
	if (desc.sd.Windowed && SCDesc.Windowed)
	{
		RECT rcWnd;
		GetClientRect(g_Window.focus, &rcWnd);
		desc.sd.BufferDesc.Width = rcWnd.right - rcWnd.left;
		desc.sd.BufferDesc.Height = rcWnd.bottom - rcWnd.top;
	}

	HRESULT hr;
	bool bDeferredDXGIAction = false;

	// If the app wants to switch from windowed to fullscreen or vice versa,
	// call the swapchain's SetFullscreenState
	// mode.
	if (SCDesc.Windowed != desc.sd.Windowed)
	{
		// Set the fullscreen state
		if (desc.sd.Windowed)
		{
			IF_FAILED(hr = dxgiSwapChain->SetFullscreenState(FALSE, NULL))
			{
				return hr;
			}

			bDeferredDXGIAction = true;
		}
		else
		{
			// Set fullscreen state by setting the display mode to fullscreen, then changing the resolution
			// to the desired value.

			// SetFullscreenState causes a WM_SIZE message to be sent to the window.  The WM_SIZE message calls
			// DXUTCheckForDXGIBufferChange which normally stores the new height and width in 
			// pDeviceSettings->d3d10.sd.BufferDesc.  SetDoNotStoreBufferSize tells DXUTCheckForDXGIBufferChange
			// not to store the height and width so that we have the correct values when calling ResizeTarget.
			IF_FAILED(hr = dxgiSwapChain->SetFullscreenState(FALSE, NULL))
			{
				return hr;
			}

			IF_FAILED(hr = dxgiSwapChain->ResizeTarget(&desc.sd.BufferDesc))
			{
				return hr;
			}

			bDeferredDXGIAction = true;
		}
	}
	else
	{
		if (desc.sd.BufferDesc.Width == SCDesc.BufferDesc.Width &&
			desc.sd.BufferDesc.Height == SCDesc.BufferDesc.Height &&
			desc.sd.BufferDesc.Format != SCDesc.BufferDesc.Format)
		{
			resize_DXGIBuffers(0, 0, !desc.sd.Windowed);
			bDeferredDXGIAction = true;
		}
		else if (desc.sd.BufferDesc.Width != SCDesc.BufferDesc.Width ||
			desc.sd.BufferDesc.Height != SCDesc.BufferDesc.Height)
		{
			IF_FAILED(hr = dxgiSwapChain->ResizeTarget(&desc.sd.BufferDesc))
			{
				return hr;
			}

			bDeferredDXGIAction = true;
		}
	}

	g_Direct3D.pause(false, false);

	return S_OK;
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC_T(void, cleanup_3DEnvironment10)(
	/* [r] */ bool _releaseSettings)
{
	ID3D10Device * const d3d10Device = g_Device.d3d10Device;

	if (d3d10Device)
	{
		// Call ClearState to avoid tons of messy debug spew telling us that we're deleting bound objects
		d3d10Device->ClearState();

		// Call the app's SwapChain lost callback
		CALLBACK_D3D10_SWAPCHAINRELEASING callback_SwapChainReleasing =
			g_Callbacks.d3d10SwapChainReleasingFunc;

		if (callback_SwapChainReleasing)
		{
			callback_SwapChainReleasing(
				g_cbUserContext.d3d10SwapChainReleasingFuncUserContext);
		}

		// Release our old depth stencil texture and view 
		g_Device.d3d10DepthStencil.~AutoRelease();
		g_Device.d3d10DSV.~AutoRelease();

		// Release our rasterizer state
		g_Device.d3d10DefaultRasterizerState.~AutoRelease();

		// Cleanup the render target view
		g_Device.d3d10RTV.~AutoRelease();

		// Call the app's device destroyed callback
		CALLBACK_D3D10_DEVICE_DESTROYED callback_DeviceDestroyed =
			g_Callbacks.d3d10DeviceDestroyedFunc;

		if (callback_DeviceDestroyed)
		{
			callback_DeviceDestroyed(
				g_cbUserContext.d3d10DeviceDestroyedFuncUserContext);
		}

		// Release the swap chain
		if (g_Device.dxgiSwapChain)
		{
			g_Device.dxgiSwapChain->SetFullscreenState(FALSE, 0);
		}

		g_Device.dxgiSwapChain.~AutoRelease();

		// Release the D3D10.1 device (if it exists) because it has a extra ref count on it
		g_Device.d3d10Device1.~AutoRelease();
		g_Device.d3d10Device.~AutoRelease();

		if (_releaseSettings)
		{
			ZeroMemory(&g_DeviceDescs.d3d10DeviceDesc, sizeof(DXGI_SURFACE_DESC));
		}
	}
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC_T(void, render_3DEnvironment10)(
	/* [x] */ void)
{

}

//--------------------------------------------------------------------------------------
REALIZE_FUNC(setup_D3D10Views)(
	/* [r] */ const D3D10_DEVICE_DESC & _desc)
{
	ID3D10Device * const d3d10Device = g_Device.d3d10Device;
	IDXGISwapChain * const dxgiSwapChain = g_Device.dxgiSwapChain;

	HRESULT hr = S_OK;

	AutoRelease<ID3D10DepthStencilView> d3d10DSV;
	AutoRelease<ID3D10RenderTargetView> d3d10RTV;

	// Get the back buffer and desc
	AutoRelease<ID3D10Texture2D> backBuffer;
	IF_FAILED(hr = dxgiSwapChain->GetBuffer(0, __uuidof(*backBuffer), (void**)&backBuffer))
	{
		return hr;
	}

	D3D10_TEXTURE2D_DESC backBufferSurfaceDesc;
	backBuffer->GetDesc(&backBufferSurfaceDesc);

	// Setup the viewport to match the backbuffer
	D3D10_VIEWPORT vp;
	vp.Width = backBufferSurfaceDesc.Width;
	vp.Height = backBufferSurfaceDesc.Height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	d3d10Device->RSSetViewports(1, &vp);

	// Create the render target view
	IF_FAILED(hr = d3d10Device->CreateRenderTargetView(
		backBuffer,
		nullptr,
		&d3d10RTV))
	{
		return hr;
	}

	// Create depth stencil texture
	AutoRelease<ID3D10Texture2D> d3d10DepthStencil;
	AutoRelease<ID3D10RasterizerState> d3d10RasterizerState;

	if (_desc.autoCreateDepthStencil)
	{
		D3D10_TEXTURE2D_DESC descDepth;
		descDepth.Width = backBufferSurfaceDesc.Width;
		descDepth.Height = backBufferSurfaceDesc.Height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = _desc.autoDepthStencilFormat;
		descDepth.SampleDesc.Count = _desc.sd.SampleDesc.Count;
		descDepth.SampleDesc.Quality = _desc.sd.SampleDesc.Quality;
		descDepth.Usage = D3D10_USAGE_DEFAULT;
		descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		IF_FAILED(hr = d3d10Device->CreateTexture2D(
			&descDepth,
			nullptr,
			&d3d10DepthStencil))
		{
			return hr;
		}

		// Create the depth stencil view
		D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;

		descDSV.Format = descDepth.Format;
		if (descDepth.SampleDesc.Count > 1)
		{
			descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2DMS;
		}
		else
		{
			descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
		}

		descDSV.Texture2D.MipSlice = 0;

		IF_FAILED(hr = d3d10Device->CreateDepthStencilView(
			d3d10DepthStencil,
			&descDSV,
			&d3d10DSV))
		{
			return hr;
		}

		// Create a default rasterizer state that enables MSAA
		D3D10_RASTERIZER_DESC RSDesc;
		RSDesc.FillMode = D3D10_FILL_SOLID;
		RSDesc.CullMode = D3D10_CULL_BACK;
		RSDesc.FrontCounterClockwise = FALSE;
		RSDesc.DepthBias = 0;
		RSDesc.SlopeScaledDepthBias = 0.0f;
		RSDesc.DepthBiasClamp = 0;
		RSDesc.DepthClipEnable = TRUE;
		RSDesc.ScissorEnable = FALSE;
		RSDesc.AntialiasedLineEnable = FALSE;
		if (descDepth.SampleDesc.Count > 1)
		{
			RSDesc.MultisampleEnable = TRUE;
		}
		else
		{
			RSDesc.MultisampleEnable = FALSE;
		}

		IF_FAILED(hr = d3d10Device->CreateRasterizerState(&RSDesc, &d3d10RasterizerState))
		{
			return hr;
		}
	}

	// Set the render targets
	d3d10Device->RSSetState(d3d10RasterizerState);
	d3d10Device->OMSetRenderTargets(1, &d3d10RTV, d3d10DSV);

	// initialize
	g_Device.d3d10DSV = d3d10DSV;
	g_Device.d3d10RTV = d3d10RTV;
	g_Device.d3d10DepthStencil = d3d10DepthStencil;
	g_Device.d3d10DefaultRasterizerState = d3d10RasterizerState;

	return hr;
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC_T(void, resize_DXGIBuffers)(
	/* [r] */ unsigned int _width,
	/* [r] */ unsigned int _height,
	/* [r] */ bool _fullScreen)
{
	ID3D10Device * const d3d10Device = g_Device.d3d10Device;
	IDXGISwapChain * const dxgiSwapChain = g_Device.dxgiSwapChain;

	HRESULT hr = S_OK;

	RECT rcCurrentClient;
	GetClientRect(g_Window.focus, &rcCurrentClient);

	// Call releasing	
	CALLBACK_D3D10_SWAPCHAINRELEASING callback_SwapChainReleasing =
		g_Callbacks.d3d10SwapChainReleasingFunc;

	if (callback_SwapChainReleasing)
	{
		callback_SwapChainReleasing(
			g_cbUserContext.d3d10SwapChainResizedFuncUserContext);
	}

	AutoRelease<ID3D10Texture2D> d3d10DS;
	AutoRelease<ID3D10DepthStencilView> d3d10DSV;
	AutoRelease<ID3D10RenderTargetView> d3d10RTV;
	AutoRelease<ID3D10RasterizerState> d3d10RState;

	// Alternate between 0 and DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH when resizing buffers.
	// When in windowed mode, we want 0 since this allows the app to change to the desktop
	// resolution from windowed mode during alt+enter.  However, in fullscreen mode, we want
	// the ability to change display modes from the Device Settings dialog.  Therefore, we
	// want to set the DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH flag.
	unsigned int Flags = 0;
	if (_fullScreen)
	{
		Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	}

	D3D10_DEVICE_DESC & desc = g_DeviceDescs.d3d10DeviceDesc;

	// ResizeBuffers
	IF_FAILED(hr = dxgiSwapChain->ResizeBuffers(
		desc.sd.BufferCount,
		_width,
		_height,
		desc.sd.BufferDesc.Format,
		Flags))
	{
		return;
	}

	// Determine if we're fullscreen
	desc.sd.Windowed = !_fullScreen;
	desc.sd.BufferDesc.Width = (unsigned int)rcCurrentClient.right;
	desc.sd.BufferDesc.Height = (unsigned int)rcCurrentClient.bottom;

	// Setup the render target view and viewport
	IF_FAILED(hr = setup_D3D10Views(desc))
	{
		return;
	}

	// Save off backbuffer desc
	update_BackBufferDesc(D3D10_DEVICE);

	// Setup cursor based on current settings (window/fullscreen mode, show cursor state, clip cursor state)
	setup_DeviceCursor(D3D10_DEVICE);

	// Call the app's SwapChain reset callback
	const DXGI_SURFACE_DESC & backBufferSurfaceDesc =
		g_Device.dxgiBackBuffer_Desc;

	CALLBACK_D3D10_SWAPCHAINRESIZED callback_SwapChainResized =
		g_Callbacks.d3d10SwapChainResizedFunc;

	if (callback_SwapChainResized)
	{
		Direct3D g_Direct3D;
		if (FAILED(hr = callback_SwapChainResized(
			d3d10Device,
			dxgiSwapChain,
			backBufferSurfaceDesc,
			g_cbUserContext.d3d10SwapChainResizedFuncUserContext)))
		{
			// If callback failed, cleanup
			CALLBACK_D3D10_SWAPCHAINRELEASING callback_SwapChainReleasing =
				g_Callbacks.d3d10SwapChainReleasingFunc;

			if (callback_SwapChainReleasing)
			{
				callback_SwapChainReleasing(
					g_cbUserContext.d3d10SwapChainResizedFuncUserContext);
			}

			g_Direct3D.pause(false, false);
			PostQuitMessage(0);
		}
		else
		{
			g_Direct3D.pause(false, false);
		}
	}
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC(update_BackBufferDesc)(
	/* [r] */ DEVICE_VERSION _version)
{
	HRESULT hr = E_FAIL;
	switch (_version)
	{
	case D3D9_DEVICE:
	{
						AutoRelease<IDirect3DSurface9> backBuffer;
						IF_SUCCEEDED(hr = g_Device.d3d9Device->GetBackBuffer(
							0,
							0,
							D3DBACKBUFFER_TYPE_MONO,
							&backBuffer))
						{
							backBuffer->GetDesc(&g_Device.backBuffer_Desc);
						}
	}
		break;
	case D3D10_DEVICE:
	{
						 AutoRelease<ID3D10Texture2D> backBuffer;
						 IF_SUCCEEDED(hr = g_Device.dxgiSwapChain->GetBuffer(
							 0,
							 __uuidof(*backBuffer),
							 (void**)&backBuffer))
						 {
							 D3D10_TEXTURE2D_DESC TexDesc;
							 backBuffer->GetDesc(&TexDesc);

							 DXGI_SURFACE_DESC & surface = g_Device.dxgiBackBuffer_Desc;
							 surface.Width = (unsigned int)TexDesc.Width;
							 surface.Height = (unsigned int)TexDesc.Height;
							 surface.Format = TexDesc.Format;
							 surface.SampleDesc = TexDesc.SampleDesc;
						 }
	}
		break;
	}

	return hr;
}

HRESULT DXUTFindD3D9AdapterFormat(
	unsigned int AdapterOrdinal,
	D3DDEVTYPE DeviceType,
	D3DFORMAT BackBufferFormat,
	BOOL Windowed,
	D3DFORMAT * pAdapterFormat);

HRESULT setupD3D10Views(ID3D10Device* pd3dDevice, DXUTDeviceSettings* pDeviceSettings);
HRESULT create3DEnvironment10(ID3D10Device* pd3dDeviceFromApp);
HRESULT reset3DEnvironment10();
void render3DEnvironment10();
void cleanup3DEnvironment10(bool bReleaseSettings = true);
void createD3D10Counters(ID3D10Device* pd3dDevice);
void destroyD3D10Counters();
void startPerformanceCounters();
void stopPerformanceCounters();
void updateD3D10CounterStats();

void updateD3D10DeviceStats(
	D3D10_DRIVER_TYPE DeviceType,
	DXGI_ADAPTER_DESC* pAdapterDesc);

//--------------------------------------------------------------------------------------
REALIZE_FUNC_T(void, checkFor_WindowChangingMonitors)(
	/* [x] */ void)
{
	// Skip this check for various reasons
	IF_FALSE(g_DeviceDescs.autoChangeAdapter)
	{
		return;
	}

	HMONITOR const hWindowMonitor = MonitorFromWindow(g_Window.focus, MONITOR_DEFAULTTOPRIMARY);
	HMONITOR const hAdapterMonitor = g_Window.adapterMonitor;
	
	if (hWindowMonitor != hAdapterMonitor)
	{
		UINT newOrdinal;
		IF_SUCCEEDED(get_AdapterOrdinalFromMonitor(&newOrdinal, hWindowMonitor))
		{
			// Find the closest valid device settings with the new ordinal
			Direct3D_DeviceDescs deviceDesc = g_DeviceDescs;
			
			switch (deviceDesc.version)
			{
			case D3D9_DEVICE:
			case D3D10_DEVICE:
			}
			if (DXUTIsD3D9(&deviceSettings))
			{
				deviceSettings.d3d9.AdapterOrdinal = newOrdinal;
			}
			else
			{
				deviceSettings.d3d10.AdapterOrdinal = newOrdinal;
				UINT newOutput;
				if (SUCCEEDED(DXUTGetOutputOrdinalFromMonitor(hWindowMonitor, &newOutput)))
					deviceSettings.d3d10.Output = newOutput;
			}

			DXUTMatchOptions matchOptions;
			matchOptions.eAPIVersion = DXUTMT_PRESERVE_INPUT;
			matchOptions.eAdapterOrdinal = DXUTMT_PRESERVE_INPUT;
			matchOptions.eDeviceType = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eWindowed = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eAdapterFormat = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eVertexProcessing = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eResolution = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eBackBufferFormat = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eBackBufferCount = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eMultiSample = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eSwapEffect = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eDepthFormat = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eStencilFormat = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.ePresentFlags = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eRefreshRate = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.ePresentInterval = DXUTMT_CLOSEST_TO_INPUT;

						HRESULT hr;
			hr = DXUTFindValidDeviceSettings(&deviceSettings, &deviceSettings, &matchOptions);
			if (SUCCEEDED(hr))
			{
				// Create a Direct3D device using the new device settings.  
				// If there is an existing device, then it will either reset or recreate the scene.
				hr = DXUTChangeDevice(&deviceSettings, NULL, NULL, false, false);

				// If hr == E_ABORT, this means the app rejected the device settings in the ModifySettingsCallback
				if (hr == E_ABORT)
				{
					// so nothing changed and keep from attempting to switch adapters next time
					GetDXUTState().SetAutoChangeAdapter(false);
				}
				else if (FAILED(hr))
				{
					DXUTShutdown();
					DXUTPause(false, false);
					return;
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC_T(HMONITOR, get_MonitorFromAdapter)(
	/* [r] */ const Direct3D_DeviceDescs & _desc)
{
	switch (_desc.version)
	{
	case D3D9_DEVICE:
	{
						IDirect3D9 * const d3d = g_Device.direct3D9;
						return d3d->GetAdapterMonitor(_desc.d3d9DeviceDesc.adapterOrdinal);
	}
	case D3D10_DEVICE:
	{
						 const VideoCard_Output_info * output_info =
							 g_Outside.get_Output_info(
							 _desc.d3d10DeviceDesc.adapterOrdinal,
							 _desc.d3d10DeviceDesc.output);

						 if (output_info)
						 {
							 return nullptr;
						 }

						 return MonitorFromRect(&output_info->desc.DesktopCoordinates, MONITOR_DEFAULTTONEAREST);
	}
	}

	return nullptr;
}

//--------------------------------------------------------------------------------------
REALIZE_FUNC(get_AdapterOrdinalFromMonitor)(
	/* [w] */ unsigned int * _adapterOrdinal,
	/* [r] */ HMONITOR _monitor)
{
	(*_adapterOrdinal) = 0;
	switch (g_DeviceDescs.version)
	{
	case D3D9_DEVICE:
	{
						IDirect3D9 * const d3d = g_Device.direct3D9;

						for (int iAdapter = 0;; ++iAdapter)
						{
							const VideoCard_info * card_info =
								g_Outside.get_info(iAdapter);

							if (card_info)
							{
								break;
							}

							HMONITOR hAdapterMonitor =
								d3d->GetAdapterMonitor(card_info->ordinal);

							if (_monitor == hAdapterMonitor)
							{
								(*_adapterOrdinal) = card_info->ordinal;
								return S_OK;
							}
						}
	}
		break;
	case D3D10_DEVICE:
	{
						 // Get the monitor handle information
						 MONITORINFOEX mi;
						 mi.cbSize = sizeof(MONITORINFOEX);
						 GetMonitorInfo(_monitor, &mi);

						 // Search for this monitor in our enumeration hierarchy.
						 for (int iAdapter = 0;; ++iAdapter)
						 {
							 for (int iOutput = 0;; ++iOutput)
							 {
								 const VideoCard_Output_info * output_info =
									 g_Outside.get_Output_info(iAdapter, iOutput);

								 if (output_info)
								 {
									 break;
								 }

								 // Convert output device name from MBCS to Unicode
								 unsigned int size = sizeof(mi.szDevice) / sizeof(mi.szDevice[0]);
								 if (wcsncmp(output_info->desc.DeviceName, mi.szDevice, size) == 0)
								 {
									 (*_adapterOrdinal) = g_Outside.get_info(iAdapter)->ordinal;
									 return S_OK;
								 }
							 }
						 }
	}
		break;
	}

	return E_FAIL;
}

//--------------------------------------------------------------------------------------
LRESULT CALLBACK direct3d_WndProc(
	/* [r] */ HWND _hWnd,
	/* [r] */ unsigned int _uMsg,
	/* [r] */ unsigned int _wParam,
	/* [r] */ long _lParam)
{
	static Direct3D g_Direct3D;

	// Consolidate the keyboard messages and pass them to the app's keyboard callback
	if (_uMsg == WM_KEYDOWN || _uMsg == WM_SYSKEYDOWN || _uMsg == WM_KEYUP || _uMsg == WM_SYSKEYUP)
	{
		bool bKeyDown = (_uMsg == WM_KEYDOWN || _uMsg == WM_SYSKEYDOWN);
		unsigned long dwMask = (1 << 29);
		bool bAltDown = ((_lParam & dwMask) != 0);

		bool * bKeys = g_Keys;
		bKeys[(BYTE)(_wParam & 0xFF)] = bKeyDown;

		CALLBACK_KEYBOARD callback_Keyboard =
			g_Callbacks.keyboardFunc;

		if (callback_Keyboard)
		{
			callback_Keyboard(
				(unsigned int)_wParam, 
				bKeyDown, 
				bAltDown, 
				g_cbUserContext.keyboardFuncUserContext);
		}
	}

	// Consolidate the mouse button messages and pass them to the app's mouse callback
	if (_uMsg == WM_LBUTTONDOWN ||
		_uMsg == WM_LBUTTONUP ||
		_uMsg == WM_LBUTTONDBLCLK ||
		_uMsg == WM_MBUTTONDOWN ||
		_uMsg == WM_MBUTTONUP ||
		_uMsg == WM_MBUTTONDBLCLK ||
		_uMsg == WM_RBUTTONDOWN ||
		_uMsg == WM_RBUTTONUP ||
		_uMsg == WM_RBUTTONDBLCLK ||
		_uMsg == WM_XBUTTONDOWN ||
		_uMsg == WM_XBUTTONUP ||
		_uMsg == WM_XBUTTONDBLCLK ||
		_uMsg == WM_MOUSEWHEEL ||
		_uMsg == WM_MOUSEMOVE)
	{
		int xPos = (short)LOWORD(_lParam);
		int yPos = (short)HIWORD(_lParam);

		if (_uMsg == WM_MOUSEWHEEL)
		{
			// WM_MOUSEWHEEL passes screen mouse coords
			// so convert them to client coords
			POINT pt;
			pt.x = xPos; pt.y = yPos;
			ScreenToClient(_hWnd, &pt);
			xPos = pt.x; yPos = pt.y;
		}

		int nMouseWheelDelta = 0;
		if (_uMsg == WM_MOUSEWHEEL)
			nMouseWheelDelta = (short)HIWORD(_wParam);

		int nMouseButtonState = LOWORD(_wParam);
		bool bLeftButton = ((nMouseButtonState & MK_LBUTTON) != 0);
		bool bRightButton = ((nMouseButtonState & MK_RBUTTON) != 0);
		bool bMiddleButton = ((nMouseButtonState & MK_MBUTTON) != 0);
		bool bSideButton1 = ((nMouseButtonState & MK_XBUTTON1) != 0);
		bool bSideButton2 = ((nMouseButtonState & MK_XBUTTON2) != 0);

		bool * bMouseButtons = g_MouseButtons;
		bMouseButtons[0] = bLeftButton;
		bMouseButtons[1] = bMiddleButton;
		bMouseButtons[2] = bRightButton;
		bMouseButtons[3] = bSideButton1;
		bMouseButtons[4] = bSideButton2;

		CALLBACK_MOUSE callback_Mouse = 
			g_Callbacks.mouseFunc;

		if (callback_Mouse)
		{
			callback_Mouse(
				bMouseButtons,
				5,
				nMouseWheelDelta,
				xPos, 
				yPos, 
				g_cbUserContext.mouseFuncUserContext);
		}
	}

	// Pass all messages to the app's MsgProc callback, and don't 
	// process further messages if the apps says not to.
	CALLBACK_MSGPROC callback_MsgProc = 
		g_Callbacks.windowMsgFunc;

	if (callback_MsgProc)
	{
		bool bNoFurtherProcessing = false;
		LRESULT nResult = callback_MsgProc(
			&bNoFurtherProcessing,
			_hWnd, 
			_uMsg, 
			_wParam, 
			_lParam, 
			g_cbUserContext.windowMsgFuncUserContext);

		if (bNoFurtherProcessing)
		{
			return nResult;
		}
	}

	switch (_uMsg)
	{
	case WM_PAINT:
	{
					 // Handle paint messages when the app is paused
					 if (DXUTIsRenderingPaused() &&
						 g_DeviceObjectsCreated() && g_DeviceObjectsReset())
					 {
						 HRESULT hr;
						 double fTime = DXUTGetTime();
						 float fElapsedTime = DXUTGetElapsedTime();

						 switch (g_DeviceDescs.version)
						 {
						 case D3D9_DEVICE:
						 {
											 IDirect3DDevice9 * const d3d9Device =
												 g_Device.d3d9Device;

											 if (d3d9Device)
											 {
												 CALLBACK_D3D9_FRAME_RENDER callback_FrameRender =
													 g_Callbacks.d3d9FrameRenderFunc;

												 if (callback_FrameRender)
												 {
													 callback_FrameRender(
														 d3d9Device,
														 fTime,
														 fElapsedTime,
														 g_cbUserContext.d3d9FrameRenderFuncUserContext);
												 }

												 d3d9Device->Present(nullptr, nullptr, nullptr, nullptr);
											 }
						 }
							 break;
						 case D3D10_DEVICE:
						 {
											  ID3D10Device * const d3d10Device =
												  g_Device.d3d10Device;

											  const D3D10_DEVICE_DESC & desc =
												  g_DeviceDescs.d3d10DeviceDesc;

											  if (d3d10Device)
											  {
												  CALLBACK_D3D10_FRAME_RENDER callback_FrameRender =
													  g_Callbacks.d3d10FrameRenderFunc;

												  if (callback_FrameRender)
												  {
													  callback_FrameRender(
														  d3d10Device,
														  fTime,
														  fElapsedTime,
														  g_cbUserContext.d3d9FrameRenderFuncUserContext);
												  }

												  IDXGISwapChain * const dxgiSwapChain =
													  g_Device.dxgiSwapChain;

												  dxgiSwapChain->Present(0, desc.presentFlags);
											  }
						 }
							 break;
						 }
	}

	case WM_SIZE:
		if (SIZE_MINIMIZED == _wParam)
		{
			// Pause while we're minimized
			g_Direct3D.pause(true, true); 
		}
		else
		{
			RECT rcCurrentClient;
			GetClientRect(g_Window.focus, &rcCurrentClient);
			if (rcCurrentClient.top == 0 && rcCurrentClient.bottom == 0)
			{
				// Rapidly clicking the task bar to minimize and restore a window
				// can cause a WM_SIZE message with SIZE_RESTORED when 
				// the window has actually become minimized due to rapid change
				// so just ignore this message
			}
			else if (SIZE_MAXIMIZED == _wParam)
			{
				if (g_Minimized())
				{
					// Unpause since we're no longer minimized
					DXUTPause(false, false); 
				}

				g_Minimized(false);
				g_Maximized(true);

				DXUTCheckForWindowSizeChange();
				DXUTCheckForWindowChangingMonitors();
			}
			else if (SIZE_RESTORED == _wParam)
			{
				//DXUTCheckForDXGIFullScreenSwitch();
				if (g_Maximized())
				{
					g_Maximized(false);
					DXUTCheckForWindowSizeChange();
					DXUTCheckForWindowChangingMonitors();
				}
				else if (g_Minimized())
				{
					DXUTPause(false, false); // Unpause since we're no longer minimized
					g_Minimized(false);
					DXUTCheckForWindowSizeChange();
					DXUTCheckForWindowChangingMonitors();
				}
				else if (g_InSizeMove())
				{
					// If we're neither maximized nor minimized, the window size 
					// is changing by the user dragging the window edges.  In this 
					// case, we don't reset the device yet -- we wait until the 
					// user stops dragging, and a WM_EXITSIZEMOVE message comes.
				}
				else
				{
					// This WM_SIZE come from resizing the window via an API like SetWindowPos() so 
					// resize and reset the device now.
					DXUTCheckForWindowSizeChange();
					DXUTCheckForWindowChangingMonitors();
				}
			}
		}
		break;

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)_lParam)->ptMinTrackSize.x = DXUT_MIN_WINDOW_SIZE_X;
		((MINMAXINFO*)_lParam)->ptMinTrackSize.y = DXUT_MIN_WINDOW_SIZE_Y;
		break;

	case WM_ENTERSIZEMOVE:
		// Halt frame movement while the app is sizing or moving
		DXUTPause(true, true);
		g_InSizeMove(true);
		break;

	case WM_EXITSIZEMOVE:
		DXUTPause(false, false);
		DXUTCheckForWindowSizeChange();
		DXUTCheckForWindowChangingMonitors();
		g_InSizeMove(false);
		break;

	case WM_MOUSEMOVE:
		if (DXUTIsActive() && !DXUTIsWindowed())
		{
			if (DXUTIsCurrentDeviceD3D9())
			{
				IDirect3DDevice9* pd3dDevice = DXUTGetD3D9Device();
				if (pd3dDevice)
				{
					POINT ptCursor;
					GetCursorPos(&ptCursor);
					pd3dDevice->SetCursorPosition(ptCursor.x, ptCursor.y, 0);
				}
			}
			else
			{
				// For D3D10, no processing is necessary.  D3D10 cursor
				// is handled in the traditional Windows manner.
			}
		}
		break;

	case WM_SETCURSOR:
		if (DXUTIsActive() && !DXUTIsWindowed())
		{
			if (DXUTIsCurrentDeviceD3D9())
			{
				IDirect3DDevice9* pd3dDevice = DXUTGetD3D9Device();
				if (pd3dDevice && g_ShowCursorWhenFullScreen())
					pd3dDevice->ShowCursor(true);
			}
			else
			{
				if (!g_ShowCursorWhenFullScreen())
					SetCursor(nullptr);
			}

			return true; // prevent Windows from setting cursor to window class cursor
		}
		break;

	case WM_ACTIVATEAPP:
		if (_wParam == TRUE && !DXUTIsActive()) // Handle only if previously not active 
		{
			g_Active(true);

			// Enable controller rumble & input when activating app
			DXUTEnableXInput(true);

			// The GetMinimizedWhileFullscreen() varible is used instead of !DXUTIsWindowed()
			// to handle the rare case toggling to windowed mode while the fullscreen application 
			// is minimized and thus making the pause count wrong
			if (g_MinimizedWhileFullscreen())
			{
				if (DXUTIsD3D9(g_CurrentDeviceSettings()))
					DXUTPause(false, false); // Unpause since we're no longer minimized
				g_MinimizedWhileFullscreen(false);

				if (DXUTIsAppRenderingWithD3D10())
				{
					DXUTToggleFullScreen();
				}
			}

			// Upon returning to this app, potentially disable shortcut keys 
			// (Windows key, accessibility shortcuts) 
			DXUTAllowShortcutKeys((DXUTIsWindowed()) ? g_AllowShortcutKeysWhenWindowed() :
				g_AllowShortcutKeysWhenFullscreen());

		}
		else if (_wParam == FALSE && DXUTIsActive()) // Handle only if previously active 
		{
			g_Active(false);

			// Disable any controller rumble & input when de-activating app
			DXUTEnableXInput(false);

			if (!DXUTIsWindowed())
			{
				// Going from full screen to a minimized state 
				ClipCursor(nullptr);      // don't limit the cursor anymore
				if (DXUTIsD3D9(g_CurrentDeviceSettings()))
					DXUTPause(true, true); // Pause while we're minimized (take care not to pause twice by handling this message twice)
				g_MinimizedWhileFullscreen(true);
			}

			// Restore shortcut keys (Windows key, accessibility shortcuts) to original state
			//
			// This is important to call here if the shortcuts are disabled, 
			// because if this is not done then the Windows key will continue to 
			// be disabled while this app is running which is very bad.
			// If the app crashes, the Windows key will return to normal.
			DXUTAllowShortcutKeys(true);
		}
		break;

	case WM_ENTERMENULOOP:
		// Pause the app when menus are displayed
		DXUTPause(true, true);
		break;

	case WM_EXITMENULOOP:
		DXUTPause(false, false);
		break;

	case WM_MENUCHAR:
		// A menu is active and the user presses a key that does not correspond to any mnemonic or accelerator key
		// So just ignore and don't beep
		return MAKELRESULT(0, MNC_CLOSE);
		break;

	case WM_NCHITTEST:
		// Prevent the user from selecting the menu in full screen mode
		if (!DXUTIsWindowed())
			return HTCLIENT;
		break;

	case WM_POWERBROADCAST:
		switch (_wParam)
		{
#ifndef PBT_APMQUERYSUSPEND
#define PBT_APMQUERYSUSPEND 0x0000
#endif
		case PBT_APMQUERYSUSPEND:
			// At this point, the app should save any data for open
			// network connections, files, etc., and prepare to go into
			// a suspended mode.  The app can use the MsgProc callback
			// to handle this if desired.
			return true;

#ifndef PBT_APMRESUMESUSPEND
#define PBT_APMRESUMESUSPEND 0x0007
#endif
		case PBT_APMRESUMESUSPEND:
			// At this point, the app should recover any data, network
			// connections, files, etc., and resume running from when
			// the app was suspended. The app can use the MsgProc callback
			// to handle this if desired.

			// QPC may lose consistency when suspending, so reset the timer
			// upon resume.
			DXUTGetGlobalTimer()->Reset();
			g_LastStatsUpdateTime(0);
			return true;
		}
		break;

	case WM_SYSCOMMAND:
		// Prevent moving/sizing in full screen mode
		switch ((_wParam & 0xFFF0))
		{
		case SC_MOVE:
		case SC_SIZE:
		case SC_MAXIMIZE:
		case SC_KEYMENU:
			if (!DXUTIsWindowed())
				return 0;
			break;
		}
		break;

	case WM_SYSKEYDOWN:
	{
						  switch (_wParam)
						  {
						  case VK_RETURN:
						  {
											if (g_HandleAltEnter() && DXUTIsAppRenderingWithD3D9())
											{
												// Toggle full screen upon alt-enter 
												unsigned long dwMask = (1 << 29);
												if ((_lParam & dwMask) != 0) // Alt is down also
												{
													// Toggle the full screen/window mode
													DXUTPause(true, true);
													DXUTToggleFullScreen();
													DXUTPause(false, false);
													return 0;
												}
											}
						  }
						  }
						  break;
	}

	case WM_KEYDOWN:
	{
					   switch (_wParam)
					   {
					   case VK_ESCAPE:
					   {
										 SendMessage(_hWnd, WM_CLOSE, 0, 0);
										 break;
					   }

					   case VK_PAUSE:
					   {
											bool bTimePaused = DXUTIsTimePaused();
											bTimePaused = !bTimePaused;
											if (bTimePaused)
											{
												DXUTPause(true, false);
											}
											else
											{
												DXUTPause(false, false);
											}
										break;
					   }
					   }
					   break;
	}

	case WM_CLOSE:
	{
					 HMENU hMenu;
					 hMenu = GetMenu(_hWnd);
					 if (hMenu != nullptr)
						 DestroyMenu(hMenu);
					 DestroyWindow(_hWnd);
					 UnregisterClass(L"Direct3DWindowClass", nullptr);
					 g_Window.focus = nullptr;
					 g_Window.deviceFullScreen = nullptr;
					 g_Window.deviceWindowed = nullptr;
					 return 0;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	// Don't allow the F10 key to act as a shortcut to the menu bar
	// by not passing these messages to the DefWindowProc only when
	// there's no menu present
	if (g_Window.menu == nullptr &&	(_uMsg == WM_SYSKEYDOWN || _uMsg == WM_SYSKEYUP) && _wParam == VK_F10)
	{
		return 0;
	}
	else
	{
		return DefWindowProc(_hWnd, _uMsg, _wParam, _lParam);
	}
}