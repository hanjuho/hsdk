#include <hsdk/win/frame/direct3d/direct3d.h>
#include <hsdk/win/frame/direct3d/direct3d_outside.h>

#define NODEBUG
#include <assert.h>
#include <list>



using namespace hsdk;
using namespace direct3d;


//--------------------------------------------------------------------------------------
// Grobal thread safety
//--------------------------------------------------------------------------------------

// 설명 : 
CRITICAL_SECTION g_cs;

// 설명 : 
BOOL g_bThreadSafe = true;


//--------------------------------------------------------------------------------------
// Grobal d3d variable
//--------------------------------------------------------------------------------------

// 설명 : 
Direct3D_Outside g_Outside;

// 설명 :
Direct3D_Callbacks g_Callbacks;

// 설명 : 
hsdk::win::UserTimeStream g_TimeStream;

//--------------------------------------------------------------------------------------
// Grobal control variable
//--------------------------------------------------------------------------------------

// 설명 : array of key state
BOOL g_Keys[256] = { 0 };

// 설명 : array of last key state
BOOL g_LastKeys[256] = { 0 };

// 설명 : array of mouse states
BOOL g_MouseButtons[5] = { 0 };

//--------------------------------------------------------------------------------------
// Grobal device handle variable
//--------------------------------------------------------------------------------------

// 설명 : 
Direct3D g_Direct3D;

// 설명 : 
Direct3D_State g_State;

// 설명 : 
Direct3D_Window g_Window;

// 설명 : 
hsdk::AutoDelete<Direct3D_DeviceFactory> g_DeviceFactory;

// 설명 : 
Direct3D_Device g_Device;

// 설명 : 
hsdk::AutoDelete<D3D9_DEVICE_DESC> g_D3D9Descs;

// 설명 : 
hsdk::AutoDelete<D3D10_DEVICE_DESC> g_D3D10Descs;

