#include <hsdk/win/frame/direct3d/direct3d.h>
#include <hsdk/win/frame/direct3d/direct3d_outside.h>
#include <hsdk/win/wintimer.h>

#define NODEBUG
#include <assert.h>
#include <list>



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
// Grobal callback function declare
//--------------------------------------------------------------------------------------

// 설명 : 
typedef long (CALLBACK *CALLBACK_CREATE_3DENVIRONMENT)(
	/* [x] */ void);

// 설명 : 
typedef long (CALLBACK *CALLBACK_RESET_3DENVIRONMENT)(
	/* [x] */ void);

// 설명 : 
typedef void (CALLBACK *CALLBACK_RENDER_3DENVIRONMENT)(
	/* [x] */ void);

// 설명 : 
typedef void (CALLBACK *CALLBACK_CLEANUP_3DENVIRONMENT)(
	/* [r] */ BOOL _releaseSettings);

//--------------------------------------------------------------------------------------
// Grobal declare
//--------------------------------------------------------------------------------------

// 설명 : 
DECL_STRUCT(Direct3D_TimeStream)
{

	// 설명 : 
	BOOL pauseTime = false;

	// 설명 : 
	BOOL pauseRendering = false;

	// 설명 : 
	int pauseTimeCount = 0;

	// 설명 : 
	int pauseRenderingCount = 0;

	// 설명 : 
	BOOL constantFrameTime = false;

	// 설명 : 
	float constantTimePerFrame = 0.0333f;

	// 설명 : 
	double time = 0.0f;

	// 설명 : 
	double absoluteTime = 0.0f;

	// 설명 : 
	float elapsedTime = 0.0f;

	// 설명 : 
	unsigned int frameCount = 0;

	// 설명 :
	float fps = 0;

	// 설명 :
	win::WINTimer timer;

};

// 설명 : 
DECL_STRUCT(Direct3D_State)
{
	// 설명 :
	volatile long runMainLoop = 0;

	// 설명 :
	bool createdDevice = false;

	// 설명 : 
	BOOL autoChangeAdapter = false;

	// 설명 : 
	BOOL is_in_GammaCorrectMode = false;

	// 설명 :
	BOOL minimizedSize = false;

	// 설명 :
	BOOL maximizedSize = false;

	// 설명 : 
	BOOL allowWhenFullscreen = false;

	// 설명 : 
	BOOL allowWhenWindowed = false;

	// 
	BOOL calledWasKeyPressed = false;

	// 설명 : 
	HHOOK keyboardHook = nullptr;

};

// 설명 : 
DECL_STRUCT(Direct3D_Dispatch)
{
	// 설명 : 
	DEVICE_VERSION version = NONE_DEVICE;

	// 설명 : 
	BOOL * refWindowed = nullptr;

	// 설명 : 
	unsigned int * refWidth = nullptr;

	// 설명 : 
	unsigned int * refHeight = nullptr;

	// 설명 : 
	unsigned int * refAdapter = nullptr;

	// 설명 : 
	CALLBACK_CREATE_3DENVIRONMENT cbCreate3DEnvironment = nullptr;

	// 설명 : 
	CALLBACK_RESET_3DENVIRONMENT cbReset3DEnvironment = nullptr;

	// 설명 : 
	CALLBACK_RENDER_3DENVIRONMENT cbRender3DEnvironment = nullptr;

	// 설명 : 
	CALLBACK_CLEANUP_3DENVIRONMENT cbCleanUp3DEnvironment = nullptr;

};

//--------------------------------------------------------------------------------------
// Grobal thread safety
//--------------------------------------------------------------------------------------

// 설명 : 
CRITICAL_SECTION g_cs;

// 설명 : 
BOOL g_bThreadSafe = true;

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

// 설명 : 
Direct3D_State g_State;

// 설명 : 
Direct3D_Dispatch g_Dispatch;

//--------------------------------------------------------------------------------------
// Grobal control variable
//--------------------------------------------------------------------------------------

// 설명 : 
long g_ExitCode = 0;

// 설명 : array of key state
BOOL g_Keys[256] = { 0 };

// 설명 : array of last key state
BOOL g_LastKeys[256] = { 0 };

// 설명 : array of mouse states
BOOL g_MouseButtons[5] = { 0 };

// 설명 : 
Direct3D_TimeStream g_TimeStream;

// 설명 :
std::list<Direct3D_TIMER_DESC> g_TimerEvents;

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

// 설명 : update dispatch function in g_Dispatch
DECL_FUNC_T(void, initialize_Dispatch)(
	/* [r] */ DEVICE_VERSION _version);

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
DECL_FUNC_T(void, render_3DEnvironment9)(
	/* [x] */ void);

/*
설명: Cleans up the 3D environment in g_Device by :
- Calls the device lost callback
- Calls the device destroyed callback
- Releases the D3D device
*/
DECL_FUNC_T(void, cleanup_3DEnvironment9)(
	/* [r] */ BOOL _releaseSettings);

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
	/* [r] */ BOOL _releaseSettings = true);

DECL_FUNC_T(void, render_3DEnvironment10)(
	/* [x] */ void);

// 설명 : Sets the viewport, creates a render target view, and depth scencil texture and view in g_Device.
DECL_FUNC(setup_D3D10Views)(
	/* [r] */ const D3D10_DEVICE_DESC & _desc);

// 설명 : resize back buffer surface desc in g_Device.10
DECL_FUNC_T(void, resize_DXGIBuffers)(
	/* [r] */ unsigned int _width,
	/* [r] */ unsigned int _height,
	/* [r] */ BOOL _fullScreen);

// 설명 : Stores back buffer surface desc in g_Device
DECL_FUNC(update_BackBufferDesc)(
	/* [r] */ DEVICE_VERSION _version);

/*
설명 : Enables/disables Windows keys, and disables or restores the StickyKeys/ToggleKeys/FilterKeys
shortcut to help prevent accidental task switching.
*/
DECL_FUNC_T(void, allow_ShortcutKeys)(
	/* [r] */ BOOL _allowKeys);

// 설명 : Handles window messages 
LRESULT CALLBACK direct3D_WndProc(
	/* [r] */ HWND _hWnd,
	/* [r] */ unsigned int _uMsg,
	/* [r] */ unsigned int _wParam,
	/* [r] */ long _lParam);

// 설명 : Low level keyboard hook to disable Windows key to prevent accidental task switching. 
LRESULT CALLBACK direct3D_LowLevelKeyboardProc(
	/* [r] */ int _code,
	/* [r] */ WPARAM _wParam,
	/* [r] */ LPARAM _lParam);

