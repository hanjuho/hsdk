#include <hsdk/win/frame/direct3d/direct3d_outside.h>



using namespace hsdk;
using namespace direct3d;

// 설명 : 
Direct3D_Outside hsdk::direct3d::g_Direct3D_Outside;

//--------------------------------------------------------------------------------------
// Grobal device handle variable
//--------------------------------------------------------------------------------------

// 설명 : 
std::vector<VideoCard_info> g_Videos;

// 설명 :
BOOL g_enumerateAllAdapterFormats;

// 설명 :
BOOL g_is_in_GammaCorrectMode;

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D_Outside, initialize)(
	/* [r] */ IDXGIFactory * _factory,
	/* [r] */ BOOL _enumerateAllAdapterFormats,
	/* [r] */ BOOL _is_in_GammaCorrectMode)
{
	IF_INVALID(_factory)
	{
		return E_ABORT;
	}

	g_enumerateAllAdapterFormats = _enumerateAllAdapterFormats;
	g_is_in_GammaCorrectMode = _is_in_GammaCorrectMode;

	// CDXUTPerfEventGenerator eventGenerator(DXUT_PERFEVENTCOLOR, L"DXUT D3D10 Enumeration");
	HRESULT hr;

	g_Videos.clear();
	for (int index = 0;; ++index)
	{
		VideoCard_info card_info;

		// DXGIERR_NOT_FOUND is expected when the end of the list is hit
		IF_FAILED(hr = _factory->EnumAdapters(index, &card_info.dxgiAdapter))
		{
			break;
		}

		card_info.ordinal = index;
		card_info.dxgiAdapter->GetDesc(&card_info.desc);

		for (int index = 0;; ++index)
		{
			VideoCard_Output_info output_info;
			IF_FAILED(hr = initialize_Outputs(
				output_info,
				index,
				card_info.dxgiAdapter))
			{
				break;
			}

			if (0 < output_info.displayModeList.size())
			{
				card_info.output_infoList.push_back(output_info);
			}
		}

		g_Videos.push_back(card_info);
	}

	//
	// Check for 2 or more adapters with the same name. Append the name
	// with some instance number if that's the case to help distinguish
	// them.
	//
	BOOL uniqueDesc = true;

	const unsigned int size = g_Videos.size();
	for (unsigned int A = 0; A < size; ++A)
	{
		const VideoCard_info & infoA = g_Videos[A];

		for (unsigned int B = A + 1; B < size; ++B)
		{
			const VideoCard_info & infoB = g_Videos[A];

			if (wcsncmp(
				infoA.desc.Description,
				infoB.desc.Description,
				128) == 0)
			{
				uniqueDesc = false;
				break;
			}
		}

		IF_FALSE(uniqueDesc)
		{
			break;
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D_Outside, initialize_Outputs)(
	/* [w] */ VideoCard_Output_info & _output_info,
	/* [r] */ unsigned int _index,
	/* [r] */ IDXGIAdapter * _adapter)
{
	HRESULT hr;

	_output_info.output = _index;
	hr = _adapter->EnumOutputs(_index, &_output_info.dxgiOutput);
	if (DXGI_ERROR_NOT_FOUND == hr)
	{
		return E_FAIL;
	}
	else if (FAILED(hr))
	{
		//Something bad happened.
		return hr;
	}
	else //Success!
	{
		_output_info.dxgiOutput->GetDesc(&_output_info.desc);

		initialize_DisplayModes(_output_info);
		if (_output_info.displayModeList.size() <= 0)
		{
			// If this output has no valid display mode, do not save it.
			return E_FAIL;
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct3D_Outside, initialize_DisplayModes)(
	/* [w] */ VideoCard_Output_info & _output_info)
{
	HRESULT hr = S_OK;
	DXGI_FORMAT allowedAdapterFormatArray[] =
	{
		//This is DXUT's preferred mode
		g_is_in_GammaCorrectMode
		? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM,
		g_is_in_GammaCorrectMode
		? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_R10G10B10A2_UNORM
	};

	const int allowedAdapterFormatArrayCount =
		sizeof(allowedAdapterFormatArray) / sizeof(allowedAdapterFormatArray[0]);

	// Swap perferred modes for apps running in linear space
	DXGI_FORMAT RemoteMode = g_is_in_GammaCorrectMode
		? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM;

	// The fast path only enumerates R8G8B8A8_UNORM_SRGB modes
	int allowedAdapterFormatArrayCount_temp = g_enumerateAllAdapterFormats
		? 1 : allowedAdapterFormatArrayCount;

	for (int index = 0; index < allowedAdapterFormatArrayCount_temp; ++index)
	{
		// Fast-path: Try to grab at least 512 modes.
		//			  This is to avoid calling GetDisplayModeList more times than necessary.
		//			  GetDisplayModeList is an expensive call.
		DXGI_MODE_DESC descs[1024];

		unsigned int numModes;
		if (FAILED(hr = _output_info.dxgiOutput->GetDisplayModeList(
			allowedAdapterFormatArray[index],
			DXGI_ENUM_MODES_SCALING,
			&numModes,
			descs)))
		{
			numModes = 0;
			break;
		}
		else if (MAKE_DXGI_HRESULT(34) == hr &&
			RemoteMode == allowedAdapterFormatArray[index])
		{
			// DXGI cannot enumerate display modes over a remote session.  Therefore, create a fake display
			// mode for the current screen resolution for the remote session.
			if (0 != GetSystemMetrics(0x1000)) // SM_REMOTESESSION
			{
				DEVMODE DevMode;
				DevMode.dmSize = sizeof(DEVMODE);
				if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode))
				{
					numModes = 1;
					descs[0].Width = DevMode.dmPelsWidth;
					descs[0].Height = DevMode.dmPelsHeight;
					descs[0].Format = RemoteMode;
					descs[0].RefreshRate.Numerator = 60;
					descs[0].RefreshRate.Denominator = 1;
					descs[0].ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
					descs[0].Scaling = DXGI_MODE_SCALING_CENTERED;
					hr = S_OK;
				}
			}
		}

		if (0 == numModes && 0 == index)
		{
			// No R8G8B8A8_UNORM_SRGB modes!
			// Abort the fast-path if we're on it
			allowedAdapterFormatArrayCount_temp =
				sizeof(allowedAdapterFormatArray) / sizeof(allowedAdapterFormatArray[0]);

			continue;
		}

		IF_SUCCEEDED(hr)
		{
			for (unsigned int i = 0; i < numModes; ++i)
			{
				_output_info.displayModeList.push_back(descs[i]);
			}
		}
	}

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D_Outside, const VideoCard_info *, get_info)(
	/* [r] */ unsigned int _adapterOrdinal)
{
	IF_FALSE(_adapterOrdinal < g_Videos.size())
	{
		return nullptr;
	}

	return &g_Videos[_adapterOrdinal];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D_Outside, const VideoCard_Output_info *, get_Output_info)(
	/* [r] */ unsigned int _adapterOrdinal,
	/* [r] */ unsigned int _output)
{
	IF_FALSE(_adapterOrdinal < g_Videos.size())
	{
		return nullptr;
	}

	VideoCard_info * adapterInfo = &g_Videos[_adapterOrdinal];

	if (_output < adapterInfo->output_infoList.size())
	{
		return &adapterInfo->output_infoList[_output];
	}

	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D_Outside, HMONITOR, get_MonitorFromAdapter)(
	/* [r] */ const D3D10_DEVICE_DESC & _desc)
{
	const VideoCard_Output_info * output_info =
		get_Output_info(
		_desc.adapterOrdinal,
		_desc.output);

	IF_INVALID(output_info)
	{
		return nullptr;
	}

	return MonitorFromRect(&output_info->desc.DesktopCoordinates, MONITOR_DEFAULTTONEAREST);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D_Outside, unsigned int, get_AdapterOrdinalFromMonitor)(
	/* [r] */ HMONITOR _monitor)
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
				get_Output_info(iAdapter, iOutput);

			IF_INVALID(output_info)
			{
				return -1;
			}

			// Convert output device name from MBCS to Unicode
			unsigned int size = sizeof(mi.szDevice) / sizeof(mi.szDevice[0]);
			if (wcsncmp(output_info->desc.DeviceName, mi.szDevice, size) == 0)
			{
				return get_info(iAdapter)->ordinal;
			}
		}
	}

	return -1;
}
//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D_Outside, unsigned int, get_OutputOrdinalFromMonitor)(
	/* [r] */ HMONITOR _monitor)
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
				get_Output_info(iAdapter, iOutput);

			IF_INVALID(output_info)
			{
				return -1;
			}

			DXGI_OUTPUT_DESC Desc;
			output_info->dxgiOutput->GetDesc(&Desc);

			if (_monitor == Desc.Monitor)
			{
				return output_info->output;
			}
		}
	}

	return -1;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct3D_Outside, void, destroy)(
	/* [x] */ void)
{
	g_Videos.clear();
}