//--------------------------------------------------------------------------------------
// Grobal function declare
//--------------------------------------------------------------------------------------

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
CLASS_IMPL_FUNC(Direct3D, setup0_Window)(
	/* [r] */ const wchar_t * _strWindowTitle,
	/* [r] */ int _x,
	/* [r] */ int _y,
	/* [r] */ HINSTANCE _hInstance,
	/* [r] */ HICON _hIcon,
	/* [r] */ HMENU _hMenu)
{
	if (TRUE == InterlockedCompareExchange(&g_State.setupWindow, TRUE, FALSE))
	{
		return S_FALSE;
	}

	// Not always needed, but lets the app create GDI dialogs
	InitCommonControls();

	// Save the current sticky/toggle/filter key settings so DXUT can restore them later
	g_State.stickyKeys = { sizeof(STICKYKEYS), 0 };
	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &g_State.stickyKeys, 0);

	g_State.toggleKeys = { sizeof(TOGGLEKEYS), 0 };
	SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_State.toggleKeys, 0);

	g_State.filterKeys = { sizeof(FILTERKEYS), 0 };
	SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &g_State.filterKeys, 0);

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

	// Find the window's initial size, but it might be changed later
	long nDefaultWidth = 640;
	long nDefaultHeight = 480;

	RECT rc = {
		0,
		0,
		nDefaultWidth,
		nDefaultHeight
	};

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, (_hMenu != nullptr) ? true : false);

	// Create the render window
	HWND hWnd = CreateWindow(
		L"Direct3DWindowClass",
		_strWindowTitle,
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
		g_State.setupWindow = FALSE;

		return ADD_FLAG(HSDK_FAIL, GetLastError());
	}

	g_Window.HINSTANCE = _hInstance;
	g_Window.windowTitle = _strWindowTitle;
	g_Window.hwnd = hWnd;
	g_Window.menu = _hMenu;

	g_Window.adapterMonitor =
		MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);

	g_State.windowed = TRUE;
	g_State.width = nDefaultWidth;
	g_State.height = nDefaultHeight;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D, setup1_DeviceFactory)(
	/* [set] */ Direct3D_DeviceFactory * _factory)
{
	IF_INVALID(_factory)
	{
		return E_INVALIDARG;
	}

	if (TRUE == InterlockedCompareExchange(&g_State.setupDeviceFactory, TRUE, FALSE))
	{
		return S_FALSE;
	}

	g_DeviceFactory = _factory;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D, setup2_Device9)(
	/* [set] */ D3D9_DEVICE_DESC & _desc)
{
	IF_FALSE(g_State.setupDeviceFactory)
	{
		return E_ACCESSDENIED;
	}

	if (TRUE == InterlockedCompareExchange(&g_State.setupDevice, TRUE, FALSE))
	{
		return S_FALSE;
	}

	g_State.modifyBackBuffer_inMsgProc = false;

	HRESULT hr;
	if (FAILED(hr = g_DeviceFactory->create9(g_Device, _desc, &g_Callbacks)))
	{
		g_State.setupDevice = FALSE;
	}
	else
	{
		g_D3D9Descs = new D3D9_DEVICE_DESC(_desc);
	}

	g_State.modifyBackBuffer_inMsgProc = true;

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D, setup2_Device10)(
	/* [set] */ D3D10_DEVICE_DESC & _desc)
{
	IF_FALSE(g_State.setupDeviceFactory)
	{
		return E_ACCESSDENIED;
	}

	if (TRUE == InterlockedCompareExchange(&g_State.setupDevice, TRUE, FALSE))
	{
		return S_FALSE;
	}

	g_State.modifyBackBuffer_inMsgProc = false;
	if (g_State.autoChangeAdapter)
	{

	}
	else
	{

	}

	HRESULT hr;
	if (FAILED(hr = g_DeviceFactory->create10(g_Device, _desc, &g_Callbacks)))
	{
		g_State.setupDevice = FALSE;
	}
	else
	{
		g_D3D10Descs = new D3D10_DEVICE_DESC(_desc);
		g_Outside.initialize(g_Device.dxgiFactory, TRUE, g_State.is_in_GammaCorrectMode);

		g_State.adapter = _desc.adapterOrdinal;
		g_State.windowed = _desc.sd.Windowed;
		g_State.width = _desc.sd.BufferDesc.Width;
		g_State.height = _desc.sd.BufferDesc.Height;
	}

	g_State.modifyBackBuffer_inMsgProc = true;

	return hr;
}


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D, dynamic_WndProc)(
	/* [r] */ unsigned int _uMsg,
	/* [r] */ unsigned int _wParam,
	/* [r] */ long _lParam)
{
	return direct3D_WndProc(g_Window.hwnd, _uMsg, _wParam, _lParam);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D, transform)(
	/* [r] */ BOOL _windowed,
	/* [r] */ unsigned long _suggestedWidth,
	/* [r] */ unsigned long _suggestedHeight,
	/* [r] */ unsigned int _adapter)
{
	if (g_State.minimizedSize)
	{
		// Need to resize, so if window is maximized or minimized then restore the window
		if (IsIconic(g_Window.hwnd))
		{
			ShowWindow(g_Window.hwnd, SW_RESTORE);
		}
	}

	if (g_State.maximizedSize)
	{
		// doing the IsIconic() check first also handles the WPF_RESTORETOMAXIMIZED case
		if (IsZoomed(g_Window.hwnd))
		{
			ShowWindow(g_Window.hwnd, SW_RESTORE);
		}
	}

	// Make the window visible
	if (!IsWindowVisible(g_Window.hwnd))
	{
		ShowWindow(g_Window.hwnd, SW_SHOW);
	}

	// Ensure that the display doesn't power down when fullscreen but does when windowed
	if (g_State.windowed)
	{
		SetThreadExecutionState(ES_DISPLAY_REQUIRED | ES_CONTINUOUS);
	}
	else
	{
		SetThreadExecutionState(ES_CONTINUOUS);
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, destroy)(
	/* [x] */ void)
{
	g_Outside.destroy();
	this->shutdown();

	// all clear
	g_TimeStream = win::UserTimeStream();;
	g_Callbacks = Direct3D_Callbacks();

	g_D3D10Descs.~AutoDelete();
	g_D3D9Descs.~AutoDelete();
	g_Device = Direct3D_Device();
	g_DeviceFactory.~AutoDelete();
	g_Window = Direct3D_Window();
	g_State = Direct3D_State();
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

	const HWND & hWnd = g_Window.hwnd;

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

		IF_FALSE(g_TimeStream.is_Time_Paused())
		{
			// Store the time for the app
			g_TimeStream.update_Time();

			// Animate the scene by calling the app's frame move callback
			CALLBACK_FRAMEMOVE callback_FrameMove =
				g_Callbacks.frameMoveFunc;

			if (callback_FrameMove)
			{
				callback_FrameMove(
					g_TimeStream.get_Time(),
					g_TimeStream.get_ElapsedTime(),
					g_Callbacks.frameMoveFuncUserContext);
			}
		}

		IF_FALSE(g_TimeStream.is_Rendering_Paused())
		{
			render();

			// Update current frame #
			g_TimeStream.update_Frame();

			++g_State.frameCount;
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
CLASS_IMPL_FUNC_T(Direct3D, void, render)(
	/* [x] */ void)
{
	// Render a frame during idle time (no messages are waiting)
	if (g_D3D10Descs)
	{
		CALLBACK_D3D10_FRAME_RENDER callback_FrameRender =
			g_Callbacks.d3d10FrameRenderFunc;

		if (callback_FrameRender)
		{
			callback_FrameRender(
				g_Device.d3d10Device,
				g_TimeStream.get_Time(),
				g_TimeStream.get_ElapsedTime(),
				g_Callbacks.d3d10FrameRenderFuncUserContext);
		}

		// Show the frame on the primary surface.
		HRESULT hr;
		IF_FAILED(hr = g_Device.dxgiSwapChain->Present(0, g_D3D10Descs->presentFlags))
		{
			if (D3DERR_DEVICELOST == hr)
			{
				g_DeviceFactory->resize10(
					g_Device,
					*g_D3D10Descs,
					&g_Callbacks);
			}
		}
	}
	else if (g_D3D9Descs)
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
				d3d9Device,
				g_TimeStream.get_Time(),
				g_TimeStream.get_ElapsedTime(),
				g_Callbacks.d3d9FrameRenderFuncUserContext);
		}

		// Show the frame on the primary surface.
		HRESULT hr;
		IF_FAILED(hr = d3d9Device->Present(nullptr, nullptr, nullptr, nullptr))
		{
			if (D3DERR_DEVICELOST == hr)
			{
				g_DeviceFactory->restore9(
					g_Device,
					*g_D3D9Descs,
					&g_Callbacks);
			}
		}
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, shutdown)(
	/* [r] */ int _exitCode)
{
	const HWND & hWnd = g_Window.hwnd;
	IF_INVALID(hWnd)
	{
		SendMessage(hWnd, WM_CLOSE, 0, 0);
	}

	g_TimeStream.pause_Time(true);
	g_TimeStream.pause_Rendering(true);

	if (g_D3D9Descs)
	{
		if (g_State.setupDevice)
		{
			CALLBACK_D3D9_DEVICE_LOST callback_DeviceLost =
				g_Callbacks.d3d9DeviceLostFunc;

			if (callback_DeviceLost)
			{
				callback_DeviceLost(
					g_Callbacks.d3d9DeviceLostFuncUserContext);
			}

			if (g_Device.d3d9Device)
			{
				CALLBACK_D3D9_DEVICE_DESTROYED callback_DeviceDestroyed =
					g_Callbacks.d3d9DeviceDestroyedFunc;

				if (callback_DeviceDestroyed)
				{
					callback_DeviceDestroyed(
						g_Callbacks.d3d9DeviceDestroyedFuncUserContext);
				}
			}

			g_State.setupDevice = false;
		}

		g_D3D9Descs.~AutoDelete();
	}
	else if (g_D3D10Descs)
	{
		if (g_State.setupDevice)
		{
			CALLBACK_D3D10_SWAPCHAINRELEASING callback_SwapChainReleasing =
				g_Callbacks.d3d10SwapChainReleasingFunc;

			if (callback_SwapChainReleasing)
			{
				callback_SwapChainReleasing(
					g_Callbacks.d3d10SwapChainReleasingFuncUserContext);
			}

			g_Device.dxgiSwapChain->SetFullscreenState(FALSE, 0);

			if (g_Device.d3d10Device)
			{
				g_Device.d3d10Device->ClearState();

				CALLBACK_D3D10_DEVICE_DESTROYED callback_DeviceDestroyed =
					g_Callbacks.d3d10DeviceDestroyedFunc;

				if (callback_DeviceDestroyed)
				{
					callback_DeviceDestroyed(
						g_Callbacks.d3d10DeviceDestroyedFuncUserContext);
				}
			}

			g_State.setupDevice = false;
		}

		g_D3D10Descs.~AutoDelete();
	}

	// all clear
	g_Device = Direct3D_Device();

	// Restore shortcut keys (Windows key, accessibility shortcuts) to original state
	// This is important to call here if the shortcuts are disabled, 
	// because accessibility setting changes are permanent.
	// This means that if this is not done then the accessibility settings 
	// might not be the same as when the app was started. 
	// If the app crashes without restoring the settings, this is also true so it
	// would be wise to backup/restore the settings from a file so they can be 
	// restored when the crashed app is run again.
	allow_ShortcutKeys(true);
}

//--------------------------------------------------------------------------------------
// Common tasks 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, userSet_ShortcutKeySettings)(
	/* [r] */ BOOL _allowWhenFullscreen,
	/* [r] */ BOOL _allowWhenWindowed)
{
	g_State.allowWhenFullscreen = _allowWhenFullscreen;
	g_State.allowWhenWindowed = _allowWhenWindowed;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, userSet_AutoChangeMoniter)(
	/* [r] */ BOOL _autoChange)
{
	g_State.autoChangeAdapter = _autoChange;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, userSet_GammaCorrectMode)(
	/* [r] */ BOOL _gammaCorrect)
{
	g_State.is_in_GammaCorrectMode = _gammaCorrect;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, userSet_Vsync)(
	/* [r] */ BOOL _vsync)
{
	assert(L"미구현");
}