//--------------------------------------------------------------------------------------
// initialize task
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D, initialize_Default)(
	/* [r] */ BOOL _parseCommandLine,
	/* [r] */ BOOL _showMsgBoxOnError,
	/* [r] */ __in_opt wchar_t * _strExtraCommandLineParams,
	/* [r] */ BOOL _threadSafe)
{
	g_bThreadSafe = _threadSafe;

	// Not always needed, but lets the app create GDI dialogs
	InitCommonControls();

	// Save the current sticky/toggle/filter key settings so DXUT can restore them later
	g_init.stickyKeys = { sizeof(STICKYKEYS), 0 };
	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &g_init.stickyKeys, 0);

	g_init.toggleKeys = { sizeof(TOGGLEKEYS), 0 };
	SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_init.toggleKeys, 0);

	g_init.filterKeys = { sizeof(FILTERKEYS), 0 };
	SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &g_init.filterKeys, 0);

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
	SetProcessDPIAware();

	// Reset the timer
	g_TimeStream.timer.reset();

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D, initialize_Window)(
	/* [r] */ const wchar_t * _strWindowTitle,
	/* [r] */ int _x,
	/* [r] */ int _y,
	/* [r] */ HINSTANCE _hInstance,
	/* [r] */ HICON _hIcon,
	/* [r] */ HMENU _hMenu)
{
	IF_INVALID(_hInstance)
	{
		_hInstance = (HINSTANCE)GetModuleHandle(nullptr);
	}

	WCHAR szExePath[MAX_PATH];
	GetModuleFileName(nullptr, szExePath, MAX_PATH);

	// If the icon is nullptr, then use the first one found in the exe
	IF_INVALID(_hIcon)
	{
		_hIcon = ExtractIcon(_hInstance, szExePath, 0);
	}

	// Register the windows class
	WNDCLASS wndClass;
	wndClass.style = CS_DBLCLKS;
	wndClass.lpfnWndProc = direct3D_WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = _hInstance;
	wndClass.hIcon = _hIcon;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = L"Direct3DWindowClass";

	IF_FALSE(RegisterClass(&wndClass))
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

	RECT rc = {
		0,
		0,
		nDefaultWidth,
		nDefaultHeight
	};

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

	IF_INVALID(hWnd)
	{
		unsigned long dwError = GetLastError();
		return ADD_FLAG(HSDK_FAIL, dwError);
	}

	HRESULT hr;
	IF_FAILED(hr = userSet_Window(hWnd, hWnd, hWnd, false))
	{
		DestroyWindow(hWnd);
	}

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D, userSet_Window)(
	/* [in] */ HWND _focus,
	/* [in] */ HWND _fullScreen,
	/* [in] */ HWND _windowScreen,
	/* [r] */ BOOL _handleMessages)
{
	if (g_Window.focus || g_Window.fullScreen || g_Window.windowScreen)
	{
		return E_ACCESSDENIED;
	}

	// To avoid confusion, we do not allow any HWND to be nullptr here.  The
	// caller must pass in valid HWND for all three parameters.  The same
	// HWND may be used for more than one parameter.
	if (nullptr == _focus || nullptr == _fullScreen || nullptr == _windowScreen)
	{
		return E_INVALIDARG;
	}

	// If subclassing the window, set the pointer to the local window procedure
	if (_handleMessages)
	{
		// Switch window procedures
		LONG_PTR nResult;
		IF_FALSE(nResult = SetWindowLongPtr(
			_focus,
			GWLP_WNDPROC,
			(LONG_PTR)direct3D_WndProc))
		{
			return ADD_FLAG(HSDK_FAIL, HRESULT_FROM_WIN32(GetLastError()));
		}
	}

	// 타임 카운트 정지
	pause_Time(true);

	wchar_t * strCachedWindowTitle = g_Window.windowTitle;
	GetWindowText(_focus, strCachedWindowTitle, 255);
	strCachedWindowTitle[255] = 0;

	g_Window.HINSTANCE = (HINSTANCE)(LONG_PTR)GetWindowLongPtr(_focus, GWLP_HINSTANCE);
	g_Window.focus = _focus;
	g_Window.fullScreen = _fullScreen;
	g_Window.windowScreen = _windowScreen;

	// 타임 카운트 시작
	pause_Time(false);

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D, dynamic_WndProc)(
	/* [r] */ HWND _hWnd,
	/* [r] */ unsigned int _uMsg,
	/* [r] */ unsigned int _wParam,
	/* [r] */ long _lParam)
{
	return direct3D_WndProc(_hWnd, _uMsg, _wParam, _lParam);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D, initialize_Device)(
	/* [r] */ BOOL _windowed,
	/* [r] */ int _suggestedWidth,
	/* [r] */ int _suggestedHeight)
{
	Direct3D_DeviceMatchOptions deviceMatOpt =
		create_DeviceMatOpt_FrominitDesc(g_init);

	if (deviceMatOpt.eResolution == IGNORE_INPUT)
	{
		if (_windowed || (_suggestedWidth != 0 && _suggestedHeight != 0))
		{
			deviceMatOpt.eResolution = CLOSEST_TO_INPUT;
		}
	}

	// Building D3D9 device settings for mathch options.  These
	// will be converted to D3D10 settings if app can use D3D10	
	Direct3D_DeviceDescs descs;
	descs.d3d9DeviceDesc = create_DeviceDesc_FrominitDesc(
		g_init,
		_windowed,
		_suggestedWidth,
		_suggestedHeight);

	descs.d3d9DeviceDesc.pp.hDeviceWindow =
		descs.d3d9DeviceDesc.pp.Windowed ? g_Window.windowScreen : g_Window.fullScreen;

	if (g_init.forceAPI == -1)
	{
		if (g_Callbacks.isD3D10DeviceAcceptableFunc ||
			g_Callbacks.d3d10DeviceCreatedFunc ||
			g_Callbacks.d3d10SwapChainResizedFunc ||
			g_Callbacks.d3d10FrameRenderFunc ||
			g_Callbacks.d3d10SwapChainReleasingFunc ||
			g_Callbacks.d3d10DeviceDestroyedFunc)
		{
			g_init.forceAPI = 10;
		}
		else if (g_Callbacks.isD3D9DeviceAcceptableFunc ||
			g_Callbacks.d3d9DeviceCreatedFunc ||
			g_Callbacks.d3d9DeviceResetFunc ||
			g_Callbacks.d3d9DeviceLostFunc ||
			g_Callbacks.d3d9DeviceDestroyedFunc ||
			g_Callbacks.d3d9FrameRenderFunc)
		{
			g_init.forceAPI = 9;
		}
	}

	BOOL bContinue = true;
	if (g_init.forceAPI == 9)
	{
		IDirect3D9 * d3d9 = get_D3D9();
		IF_INVALID(d3d9)
		{
			assert(L"Direct3DERR_NODIRECT3D");
			return E_ABORT;
		}

		D3D9_DEVICE_DESC & desc = descs.d3d9DeviceDesc;
		build_OptimalDeviceDesc(desc, desc, deviceMatOpt);

		HRESULT hr;
		D3DCAPS9 caps;
		IF_FAILED(hr = d3d9->GetDeviceCaps(
			desc.adapterOrdinal,
			desc.deviceType,
			&caps))
		{
			return hr;
		}

		CALLBACK_IS_D3D9_DEVICE_ACCEPTABLE callback_acceptable =
			g_Callbacks.isD3D9DeviceAcceptableFunc;

		if (callback_acceptable)
		{
			bContinue = callback_acceptable(
				caps,
				desc.adapterFormat,
				desc.pp.BackBufferFormat,
				desc.pp.Windowed,
				g_cbUserContext.isD3D9DeviceAcceptableFuncUserContext);
		}

		descs.version = D3D9_DEVICE;
	}
	else if (g_init.forceAPI == 10)
	{
		IF_INVALID(get_DXGIFactory())
		{
			assert(L"Direct3DERR_NODIRECT3D");
			return E_ABORT;
		}

		// Reset the Outside
		g_Outside.initialize(get_DXGIFactory());

		D3D10_DEVICE_DESC desc = { 0 };
		convert_DeviceDesc_9to10(desc, descs.d3d9DeviceDesc);
		build_OptimalDeviceDesc(desc, desc, deviceMatOpt);

		CALLBACK_IS_D3D10_DEVICE_ACCEPTABLE callback_acceptable =
			g_Callbacks.isD3D10DeviceAcceptableFunc;

		if (callback_acceptable)
		{
			bContinue = callback_acceptable(
				desc.adapterOrdinal,
				desc.output,
				desc.driverType,
				desc.sd.BufferDesc.Format,
				desc.sd.Windowed,
				g_cbUserContext.isD3D10DeviceAcceptableFuncUserContext);
		}

		descs.d3d10DeviceDesc = desc;
		descs.version = D3D10_DEVICE;
	}

	IF_FALSE(bContinue)
	{
		return E_FAIL;
	}

	// Change to a Direct3D device created from the new device settings.  
	// If there is an existing device, then either reset or recreated the scene
	return userSet_Device(descs);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D, userSet_Device)(
	/* [r] */ const Direct3D_DeviceDescs & _newDescs,
	/* [r] */ BOOL _clipWindowToSingleAdapter,
	/* [r] */ BOOL _resetRecovery)
{
	g_State.createdDevice = false;
	HRESULT hr = E_FAIL;

	// Make a copy of the pNewDeviceSettings on the heap
	Direct3D_DeviceDescs newDesc;
	memcpy(&newDesc, &_newDescs, sizeof(Direct3D_DeviceDescs));

	// If the ModifyDeviceSettings callback is non-NULL, then call it to let the app 
	// change the settings or reject the device change by returning false.
	CALLBACK_MODIFY_DEVICE_SETTINGS callback_ModifyDeviceSettings =
		g_Callbacks.modifyDeviceSettingsFunc;

	if (callback_ModifyDeviceSettings)
	{
		IF_FALSE(callback_ModifyDeviceSettings(
			newDesc,
			g_cbUserContext.modifyDeviceSettingsFuncUserContext))
		{
			return E_ABORT;
		}
	}

	if (newDesc.version == NONE_DEVICE)
	{
		return E_INVALIDARG;
	}

	const Direct3D_DeviceDescs currentDesc_Temp;
	memcpy((void *)&currentDesc_Temp, &g_DeviceDescs, sizeof(Direct3D_DeviceDescs));

	// 랜더링 정지
	pause_Rendering(true);

	// 디바이스 상태 갱신
	memcpy(&g_DeviceDescs, &newDesc, sizeof(Direct3D_DeviceDescs));

	// 빠른 경로 업데이트
	initialize_Dispatch(newDesc.version);

	// Make a window rect with a client rect that is the same size as the backbuffer
	if (*g_Dispatch.refWindowed)
	{
		RECT window;
		GetWindowRect(g_Window.windowScreen, &window);

		RECT rcWindow = { 0 };
		rcWindow.right = (*g_Dispatch.refWidth);
		rcWindow.bottom = (*g_Dispatch.refHeight);

		AdjustWindowRect(
			&rcWindow,
			GetWindowLong(g_Window.windowScreen, GWL_STYLE),
			g_Window.menu != nullptr);

		// Resize the window.  It is important to adjust the window size 
		// after resetting the device rather than beforehand to ensure 
		// that the monitor resolution is correct and does not limit the size of the new window.
		window.right = (int)(rcWindow.right - rcWindow.left);
		window.bottom = (int)(rcWindow.bottom - rcWindow.top);

		if (_clipWindowToSingleAdapter)
		{
			// Get the rect of the monitor attached to the adapter
			MONITORINFO miAdapter;
			miAdapter.cbSize = sizeof(MONITORINFO);

			if (newDesc.version == D3D9_DEVICE)
			{
				GetMonitorInfo(g_Device.direct3D9->GetAdapterMonitor(newDesc.d3d9DeviceDesc.adapterOrdinal), &miAdapter);
			}
			else
			{
				// NONE_DEVICE 는 이 분기에 들어올 수 없음
				GetMonitorInfo(g_Outside.get_MonitorFromAdapter(newDesc.d3d10DeviceDesc), &miAdapter);
			}

			// Do something reasonable if the BackBuffer size is greater than the monitor size
			int nAdapterMonitorWidth = miAdapter.rcWork.left + miAdapter.rcWork.right;
			int nAdapterMonitorHeight = miAdapter.rcWork.top + miAdapter.rcWork.bottom;

			int empty_x = window.left - miAdapter.rcWork.left;
			int empty_y = window.top - miAdapter.rcWork.top;

			if (window.left < miAdapter.rcWork.left)
			{
				window.left = miAdapter.rcWork.left;
				empty_x = 0;
			}

			if (window.top < miAdapter.rcWork.top)
			{
				window.top = miAdapter.rcWork.top;
				empty_y = 0;
			}

			int cx = window.left + window.right;
			if (nAdapterMonitorWidth < cx)
			{
				int dw = cx - nAdapterMonitorWidth;
				if (empty_x < dw)
				{
					window.left -= empty_x;
					dw -= empty_x;

					window.right -= dw;
					(*g_Dispatch.refWidth) -= dw;
				}
				else
				{
					window.left -= dw;
				}
			}

			int cy = window.top + window.bottom;
			if (nAdapterMonitorHeight < cy)
			{
				int dh = cy - nAdapterMonitorHeight;
				if (empty_y < dh)
				{
					window.top -= empty_y;
					dh -= empty_y;

					window.bottom -= dh;
					(*g_Dispatch.refHeight) -= dh;
				}
				else
				{
					window.top -= dh;
				}
			}
		}

		SetWindowPos(
			g_Window.windowScreen,
			0,
			window.left,
			window.top,
			window.right,
			window.bottom,
			SWP_NOZORDER);
	}
	else
	{
		// Get the rect of the monitor attached to the adapter
		if (newDesc.version == D3D9_DEVICE)
		{
			D3DDISPLAYMODE mode;
			IF_FAILED(hr = g_Device.direct3D9->GetAdapterDisplayMode(newDesc.d3d9DeviceDesc.adapterOrdinal, &mode))
			{
				return hr;
			}

			// Do something reasonable if the BackBuffer size is greater than the monitor size
			(*g_Dispatch.refWidth) = mode.Width;
			(*g_Dispatch.refHeight) = mode.Height;
		}
		else
		{
			const VideoCard_Output_info * output_info = g_Outside.get_Output_info(
				newDesc.d3d10DeviceDesc.adapterOrdinal,
				newDesc.d3d10DeviceDesc.output);

			IF_INVALID(output_info)
			{
				return E_INVALIDARG;
			}

			const DXGI_OUTPUT_DESC & Desc =
				output_info->desc;

			(*g_Dispatch.refWidth) = Desc.DesktopCoordinates.right - Desc.DesktopCoordinates.left;
			(*g_Dispatch.refHeight) = Desc.DesktopCoordinates.bottom - Desc.DesktopCoordinates.top;
		}

	}

	// If API version, AdapterOrdinal and DeviceType are the same, we can just do a Reset().
	// If they've changed, we need to do a complete device tear down/rebuild.
	// Also only allow a reset if pd3dDevice is the same as the current device 
	if (is_ResetDevice(currentDesc_Temp, newDesc))
	{
		// Reset the Direct3D device and call the app's device callbacks
		IF_FAILED(hr = g_Dispatch.cbReset3DEnvironment())
		{
			// 리셋 복구
			if (_resetRecovery)
			{
				// 복구를 원하는 경우
				IF_SUCCEEDED(userSet_Device(currentDesc_Temp, _clipWindowToSingleAdapter))
				{
					// 이 루트는 userSet_Device 함수가 1번 끝을 보지 못한 상태이기 때문에 이전 pause를 해제
					pause_Rendering(true);
				}
			}
			else
			{
				// 복구를 원치 않는 경우
				assert(L"Direct3DERR_RESETTINGDEVICE : reset_3DEnvironment");
			}

			return hr;
		}
	}
	else
	{
		// Reset the Direct3D device and call the app's device callbacks
		IF_FAILED(hr = g_Dispatch.cbCreate3DEnvironment())
		{
			assert(L"Direct3DERR_RESETTINGDEVICE : create_3DEnvironment");
			return hr;
		}
	}

	if (g_State.minimizedSize)
	{
		// Need to resize, so if window is maximized or minimized then restore the window
		if (IsIconic(g_Window.windowScreen))
		{
			ShowWindow(g_Window.windowScreen, SW_RESTORE);
		}
	}

	if (g_State.maximizedSize)
	{
		// doing the IsIconic() check first also handles the WPF_RESTORETOMAXIMIZED case
		if (IsZoomed(g_Window.windowScreen))
		{
			ShowWindow(g_Window.windowScreen, SW_RESTORE);
		}
	}

	// Make the window visible
	if (!IsWindowVisible(g_Window.focus))
	{
		ShowWindow(g_Window.focus, SW_SHOW);
	}

	// Ensure that the display doesn't power down when fullscreen but does when windowed
	if (*g_Dispatch.refWindowed)
	{
		SetThreadExecutionState(ES_DISPLAY_REQUIRED | ES_CONTINUOUS);
	}
	else
	{
		SetThreadExecutionState(ES_CONTINUOUS);
	}

	// 랜더링 시작
	pause_Rendering(false);
	g_State.createdDevice = true;

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, destroy)(
	/* [x] */ void)
{
	g_Outside.destroy();
	this->shutdown();

	// all clear
	g_init = Direct3D_INIT_DESC();
	g_State = Direct3D_State();
	g_Callbacks = Direct3D_Callbacks();
	g_cbUserContext = Direct3D_Callback_UserContexts();
	g_TimeStream = Direct3D_TimeStream();
	g_DeviceDescs = Direct3D_DeviceDescs();
	g_Dispatch = Direct3D_Dispatch();
}

//--------------------------------------------------------------------------------------
// Active task
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D, mainLoop)(
	/* [r] */ HACCEL _accel)
{
	if (TRUE == InterlockedCompareExchange(&g_State.runMainLoop, TRUE, FALSE))
	{
		return S_FALSE;
	}

	const HWND & hWnd = g_Window.focus;

	float oneSecond = 0.0f;
	unsigned int oneSecondCount = 0;

	// Now we're ready to receive and process Windows messages.
	MSG msg;

	msg.message = WM_NULL;
	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			// Translate and dispatch the message
			IF_FALSE(TranslateAccelerator(hWnd, _accel, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		IF_FALSE(g_TimeStream.pauseTime)
		{
			// Get the app's time, in seconds. Skip rendering if no time elapsed
			double fTime, fAbsTime; float fElapsedTime;
			g_TimeStream.timer.get_TimeValues(&fTime, &fAbsTime, &fElapsedTime);

			// Store the time for the app
			if (g_TimeStream.constantFrameTime)
			{
				fElapsedTime = g_TimeStream.constantTimePerFrame;
				fTime += fElapsedTime;
			}

			g_TimeStream.time = fTime;
			g_TimeStream.absoluteTime = fAbsTime;
			g_TimeStream.elapsedTime = fElapsedTime;

			if (oneSecond < 1.0f)
			{
				++oneSecondCount;
				oneSecond += fElapsedTime;
			}
			else
			{
				g_TimeStream.fps = (float)oneSecondCount / oneSecond;

				oneSecondCount = 1;
				oneSecond -= 1.0f;
			}

			auto begin = g_TimerEvents.begin();
			auto end = g_TimerEvents.end();

			for (auto iter = begin; iter != end; ++iter)
			{
				IF_FALSE(iter->bEnabled)
				{
					iter = g_TimerEvents.erase(iter);
					continue;
				}

				iter->fCountdown -= fElapsedTime;

				// Call the callback if count down expired
				if (iter->fCountdown < 0)
				{
					iter->bEnabled =
						iter->callbackTimer(
						iter->nID,
						iter->callbackUserContext);

					iter->fCountdown = iter->fTimeoutInSecs;
				}
			}

			// Animate the scene by calling the app's frame move callback
			CALLBACK_FRAMEMOVE callback_FrameMove =
				g_Callbacks.frameMoveFunc;

			if (callback_FrameMove)
			{
				callback_FrameMove(
					fTime,
					fElapsedTime,
					g_cbUserContext.frameMoveFuncUserContext);
			}
		}

		IF_FALSE(g_TimeStream.pauseRendering)
		{
			// Render a frame during idle time (no messages are waiting)
			g_Dispatch.cbRender3DEnvironment();

			// Update current frame #
			++g_TimeStream.frameCount;
		}

		// If the app called DXUTWasKeyPressed() then do the work 
		// to store the current state of the keys in bLastKeys
		if (g_State.calledWasKeyPressed)
		{
			memcpy(g_LastKeys, g_Keys, sizeof(BOOL)* 256);
		}
	}

	// Cleanup the accelerator table
	if (_accel)
	{
		DestroyAcceleratorTable(_accel);
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Common tasks 
//--------------------------------------------------------------------------------------

// 설명 : 
//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, shutdown)(
	/* [r] */ int _exitCode)
{
	const HWND & hWnd = g_Window.focus;
	IF_INVALID(hWnd)
	{
		SendMessage(hWnd, WM_CLOSE, 0, 0);
	}
	g_ExitCode = _exitCode;

	g_Direct3D.pause_Time(true);
	g_Direct3D.pause_Rendering(true);
	g_Dispatch.cbCleanUp3DEnvironment(true);

	// Restore shortcut keys (Windows key, accessibility shortcuts) to original state
	// This is important to call here if the shortcuts are disabled, 
	// because accessibility setting changes are permanent.
	// This means that if this is not done then the accessibility settings 
	// might not be the same as when the app was started. 
	// If the app crashes without restoring the settings, this is also true so it
	// would be wise to backup/restore the settings from a file so they can be 
	// restored when the crashed app is run again.
	allow_ShortcutKeys(true);

	// all clear
	g_Device = Direct3D_Device();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, pause_Time)(
	/* [r] */ BOOL _pauseTime)
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

	if (0 < nPauseTimeCount)
	{
		nPauseTimeCount = 0;
	}

	g_TimeStream.pauseTimeCount = nPauseTimeCount;

	if (0 < nPauseTimeCount)
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
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, pause_Rendering)(
	/* [r] */ BOOL _pauseRendering)
{
	int nPauseRenderingCount = g_TimeStream.pauseRenderingCount;
	if (g_TimeStream.pauseRendering)
	{
		++nPauseRenderingCount;
	}
	else
	{
		--nPauseRenderingCount;
	}

	if (0 < nPauseRenderingCount)
	{
		nPauseRenderingCount = 0;
	}

	g_TimeStream.pauseRendering = nPauseRenderingCount > 0;

	g_TimeStream.pauseRenderingCount = nPauseRenderingCount;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_ConstantFrameTime)(
	/* [r] */ BOOL _constantFrameTime,
	/* [r] */ float _fTimePerFrame)
{
	g_TimeStream.constantFrameTime = _constantFrameTime;
	g_TimeStream.constantTimePerFrame = _fTimePerFrame;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D, set_Timer)(
	/* [r] */ CALLBACK_TIMER _callbackTimer,
	/* [r] */ float _fTimeoutInSecs,
	/* [r] */ unsigned int * _nIDEvent,
	/* [r] */ void * _userContext)
{
	static unsigned int timerCount = 0;

	Direct3D_TIMER_DESC timer;
	timer.callbackTimer = _callbackTimer;
	timer.callbackUserContext = _userContext;
	timer.fTimeoutInSecs = _fTimeoutInSecs;
	timer.fCountdown = _fTimeoutInSecs;
	timer.nID = timerCount++;

	try
	{
		g_TimerEvents.push_back(timer);
	}
	catch (...)
	{
		return E_FAIL;
	}

	if (_nIDEvent)
	{
		(*_nIDEvent) = timer.nID;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D, kill_Timer)(
	/* [r] */ unsigned int _nIDEvent)
{
	auto begin = g_TimerEvents.begin();
	auto end = g_TimerEvents.end();

	for (auto iter = begin; iter != end; ++iter)
	{
		if (iter->nID == _nIDEvent)
		{
			iter->bEnabled = false;
			return S_OK;
		}
	}

	return S_FALSE;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_ShortcutKeySettings)(
	/* [r] */ BOOL _allowWhenFullscreen,
	/* [r] */ BOOL _allowWhenWindowed)
{
	g_State.allowWhenFullscreen = _allowWhenFullscreen;
	g_State.allowWhenWindowed = _allowWhenWindowed;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, change_Monitor)(
	/* [r] */ BOOL _windowed,
	/* [r] */ int _suggestedWidth,
	/* [r] */ int _suggestedHeight,
	/* [r] */ unsigned int _adapter)
{
	if (_adapter == -1)
	{
		HMONITOR const hWindowMonitor = MonitorFromWindow(g_Window.focus, MONITOR_DEFAULTTOPRIMARY);
		HMONITOR const hAdapterMonitor = g_Window.adapterMonitor;

		if (hWindowMonitor != hAdapterMonitor)
		{
			IDirect3D9 * d3d9 = nullptr;
			if (g_DeviceDescs.version == D3D9_DEVICE)
			{
				d3d9 = g_Device.direct3D9;
			}

			UINT newOrdinal;
			if (SUCCEEDED(newOrdinal = g_Outside.get_AdapterOrdinalFromMonitor(hWindowMonitor, d3d9)))
			{
				change_Monitor(_windowed, _suggestedWidth, _suggestedHeight, newOrdinal);
			}
		}
	}
	else
	{
		// Find the closest valid device settings with the new ordinal
		Direct3D_DeviceDescs deviceDesc = g_DeviceDescs;

		Direct3D_DeviceMatchOptions matchOptions;
		matchOptions.eAPIVersion = PRESERVE_INPUT;
		matchOptions.eAdapterOrdinal = PRESERVE_INPUT;
		matchOptions.eDeviceType = CLOSEST_TO_INPUT;
		matchOptions.eWindowed = CLOSEST_TO_INPUT;
		matchOptions.eAdapterFormat = CLOSEST_TO_INPUT;
		matchOptions.eVertexProcessing = CLOSEST_TO_INPUT;
		matchOptions.eResolution = CLOSEST_TO_INPUT;
		matchOptions.eBackBufferFormat = CLOSEST_TO_INPUT;
		matchOptions.eBackBufferCount = CLOSEST_TO_INPUT;
		matchOptions.eMultiSample = CLOSEST_TO_INPUT;
		matchOptions.eSwapEffect = CLOSEST_TO_INPUT;
		matchOptions.eDepthFormat = CLOSEST_TO_INPUT;
		matchOptions.eStencilFormat = CLOSEST_TO_INPUT;
		matchOptions.ePresentFlags = CLOSEST_TO_INPUT;
		matchOptions.eRefreshRate = CLOSEST_TO_INPUT;
		matchOptions.ePresentInterval = CLOSEST_TO_INPUT;

		if (deviceDesc.version == D3D9_DEVICE)
		{
			D3D9_DEVICE_DESC & desc = deviceDesc.d3d9DeviceDesc;

			desc.adapterOrdinal = _adapter;
			desc.pp.Windowed = _windowed;
			desc.pp.BackBufferWidth = _suggestedWidth;
			desc.pp.BackBufferHeight = _suggestedHeight;

			build_OptimalDeviceDesc(
				desc,
				desc,
				matchOptions);

			desc.pp.hDeviceWindow =
				desc.pp.Windowed ? g_Window.windowScreen : g_Window.fullScreen;
		}
		else if (deviceDesc.version = D3D10_DEVICE)
		{
			D3D10_DEVICE_DESC & desc = deviceDesc.d3d10DeviceDesc;

			desc.adapterOrdinal = _adapter;
			desc.sd.Windowed = _windowed;
			desc.sd.BufferDesc.Width = _suggestedWidth;
			desc.sd.BufferDesc.Height = _suggestedHeight;

			unsigned int newOutput;
			IF_SUCCEEDED(newOutput = g_Outside.get_OutputOrdinalFromMonitor(
				MonitorFromWindow(g_Window.focus, MONITOR_DEFAULTTOPRIMARY)))
			{
				desc.output = newOutput;
			}

			build_OptimalDeviceDesc(
				desc,
				desc,
				matchOptions);

			desc.sd.OutputWindow =
				desc.sd.Windowed ? g_Window.windowScreen : g_Window.fullScreen;
		}

		// Create a Direct3D device using the new device settings.  
		// If there is an existing device, then it will either reset or recreate the scene.
		g_Direct3D.userSet_Device(deviceDesc, true, true);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_AutoChangeMoniter)(
	/* [r] */ BOOL _autoChange)
{
	g_State.autoChangeAdapter = _autoChange;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_GammaCorrectMode)(
	/* [r] */ BOOL _gammaCorrect)
{
	g_State.is_in_GammaCorrectMode = _gammaCorrect;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Vsync)(
	/* [r] */ BOOL _vsync)
{
	assert(L"미구현");
}


//--------------------------------------------------------------------------------------
// General
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, const wchar_t *, get_Window_Title)(
	/* [x] */ void)const
{
	return g_Window.windowTitle;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, HINSTANCE, get_HINSTANCE)(
	/* [x] */ void)const
{
	return g_Window.HINSTANCE;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, HWND, get_HWND_Focus)(
	/* [x] */ void)const
{
	return g_Window.focus;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, HWND, get_HWND_FullScreen)(
	/* [x] */ void)const
{
	return g_Window.fullScreen;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, HWND, get_HWND_WindowScreen)(
	/* [x] */ void)const
{
	return g_Window.windowScreen;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, double, get_Time)(
	/* [x] */ void)const
{
	return g_TimeStream.time;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, float, get_ElapsedTime)(
	/* [x] */ void)const
{
	return g_TimeStream.elapsedTime;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, float, get_FPS)(
	/* [x] */ void)const
{
	return g_TimeStream.fps;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, BOOL, is_Time_Paused)(
	/* [x] */ void)const
{
	return g_TimeStream.pauseTime;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, BOOL, is_Rendering_Paused)(
	/* [x] */ void)const
{
	return g_TimeStream.pauseRendering;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, BOOL, is_Active)(
	/* [x] */ void)const
{
	return g_State.runMainLoop;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, BOOL, is_KeyDown)(
	/* [r] */ unsigned char vKey)const
{
	if (0x9f < vKey && vKey < 0xA6)
	{
		// VK_LSHIFT, VK_RSHIFT, VK_LCONTROL, VK_RCONTROL, VK_LMENU, VK_RMENU
		// these keys only are tracked via GetAsyncKeyState()
		return GetAsyncKeyState(vKey) & 0x8000;
	}
	else if (0x00 < vKey && vKey < 0x07 && vKey != 0x03)
	{
		// mouse buttons (VK_BUTTON)
		return is_MouseButtonDown(vKey);
	}
	else
	{
		return g_Keys[vKey];
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, BOOL, was_KeyPressed)(
	/* [r] */ unsigned char vKey)const
{
	return (!g_LastKeys[vKey] && g_Keys[vKey]);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, BOOL, is_MouseButtonDown)(
	/* [r] */ unsigned char vButton)const
{
	switch (vButton)
	{
	case MK_LBUTTON:
		return g_MouseButtons[0];
	case VK_MBUTTON:
	case MK_MBUTTON:
		return g_MouseButtons[1];
	case MK_RBUTTON:
		return g_MouseButtons[2];
	case VK_XBUTTON1:
	case MK_XBUTTON1:
		return g_MouseButtons[3];
	case VK_XBUTTON2:
	case MK_XBUTTON2:
		return g_MouseButtons[4];
	}

	return false;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, BOOL, is_Windowed)(
	/* [x] */ void)const
{
	return *g_Dispatch.refWindowed;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, int, get_Width)(
	/* [x] */ void)const
{
	return *g_Dispatch.refWidth;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, int, get_Height)(
	/* [x] */ void)const
{
	return *g_Dispatch.refHeight;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, int, get_ExitCode)(
	/* [x] */ void)const
{
	return g_ExitCode;
}

//--------------------------------------------------------------------------------------
// State Retrieval  
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, DEVICE_VERSION, get_D3DVersion)(
	/* [x] */ void)const
{
	return g_DeviceDescs.version;
}

// Direct3D 9

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, IDirect3D9 *, get_D3D9)(
	/* [x] */ void)const
{
	IF_INVALID(g_Device.direct3D9)
	{
		g_Device.direct3D9 = AutoRelease<IDirect3D9>(Direct3DCreate9(D3D_SDK_VERSION));
	}

	return g_Device.direct3D9;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, IDirect3DDevice9 *, get_D3D9_Device) (
	/* [x] */ void)const
{
	return g_Device.d3d9Device;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, D3D9_DEVICE_DESC, get_D3D9_Settings)(
	/* [x] */ void)const
{
	return g_DeviceDescs.d3d9DeviceDesc;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, D3DPRESENT_PARAMETERS, get_D3D9_PresentParameters) (
	/* [x] */ void)const
{
	return g_DeviceDescs.d3d9DeviceDesc.pp;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, D3DSURFACE_DESC, get_D3D9_BackBufferSurfaceDesc) (
	/* [x] */ void)const
{
	return g_Device.backBuffer_Desc;
}

// Direct3D 10

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, BOOL, is_D3D10_Available)(
	/* [x] */ void)const
{
	return nullptr != get_DXGIFactory();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, IDXGIFactory *, get_DXGIFactory)(
	/* [x] */ void)const
{
	IF_INVALID(g_Device.dxgiFactory)
	{
		HRESULT hr;
		IF_FAILED(hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&g_Device.dxgiFactory))
		{
			assert(L"I don't know this err!! your device maybe not support D3D10...");
		}
	}

	return g_Device.dxgiFactory;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, ID3D10Device *, get_D3D10_Device)(
	/* [x] */ void)const
{
	return g_Device.d3d10Device;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, ID3D10Device1 *, get_D3D10_Device1)(
	/* [x] */ void)const
{
	return g_Device.d3d10Device1;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, D3D10_DEVICE_DESC, get_D3D10_Settings)(
	/* [x] */ void)const
{
	return g_DeviceDescs.d3d10DeviceDesc;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, IDXGISwapChain *, get_DXGISwapChain)(
	/* [x] */ void)const
{
	return g_Device.dxgiSwapChain;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, ID3D10RenderTargetView *, get_D3D10_RenderTargetView)(
	/* [x] */ void)const
{
	return g_Device.d3d10RTV;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, ID3D10DepthStencilView *, get_D3D10_DepthStencilView)(
	/* [x] */ void)const
{
	return g_Device.d3d10DSV;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, DXGI_SURFACE_DESC, get_DXGIBackBufferSurfaceDesc)(
	/* [x] */ void)const
{
	return g_Device.dxgiBackBuffer_Desc;
}