//--------------------------------------------------------------------------------------
// General
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, void, clip_Screen)(
	/* [r] */ BOOL _windowed,
	/* [r] */ RECT & _rect)const
{
	// Get the rect of the monitor attached to the adapter
	MONITORINFO miAdapter;
	miAdapter.cbSize = sizeof(MONITORINFO);

	IF_FALSE(GetMonitorInfo(g_Window.adapterMonitor, &miAdapter))
	{
		return;
	}

	if (_windowed)
	{
		// Do something reasonable if the BackBuffer size is greater than the monitor size
		int nAdapterMonitorWidth = miAdapter.rcWork.left + miAdapter.rcWork.right;
		int nAdapterMonitorHeight = miAdapter.rcWork.top + miAdapter.rcWork.bottom;

		int empty_x = _rect.left - miAdapter.rcWork.left;
		int empty_y = _rect.top - miAdapter.rcWork.top;

		if (_rect.left < miAdapter.rcWork.left)
		{
			_rect.left = miAdapter.rcWork.left;
			empty_x = 0;
		}

		if (_rect.top < miAdapter.rcWork.top)
		{
			_rect.top = miAdapter.rcWork.top;
			empty_y = 0;
		}

		int cx = _rect.left + _rect.right;
		if (nAdapterMonitorWidth < cx)
		{
			int dw = cx - nAdapterMonitorWidth;
			if (empty_x < dw)
			{
				_rect.left -= empty_x;
				dw -= empty_x;

				_rect.right -= dw;
			}
			else
			{
				_rect.left -= dw;
			}
		}

		int cy = _rect.top + _rect.bottom;
		if (nAdapterMonitorHeight < cy)
		{
			int dh = cy - nAdapterMonitorHeight;
			if (empty_y < dh)
			{
				_rect.top -= empty_y;
				dh -= empty_y;

				_rect.bottom -= dh;
			}
			else
			{
				_rect.top -= dh;
			}
		}
	}
	else
	{
		_rect = miAdapter.rcWork;
	}
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
// Grobal function impl
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, Direct3D_Callbacks *, callbacks)(
	/* [x] */ void)const
{
	return &g_Callbacks;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, win::UserTimeStream *, timeStream)(
	/* [x] */ void)const
{
	return &g_TimeStream;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, const Direct3D_State *, get_State)(
	/* [x] */ void)const
{
	return &g_State;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, const Direct3D_Window *, get_Window)(
	/* [x] */ void)const
{
	return &g_Window;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, const Direct3D_Device *, get_Device)(
	/* [x] */ void)const
{
	return &g_Device;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, const D3D9_DEVICE_DESC *, get_Device9Desc)(
	/* [x] */ void)const
{
	return g_D3D9Descs;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D, const D3D10_DEVICE_DESC *, get_Device10Desc)(
	/* [x] */ void)const
{
	return g_D3D10Descs;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, allow_ShortcutKeys)(
	/* [r] */ BOOL _allowKeys)
{
	if (_allowKeys)
	{
		// Restore StickyKeys/etc to original state and enable Windows key      
		STICKYKEYS sk = g_State.stickyKeys;
		TOGGLEKEYS tk = g_State.toggleKeys;
		FILTERKEYS fk = g_State.filterKeys;

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

		STICKYKEYS skOff = g_State.stickyKeys;
		if ((skOff.dwFlags & SKF_STICKYKEYSON) == 0)
		{
			// Disable the hotkey and the confirmation
			skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
			skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
		}

		TOGGLEKEYS tkOff = g_State.toggleKeys;
		if ((tkOff.dwFlags & TKF_TOGGLEKEYSON) == 0)
		{
			// Disable the hotkey and the confirmation
			tkOff.dwFlags &= ~TKF_HOTKEYACTIVE;
			tkOff.dwFlags &= ~TKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tkOff, 0);
		}

		FILTERKEYS fkOff = g_State.filterKeys;
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
				g_Callbacks.keyboardFuncUserContext);
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
				g_Callbacks.mouseFuncUserContext);
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
			g_Callbacks.windowMsgFuncUserContext);

		if (bNoFurtherProcessing)
		{
			return nResult;
		}
	}

	switch (_uMsg)
	{
	case WM_PAINT:
		// Handle paint messages when the app is paused
		if (g_TimeStream.is_Rendering_Paused())
		{
			g_Direct3D.render();
		}
		break;

	case WM_SIZE:
		if (SIZE_MINIMIZED == _wParam)
		{
			g_State.minimizedSize = true;

			// Pause while we're minimized
			g_TimeStream.pause_Rendering(true);
		}
		else
		{
			IF_FALSE(g_State.modifyBackBuffer_inMsgProc)
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
						g_Direct3D.transform(
							g_State.windowed,
							width,
							height);
					}
				}
				else if (SIZE_MAXIMIZED == _wParam)
				{
					if (g_State.windowed)
					{
						g_Direct3D.transform(
							g_State.windowed,
							width,
							height);
					}

					g_State.maximizedSize = true;
				}
			}

			if (g_State.minimizedSize)
			{
				g_State.minimizedSize = false;

				// pause_Rendering(true) in SIZE_MINIMIZED
				g_TimeStream.pause_Rendering(false);
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
			allow_ShortcutKeys(g_State.windowed);
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
		IF_FALSE(g_State.windowed)
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
			g_TimeStream.reset();
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
			IF_FALSE(g_State.windowed)
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
			if (g_State.windowed)
			{
				if (MessageBox(_hWnd, L"게임을 종료하시겠습니까? ", L"주의!!", MB_YESNO) == IDYES)
				{
					SendMessage(_hWnd, WM_CLOSE, 0, 0);
				}
			}
			else
			{
				g_Direct3D.transform();
			}
		}
		else if (_wParam == VK_F5)
		{
			if (g_State.windowed)
			{
				if (MessageBox(_hWnd, L"전체화면으로 하시겠습니까? ", L"주의!!", MB_YESNO) == IDYES)
				{
					g_Direct3D.transform(false);
				}
			}
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

					 g_Window.hwnd = nullptr;
					 g_State.setupWindow = false;

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