//--------------------------------------------------------------------------------------
// Grobal function realize
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, initialize_Dispatch)(
	/* [r] */ DEVICE_VERSION _version)
{
	//
	if (_version == D3D9_DEVICE)
	{
		g_Dispatch.version = _version;
		g_Dispatch.refWindowed = &g_DeviceDescs.d3d9DeviceDesc.pp.Windowed;
		g_Dispatch.refWidth = &g_DeviceDescs.d3d9DeviceDesc.pp.BackBufferWidth;
		g_Dispatch.refHeight = &g_DeviceDescs.d3d9DeviceDesc.pp.BackBufferHeight;
		g_Dispatch.refAdapter = &g_DeviceDescs.d3d9DeviceDesc.adapterOrdinal;
		g_Dispatch.cbCreate3DEnvironment = create_3DEnvironment9;
		g_Dispatch.cbReset3DEnvironment = reset_3DEnvironment9;
		g_Dispatch.cbRender3DEnvironment = render_3DEnvironment9;
		g_Dispatch.cbCleanUp3DEnvironment = cleanup_3DEnvironment9;
	}
	else if (_version == D3D10_DEVICE)
	{
		g_Dispatch.version = _version;
		g_Dispatch.refWindowed = &g_DeviceDescs.d3d10DeviceDesc.sd.Windowed;
		g_Dispatch.refWidth = &g_DeviceDescs.d3d10DeviceDesc.sd.BufferDesc.Width;
		g_Dispatch.refHeight = &g_DeviceDescs.d3d10DeviceDesc.sd.BufferDesc.Height;
		g_Dispatch.refAdapter = &g_DeviceDescs.d3d10DeviceDesc.adapterOrdinal;
		g_Dispatch.cbCreate3DEnvironment = create_3DEnvironment10;
		g_Dispatch.cbReset3DEnvironment = reset_3DEnvironment10;
		g_Dispatch.cbRender3DEnvironment = render_3DEnvironment10;
		g_Dispatch.cbCleanUp3DEnvironment = cleanup_3DEnvironment10;
	}
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(create_3DEnvironment9)(
	/* [x] */ void)
{
	if (g_Device.d3d9Device)
	{
		return S_FALSE;
	}

	// Try to create the device with the chosen settings
	IDirect3D9 * const d3d = g_Device.direct3D9;

	D3D9_DEVICE_DESC & desc =
		g_DeviceDescs.d3d9DeviceDesc;

	// create device
	HRESULT hr = E_FAIL;

	AutoRelease<IDirect3DDevice9> d3d9Device;
	IF_FAILED(hr = d3d->CreateDevice(
		desc.adapterOrdinal,
		desc.deviceType,
		g_Window.focus,
		desc.behaviorFlags,
		(D3DPRESENT_PARAMETERS*)&desc.pp,
		&d3d9Device))
	{
		return hr;
	}

	// If switching to REF, set the exit code to 10.  If switching to HAL and exit code was 10, then set it back to 0.
	if (desc.deviceType == D3DDEVTYPE_REF && g_ExitCode == 0)
	{
		g_ExitCode = 10;
	}
	else if (desc.deviceType == D3DDEVTYPE_HAL && g_ExitCode == 10)
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

	// Call the app's device created callback if non-NULL
	CALLBACK_D3D9_DEVICE_CREATED pCallbackDeviceCreated =
		g_Callbacks.d3d9DeviceCreatedFunc;

	if (pCallbackDeviceCreated)
	{
		hr = pCallbackDeviceCreated(
			g_Device,
			backBufferDesc,
			g_cbUserContext.d3d9DeviceCreatedFuncUserContext);
	}

	IF_SUCCEEDED(hr)
	{
		g_Device.d3d9Device = d3d9Device;
		memcpy(&g_Device.backBuffer_Desc, &backBufferDesc, sizeof(D3DSURFACE_DESC));
	}

	return hr;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(reset_3DEnvironment9)(
	/* [x] */ void)
{
	IDirect3DDevice9 * const d3d9Device = g_Device.d3d9Device;

	if (d3d9Device)
	{
		// Call the app's device lost callback
		CALLBACK_D3D9_DEVICE_LOST callback_DeviceLost =
			g_Callbacks.d3d9DeviceLostFunc;

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
			g_Device,
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

	return hr;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, render_3DEnvironment9)(
	/* [x] */ void)
{
	// If no device created yet because device was lost (ie. another fullscreen exclusive device exists), 
	// then wait and try to create every so often.
	IDirect3DDevice9 * const d3d9Device = g_Device.d3d9Device;

	// Render the scene by calling the app's render callback
	CALLBACK_D3D9_FRAME_RENDER callback_FrameRender =
		g_Callbacks.d3d9FrameRenderFunc;

	if (callback_FrameRender)
	{
		callback_FrameRender(
			g_Device,
			g_TimeStream.time,
			g_TimeStream.elapsedTime,
			g_cbUserContext.d3d9FrameRenderFuncUserContext);
	}
	// Show the frame on the primary surface.
	HRESULT hr;
	IF_FAILED(hr = d3d9Device->Present(NULL, NULL, NULL, NULL))
	{
		if (D3DERR_DEVICELOST == hr)
		{
			reset_3DEnvironment9();
		}
	}

	return;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, cleanup_3DEnvironment9)(
	BOOL _releaseSettings)
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
IMPL_FUNC(create_3DEnvironment10)(
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
			assert(L"Direct3DERR_CREATINGDEVICE : dxgiFactory::EnumAdapters");
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
		assert(L"Direct3DERR_CREATINGDEVICE : D3D10CreateDevice");
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
		assert(L"Direct3DERR_CREATINGDEVICE : CreateSwapChain");
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

	// Call the app's device created callback if non-nullptr
	const DXGI_SURFACE_DESC & backBufferSurfaceDesc =
		g_Device.dxgiBackBuffer_Desc;

	CALLBACK_D3D10_DEVICE_CREATED callback_DeviceCreated =
		g_Callbacks.d3d10DeviceCreatedFunc;

	if (callback_DeviceCreated != nullptr)
	{
		hr = callback_DeviceCreated(
			g_Device,
			backBufferSurfaceDesc,
			g_cbUserContext.d3d10DeviceCreatedFuncUserContext);
	}

	if (FAILED(hr))
	{
		assert(L"Direct3DERR_CREATINGDEVICE : DeviceCreated callback");
		cleanup_3DEnvironment10(false);

		return hr;
	}

	// Setup the render target view and viewport
	IF_FAILED(hr = setup_D3D10Views(_desc))
	{
		assert(L"Direct3DERR_CREATINGDEVICEOBJECTS : setup_D3D10Views");
		cleanup_3DEnvironment10(false);

		return hr;
	}

	// Call the app's swap chain reset callback if non-nullptr
	CALLBACK_D3D10_SWAPCHAINRESIZED callback_SwapChainResized =
		g_Callbacks.d3d10SwapChainResizedFunc;

	if (callback_SwapChainResized)
	{
		IF_FAILED(hr = callback_SwapChainResized(
			g_Device,
			dxgiSwapChain,
			backBufferSurfaceDesc,
			g_cbUserContext.d3d10SwapChainResizedFuncUserContext))
		{
			assert(L"Direct3DERR_CREATINGDEVICE : DeviceReset callback");
			cleanup_3DEnvironment10(false);

			return hr;
		}
	}

	return hr;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(reset_3DEnvironment10)(
	/* [x] */ void)
{
	IDXGISwapChain * const dxgiSwapChain = g_Device.dxgiSwapChain;
	D3D10_DEVICE_DESC & desc = g_DeviceDescs.d3d10DeviceDesc;

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
	BOOL bDeferredDXGIAction = false;

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
			IF_FAILED(hr = dxgiSwapChain->SetFullscreenState(TRUE, NULL))
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

	return S_OK;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, render_3DEnvironment10)(
	/* [x] */ void)
{
	const D3D10_DEVICE_DESC & desc =
		g_DeviceDescs.d3d10DeviceDesc;

	CALLBACK_D3D10_FRAME_RENDER callback_FrameRender =
		g_Callbacks.d3d10FrameRenderFunc;

	if (callback_FrameRender)
	{
		callback_FrameRender(
			g_Device,
			g_TimeStream.time,
			g_TimeStream.elapsedTime,
			g_cbUserContext.d3d9FrameRenderFuncUserContext);
	}

	g_Device.dxgiSwapChain->Present(0, desc.presentFlags);
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, cleanup_3DEnvironment10)(
	/* [r] */ BOOL _releaseSettings)
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
IMPL_FUNC(setup_D3D10Views)(
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
IMPL_FUNC_T(void, resize_DXGIBuffers)(
	/* [r] */ unsigned int _width,
	/* [r] */ unsigned int _height,
	/* [r] */ BOOL _fullScreen)
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

	// Call the app's SwapChain reset callback
	const DXGI_SURFACE_DESC & backBufferSurfaceDesc =
		g_Device.dxgiBackBuffer_Desc;

	CALLBACK_D3D10_SWAPCHAINRESIZED callback_SwapChainResized =
		g_Callbacks.d3d10SwapChainResizedFunc;

	if (callback_SwapChainResized)
	{
		if (FAILED(hr = callback_SwapChainResized(
			g_Device,
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

			PostQuitMessage(0);
		}
	}
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(update_BackBufferDesc)(
	/* [r] */ DEVICE_VERSION _version)
{
	HRESULT hr = E_FAIL;
	if (_version == D3D9_DEVICE)
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
	else if (_version == D3D10_DEVICE)
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

	return hr;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, allow_ShortcutKeys)(
	/* [r] */ BOOL _allowKeys)
{
	if (_allowKeys)
	{
		// Restore StickyKeys/etc to original state and enable Windows key      
		STICKYKEYS sk = g_init.stickyKeys;
		TOGGLEKEYS tk = g_init.toggleKeys;
		FILTERKEYS fk = g_init.filterKeys;

		SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &sk, 0);
		SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tk, 0);
		SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fk, 0);

		// Remove the keyboard hoook when it isn't needed to prevent any slow down of other apps
		if (g_State.keyboardHook)
		{
			UnhookWindowsHookEx(g_State.keyboardHook);
			g_State.keyboardHook = nullptr;
		}
	}
	else
	{
		// Set low level keyboard hook if haven't already
		IF_INVALID(g_State.keyboardHook)
		{
			HHOOK hKeyboardHook = SetWindowsHookEx(
				WH_KEYBOARD_LL,
				direct3D_LowLevelKeyboardProc,
				GetModuleHandle(NULL),
				0);

			g_State.keyboardHook = hKeyboardHook;
		}

		// Disable StickyKeys/etc shortcuts but if the accessibility feature is on, 
		// then leave the settings alone as its probably being usefully used

		STICKYKEYS skOff = g_init.stickyKeys;
		if ((skOff.dwFlags & SKF_STICKYKEYSON) == 0)
		{
			// Disable the hotkey and the confirmation
			skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
			skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
		}

		TOGGLEKEYS tkOff = g_init.toggleKeys;
		if ((tkOff.dwFlags & TKF_TOGGLEKEYSON) == 0)
		{
			// Disable the hotkey and the confirmation
			tkOff.dwFlags &= ~TKF_HOTKEYACTIVE;
			tkOff.dwFlags &= ~TKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tkOff, 0);
		}

		FILTERKEYS fkOff = g_init.filterKeys;
		if ((fkOff.dwFlags & FKF_FILTERKEYSON) == 0)
		{
			// Disable the hotkey and the confirmation
			fkOff.dwFlags &= ~FKF_HOTKEYACTIVE;
			fkOff.dwFlags &= ~FKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fkOff, 0);
		}
	}
}

//--------------------------------------------------------------------------------------
LRESULT CALLBACK direct3D_WndProc(
	/* [r] */ HWND _hWnd,
	/* [r] */ unsigned int _uMsg,
	/* [r] */ unsigned int _wParam,
	/* [r] */ long _lParam)
{
	// Consolidate the keyboard messages and pass them to the app's keyboard callback
	if (_uMsg == WM_KEYDOWN || _uMsg == WM_SYSKEYDOWN || _uMsg == WM_KEYUP || _uMsg == WM_SYSKEYUP)
	{
		BOOL bKeyDown = (_uMsg == WM_KEYDOWN || _uMsg == WM_SYSKEYDOWN);
		unsigned long dwMask = (1 << 29);
		BOOL bAltDown = ((_lParam & dwMask) != 0);

		BOOL * bKeys = g_Keys;
		bKeys[(unsigned char)(_wParam & 0xFF)] = bKeyDown;

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
		BOOL bLeftButton = ((nMouseButtonState & MK_LBUTTON) != 0);
		BOOL bRightButton = ((nMouseButtonState & MK_RBUTTON) != 0);
		BOOL bMiddleButton = ((nMouseButtonState & MK_MBUTTON) != 0);
		BOOL bSideButton1 = ((nMouseButtonState & MK_XBUTTON1) != 0);
		BOOL bSideButton2 = ((nMouseButtonState & MK_XBUTTON2) != 0);

		BOOL * bMouseButtons = g_MouseButtons;
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
		BOOL bNoFurtherProcessing = false;
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
		// Handle paint messages when the app is paused
		if (g_TimeStream.pauseRendering)
		{
			g_Dispatch.cbRender3DEnvironment();
		}
		break;

	case WM_SIZE:
		if (SIZE_MINIMIZED == _wParam)
		{
			g_State.minimizedSize = true;

			// Pause while we're minimized
			g_Direct3D.pause_Rendering(true);
		}
		else
		{
			if (g_State.createdDevice)
			{
				unsigned int width = LOWORD(_lParam);
				unsigned int height = HIWORD(_lParam);

				if (width == 0 || height == 0)
				{

				}
				else if (SIZE_RESTORED == _wParam)
				{
					if (g_State.autoChangeAdapter)
					{
						g_Direct3D.change_Monitor(
							(*g_Dispatch.refWindowed),
							width,
							height);
					}
					else
					{
						g_Direct3D.change_Monitor(
							(*g_Dispatch.refWindowed),
							width,
							height,
							(*g_Dispatch.refAdapter));
					}
				}
				else if (SIZE_MAXIMIZED == _wParam)
				{
					if (*g_Dispatch.refWindowed)
					{
						if (g_State.autoChangeAdapter)
						{
							g_Direct3D.change_Monitor(
								(*g_Dispatch.refWindowed),
								width,
								height);
						}
						else
						{
							g_Direct3D.change_Monitor(
								(*g_Dispatch.refWindowed),
								width,
								height,
								(*g_Dispatch.refAdapter));
						}
					}

					g_State.maximizedSize = true;
				}
			}

			if (g_State.minimizedSize)
			{
				g_State.minimizedSize = false;
				g_Direct3D.pause_Rendering(false);
			}
			else if (g_State.maximizedSize)
			{
				g_State.maximizedSize = false;
			}
		}
		break;

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)_lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)_lParam)->ptMinTrackSize.y = 200;
		break;

	case WM_ACTIVATEAPP:
		if (_wParam == TRUE && g_State.runMainLoop) // Handle only if previously not active 
		{
			// Upon returning to this app, potentially disable shortcut keys 
			// (Windows key, accessibility shortcuts)
			allow_ShortcutKeys((*g_Dispatch.refWindowed));

		}
		else if (_wParam == FALSE && g_State.runMainLoop) // Handle only if previously active 
		{
			// Restore shortcut keys (Windows key, accessibility shortcuts) to original state
			//
			// This is important to call here if the shortcuts are disabled, 
			// because if this is not done then the Windows key will continue to 
			// be disabled while this app is running which is very bad.
			// If the app crashes, the Windows key will return to normal.
			allow_ShortcutKeys(true);
		}
		break;

	case WM_MENUCHAR:
		// A menu is active and the user presses a key that does not correspond to any mnemonic or accelerator key
		// So just ignore and don't beep
		return MAKELRESULT(0, MNC_CLOSE);
		break;

	case WM_NCHITTEST:
		// Prevent the user from selecting the menu in full screen mode
		IF_FALSE((*g_Dispatch.refWindowed))
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
			g_TimeStream.timer.reset();
			g_TimeStream.time = 0.0f;
			g_TimeStream.elapsedTime = 0.0f;
			g_TimeStream.absoluteTime = 0.0f;
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
			IF_FALSE((*g_Dispatch.refWindowed))
				return 0;
			break;
		}
		break;

	case WM_SYSKEYDOWN:
		g_Direct3D.destroy();
		break;

	case WM_KEYDOWN:
		if (_wParam == VK_ESCAPE)
		{
			SendMessage(_hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case WM_CLOSE:
	{
					 HMENU hMenu;
					 if (hMenu = GetMenu(_hWnd))
					 {
						 DestroyMenu(hMenu);
					 }

					 DestroyWindow(_hWnd);

					 UnregisterClass(L"Direct3DWindowClass", nullptr);

					 g_Window.focus = nullptr;
					 g_Window.fullScreen = nullptr;
					 g_Window.windowScreen = nullptr;
					 return 0;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	// Don't allow the F10 key to act as a shortcut to the menu bar
	// by not passing these messages to the DefWindowProc only when
	// there's no menu present
	if (nullptr == g_Window.menu && (_uMsg == WM_SYSKEYDOWN || _uMsg == WM_SYSKEYUP) && _wParam == VK_F10)
	{
		return 0;
	}
	else
	{
		return DefWindowProc(_hWnd, _uMsg, _wParam, _lParam);
	}
}

//--------------------------------------------------------------------------------------
LRESULT CALLBACK direct3D_LowLevelKeyboardProc(
	/* [r] */ int _code,
	/* [r] */ WPARAM _wParam,
	/* [r] */ LPARAM _lParam)
{
	// do not process message 
	if (_code < 0 || _code != HC_ACTION)
	{
		return CallNextHookEx(
			g_State.keyboardHook,
			_code,
			_wParam,
			_lParam);
	}

	BOOL bEatKeystroke = false;
	KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)_lParam;
	switch (_wParam)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	{
					 bEatKeystroke = p->vkCode == VK_LWIN || p->vkCode == VK_RWIN;
					 break;
	}
	}

	if (bEatKeystroke)
	{
		return 1;
	}
	else
	{
		return CallNextHookEx(
			g_State.keyboardHook,
			_code,
			_wParam,
			_lParam);
	}
}

//--------------------------------------------------------------------------------------
// General callbacks
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_FrameMove)(
	/* [ref] */ CALLBACK_FRAMEMOVE _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.frameMoveFunc = _callback;
	g_cbUserContext.frameMoveFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_Keyboard)(
	/* [ref] */ CALLBACK_KEYBOARD _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.keyboardFunc = _callback;
	g_cbUserContext.keyboardFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_Mouse)(
	/* [ref] */ CALLBACK_MOUSE _callback,
	/* [r] */ BOOL _includeMouseMove,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.mouseFunc = _callback;
	g_cbUserContext.notifyOnMouseMove = _includeMouseMove;
	g_cbUserContext.mouseFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_MsgProc)(
	/* [ref] */ CALLBACK_MSGPROC _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.windowMsgFunc = _callback;
	g_cbUserContext.windowMsgFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_DeviceChanging)(
	/* [ref] */ CALLBACK_MODIFY_DEVICE_SETTINGS _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.modifyDeviceSettingsFunc = _callback;
	g_cbUserContext.modifyDeviceSettingsFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_DeviceRemoved)(
	/* [ref] */ CALLBACK_DEVICE_REMOVED _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.deviceRemovedFunc = _callback;
	g_cbUserContext.deviceRemovedFuncUserContext = _userContext;
}

// Direct3D 9 callbacks

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_D3D9_DeviceAcceptable)(
	/* [ref] */ CALLBACK_IS_D3D9_DEVICE_ACCEPTABLE _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.isD3D9DeviceAcceptableFunc = _callback;
	g_cbUserContext.isD3D9DeviceAcceptableFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_D3D9_DeviceCreated)(
	/* [ref] */ CALLBACK_D3D9_DEVICE_CREATED _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d9DeviceCreatedFunc = _callback;
	g_cbUserContext.d3d9DeviceCreatedFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_D3D9_DeviceReset)(
	/* [ref] */ CALLBACK_D3D9_DEVICE_RESET _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d9DeviceResetFunc = _callback;
	g_cbUserContext.d3d9DeviceResetFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_D3D9_DeviceLost)(
	/* [ref] */ CALLBACK_D3D9_DEVICE_LOST _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d9DeviceLostFunc = _callback;
	g_cbUserContext.d3d9DeviceLostFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_D3D9_DeviceDestroyed)(
	/* [ref] */ CALLBACK_D3D9_DEVICE_DESTROYED _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d9DeviceDestroyedFunc = _callback;
	g_cbUserContext.d3d9DeviceDestroyedFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_D3D9_FrameRender)(
	/* [ref] */ CALLBACK_D3D9_FRAME_RENDER _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d9FrameRenderFunc = _callback;
	g_cbUserContext.d3d9FrameRenderFuncUserContext = _userContext;
}

// Direct3D 10 callbacks

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_D3D10_DeviceAcceptable)(
	/* [ref] */ CALLBACK_IS_D3D10_DEVICE_ACCEPTABLE _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.isD3D10DeviceAcceptableFunc = _callback;
	g_cbUserContext.isD3D10DeviceAcceptableFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_D3D10_DeviceCreated)(
	/* [ref] */ CALLBACK_D3D10_DEVICE_CREATED _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d10DeviceCreatedFunc = _callback;
	g_cbUserContext.d3d10DeviceCreatedFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_D3D10_SwapChainResized)(
	/* [ref] */ CALLBACK_D3D10_SWAPCHAINRESIZED _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d10SwapChainResizedFunc = _callback;
	g_cbUserContext.d3d10SwapChainResizedFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_D3D10_SwapChainReleasing)(
	/* [ref] */ CALLBACK_D3D10_SWAPCHAINRELEASING _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d10SwapChainReleasingFunc = _callback;
	g_cbUserContext.d3d10SwapChainReleasingFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_D3D10_DeviceDestroyed)(
	/* [ref] */ CALLBACK_D3D10_DEVICE_DESTROYED _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d10DeviceDestroyedFunc = _callback;
	g_cbUserContext.d3d10DeviceDestroyedFuncUserContext = _userContext;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, set_Callback_D3D10_FrameRender)(
	/* [ref] */ CALLBACK_D3D10_FRAME_RENDER _callback,
	/* [r/w] */ void * _userContext)
{
	g_Callbacks.d3d10FrameRenderFunc = _callback;
	g_cbUserContext.d3d10FrameRenderFuncUserContext = _userContext;
}