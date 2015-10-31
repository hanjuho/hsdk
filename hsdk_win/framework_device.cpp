#include <hsdk/win/framework_device.h>
#include <vector>



using namespace hsdk::framework;


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Framework_DeviceFactory, create9)(
	_Out_ Framework_Device & _device,
	_In_ const D3D9_DEVICE_DESC & _desc,
	_In_ const Framework_Callbacks * _callback)
{
	// Try to create the device with the chosen settings
	IDirect3D9 * d3d9 = nullptr;
	IF_INVALID((d3d9 = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		DEL_COM(d3d9);
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;

	D3DCAPS9 caps;
	IF_FAILED(hr = d3d9->GetDeviceCaps(
		_desc.adapterOrdinal,
		_desc.deviceType,
		&caps))
	{
		return hr;
	}

	if (_callback)
	{
		CALLBACK_IS_D3D9_DEVICE_ACCEPTABLE isD3D9DeviceAcceptable =
			_callback->isD3D9DeviceAcceptableFunc;

		if (isD3D9DeviceAcceptable)
		{
			IF_FALSE(isD3D9DeviceAcceptable(
				caps, _desc.adapterFormat,
				_desc.pp.BackBufferFormat,
				_desc.pp.Windowed,
				_callback->isD3D9DeviceAcceptableFuncUserContext))
			{
				return E_ABORT;
			}
		}
	}

	// create device
	AutoRelease<IDirect3DDevice9> d3d9Device;
	IF_FAILED(hr = d3d9->CreateDevice(
		_desc.adapterOrdinal,
		_desc.deviceType,
		_desc.pp.hDeviceWindow,
		_desc.behaviorFlags,
		(D3DPRESENT_PARAMETERS*)&_desc.pp,
		&d3d9Device))
	{
		return hr;
	}

	D3DSURFACE_DESC backBufferDesc;

	// Update back buffer desc before calling app's device callbacks
	AutoRelease<IDirect3DSurface9> backBuffer;
	IF_FAILED(hr = d3d9Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer))
	{
		return hr;
	}

	backBuffer->GetDesc(&backBufferDesc);

	_device.d3d9.~AutoRelease();
	*(&_device.d3d9) = d3d9;

	_device.d3d9Device = d3d9Device;
	memcpy(&_device.backBuffer_Desc, &backBufferDesc, sizeof(D3DSURFACE_DESC));

	D3DXCreateSprite(d3d9Device, &_device.d3d9Sprite);

	if (_callback)
	{
		// Call the app's device created callback if non-nullptr
		CALLBACK_D3D9_DEVICE_CREATED pCallbackDeviceCreated =
			_callback->d3d9DeviceCreatedFunc;

		if (pCallbackDeviceCreated)
		{
			hr = pCallbackDeviceCreated(
				d3d9Device,
				backBufferDesc,
				_callback->d3d9DeviceCreatedFuncUserContext);
		}
	}

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Framework_DeviceFactory, reset9)(
	_Out_ Framework_Device & _device,
	_In_ const D3D9_DEVICE_DESC & _desc,
	_In_ const Framework_Callbacks * _callback)
{
	HRESULT hr;

	// Reset the device
	IF_FAILED(hr = _device.d3d9Device->Reset((D3DPRESENT_PARAMETERS*)&_desc.pp))
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
	AutoRelease<IDirect3DSurface9> backBuffer;
	IF_FAILED(hr = _device.d3d9Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer))
	{
		return hr;
	}

	backBuffer->GetDesc(&_device.backBuffer_Desc);

	if (_callback)
	{
		// Call the app's OnDeviceReset callback
		CALLBACK_D3D9_DEVICE_RESET callback_DeviceReset =
			_callback->d3d9DeviceResetFunc;

		if (callback_DeviceReset)
		{
			IF_FAILED(hr = callback_DeviceReset(
				_device.d3d9Device,
				_device.backBuffer_Desc,
				_callback->d3d9DeviceResetFuncUserContext))
			{
				// If callback failed				
				return hr;
			}
		}
	}

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Framework_DeviceFactory, restore9)(
	_Out_ Framework_Device & _device,
	_In_ const D3D9_DEVICE_DESC & _desc,
	_In_ const Framework_Callbacks * _callback)
{
	HRESULT hr;
	if (SUCCEEDED(hr = _device.d3d9Device->Present(nullptr, nullptr, nullptr, nullptr)))
	{
		return hr;
	}
	else
	{
		if (hr != D3DERR_DEVICELOST)
		{
			return E_FAIL;
		}
	}

	if (_callback)
	{
		// Call the app's device lost callback
		CALLBACK_D3D9_DEVICE_LOST callback_DeviceLost =
			_callback->d3d9DeviceLostFunc;

		if (callback_DeviceLost)
		{
			callback_DeviceLost(
				_callback->d3d9DeviceLostFuncUserContext);
		}
	}

	// Reset the device
	IF_FAILED(hr = _device.d3d9Device->Reset((D3DPRESENT_PARAMETERS*)&_desc.pp))
	{
		if (hr == D3DERR_DEVICELOST)
		{
			// Reset could legitimately fail if the device is lost
			return E_ABORT;
		}
		else
		{
			return hr;
		}
	}

	// Update back buffer desc before calling app's device callbacks
	AutoRelease<IDirect3DSurface9> backBuffer;
	IF_SUCCEEDED(_device.d3d9Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer))
	{
		backBuffer->GetDesc(&_device.backBuffer_Desc);

		if (_callback)
		{
			// Call the app's OnDeviceReset callback
			CALLBACK_D3D9_DEVICE_RESET callback_DeviceReset =
				_callback->d3d9DeviceResetFunc;

			if (callback_DeviceReset)
			{
				IF_FAILED(hr = callback_DeviceReset(
					_device.d3d9Device,
					_device.backBuffer_Desc,
					_callback->d3d9DeviceResetFuncUserContext))
				{
					CALLBACK_D3D9_DEVICE_DESTROYED callback_DeviceDistroyed =
						_callback->d3d9DeviceDestroyedFunc;

					if (callback_DeviceDistroyed)
					{
						callback_DeviceDistroyed(
							_callback->d3d9DeviceDestroyedFuncUserContext);
					}

					// If callback failed				
					return create9(_device, _desc, _callback);
				}
			}
		}
	}

	return hr;
}

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Framework_DeviceFactory, create10)(
	_Out_ Framework_Device & _device,
	_In_ const D3D10_DEVICE_DESC & _desc,
	_In_ const Framework_Callbacks * _callback,
	_In_ BOOL _createRenderTarget)
{
	IDXGIFactory * dxgiFactory = nullptr;
	IF_FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxgiFactory))
	{
		DEL_COM(dxgiFactory);
		return E_ABORT;
	}

	HRESULT hr = S_OK;

	if (_callback)
	{
		CALLBACK_IS_D3D10_DEVICE_ACCEPTABLE isD3D10DeviceAcceptable =
			_callback->isD3D10DeviceAcceptableFunc;

		if (isD3D10DeviceAcceptable)
		{
			IF_FALSE(isD3D10DeviceAcceptable(
				_desc.adapterOrdinal,
				_desc.output,
				_desc.driverType,
				_desc.sd.BufferDesc.Format,
				_desc.sd.Windowed,
				_callback->isD3D10DeviceAcceptableFuncUserContext))
			{
				return E_ABORT;
			}
		}
	}

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
			return hr;
		}
	}

	// Try creating the D3D10.1 device first
	AutoRelease<ID3D10Device1> d3d10Device1;
	AutoRelease<ID3D10Device> d3d10Device;
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
	AutoRelease<IDXGISwapChain> dxgiSwapChain;
	IF_FAILED(hr = dxgiFactory->CreateSwapChain(
		d3d10Device,
		(DXGI_SWAP_CHAIN_DESC *)&_desc.sd,
		&dxgiSwapChain))
	{
		return hr;
	}

	// Call the app's device created callback if non-nullptr
	DXGI_SURFACE_DESC backBufferSurfaceDesc;

	AutoRelease<ID3D10Texture2D> dxgiBackBuffer;
	IF_FAILED(hr = dxgiSwapChain->GetBuffer(0, __uuidof(*dxgiBackBuffer), (void**)&dxgiBackBuffer))
	{
		return hr;
	}

	D3D10_TEXTURE2D_DESC TexDesc;
	dxgiBackBuffer->GetDesc(&TexDesc);

	backBufferSurfaceDesc.Width = (unsigned int)TexDesc.Width;
	backBufferSurfaceDesc.Height = (unsigned int)TexDesc.Height;
	backBufferSurfaceDesc.Format = TexDesc.Format;
	backBufferSurfaceDesc.SampleDesc = TexDesc.SampleDesc;

	_device.dxgiFactory.~AutoRelease();
	*(&_device.dxgiFactory) = dxgiFactory;

	_device.d3d10Device1 = d3d10Device1;
	_device.d3d10Device = d3d10Device;
	_device.dxgiSwapChain = dxgiSwapChain;
	_device.dxgiBackBuffer = dxgiBackBuffer;

	memcpy(&_device.dxgiBackBuffer_Desc, &backBufferSurfaceDesc, sizeof(DXGI_SURFACE_DESC));

	D3DX10CreateSprite(d3d10Device, 0, &_device.d3d10Sprite);

	if (_callback)
	{
		CALLBACK_D3D10_DEVICE_CREATED callback_DeviceCreated =
			_callback->d3d10DeviceCreatedFunc;

		if (callback_DeviceCreated != nullptr)
		{
			hr = callback_DeviceCreated(
				d3d10Device,
				backBufferSurfaceDesc,
				_callback->d3d10DeviceCreatedFuncUserContext);
		}
	}

	IF_FAILED(hr)
	{
		return hr;
	}

	if (_callback)
	{
		// Call the app's swap chain reset callback if non-nullptr
		CALLBACK_D3D10_SWAPCHAINRESIZED callback_SwapChainResized =
			_callback->d3d10SwapChainResizedFunc;

		if (callback_SwapChainResized)
		{
			IF_FAILED(hr = callback_SwapChainResized(
				d3d10Device,
				dxgiSwapChain,
				backBufferSurfaceDesc,
				_callback->d3d10SwapChainResizedFuncUserContext))
			{
				return hr;
			}
		}
	}

	if (_createRenderTarget)
	{
		return setup_RenderTarget(_device, _desc);
	}
	else
	{
		return hr;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Framework_DeviceFactory, resize10)(
	_Out_ Framework_Device & _device,
	_In_ const D3D10_DEVICE_DESC & _desc,
	_In_ const Framework_Callbacks * _callback)
{
	// SetFullscreenState causes a WM_SIZE message to be sent to the window.  The WM_SIZE message calls
	// DXUTCheckForDXGIBufferChange which normally stores the new height and width in 
	// pDeviceSettings->d3d10.sd.BufferDesc.  SetDoNotStoreBufferSize tells DXUTCheckForDXGIBufferChange
	// not to store the height and width so that we have the correct values when calling ResizeTarget.
	HRESULT hr;

	unsigned int flag = 0;
	if (_desc.sd.Windowed)
	{
		IF_FAILED(hr = _device.dxgiSwapChain->SetFullscreenState(FALSE, nullptr))
		{
			return hr;
		}
	}
	else
	{
		IF_FAILED(hr = _device.dxgiSwapChain->SetFullscreenState(TRUE, nullptr))
		{
			return hr;
		}

		flag = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	}

	_device.d3d10Device->ClearState();

	// if do not initialize folow, to call ResizeBuffers fail
	_device.dxgiBackBuffer.~AutoRelease();
	_device.d3d10RTV.~AutoRelease();
	_device.d3d10DSV.~AutoRelease();
	_device.d3d10DepthStencil.~AutoRelease();
	_device.d3d10DefaultRasterizerState.~AutoRelease();

	IF_FAILED(hr = _device.dxgiSwapChain->ResizeBuffers(
		_desc.sd.BufferCount,
		_desc.sd.BufferDesc.Width,
		_desc.sd.BufferDesc.Height,
		_desc.sd.BufferDesc.Format,
		flag))
	{
		return hr;
	}

	AutoRelease<ID3D10Texture2D> dxgiBackBuffer;
	IF_FAILED(hr = _device.dxgiSwapChain->GetBuffer(0, __uuidof(*dxgiBackBuffer), (void**)&dxgiBackBuffer))
	{
		return hr;

	}

	D3D10_TEXTURE2D_DESC TexDesc;
	dxgiBackBuffer->GetDesc(&TexDesc);

	DXGI_SURFACE_DESC & dxgiBackBuffer_Desc = _device.dxgiBackBuffer_Desc;
	dxgiBackBuffer_Desc.Width = (unsigned int)TexDesc.Width;
	dxgiBackBuffer_Desc.Height = (unsigned int)TexDesc.Height;
	dxgiBackBuffer_Desc.Format = TexDesc.Format;
	dxgiBackBuffer_Desc.SampleDesc = TexDesc.SampleDesc;

	_device.dxgiBackBuffer = dxgiBackBuffer;

	if (_callback)
	{
		CALLBACK_D3D10_SWAPCHAINRESIZED callback_SwapChainResized =
			_callback->d3d10SwapChainResizedFunc;

		if (callback_SwapChainResized)
		{
			hr = callback_SwapChainResized(
				_device.d3d10Device,
				_device.dxgiSwapChain,
				_device.dxgiBackBuffer_Desc,
				_callback->d3d10SwapChainResizedFuncUserContext);
		}

	}

	return setup_RenderTarget(_device, _desc);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Framework_DeviceFactory, reset10)(
	_Out_ Framework_Device & _device,
	_In_ const D3D10_DEVICE_DESC & _desc,
	_In_ const Framework_Callbacks * _callback,
	_In_ BOOL _resetAll)
{
	DXGI_SWAP_CHAIN_DESC SCDesc;
	_device.dxgiSwapChain->GetDesc(&SCDesc);

	if (_resetAll || SCDesc.OutputWindow != _desc.sd.OutputWindow)
	{
		if (_callback)
		{
			// Call releasing	
			CALLBACK_D3D10_SWAPCHAINRELEASING callback_SwapChainReleasing =
				_callback->d3d10SwapChainReleasingFunc;

			if (callback_SwapChainReleasing)
			{
				callback_SwapChainReleasing(
					_callback->d3d10SwapChainResizedFuncUserContext);
			}

			if (_device.d3d10Device)
			{
				CALLBACK_D3D10_DEVICE_DESTROYED callback_DeviceDestroyed =
					_callback->d3d10DeviceDestroyedFunc;

				if (callback_DeviceDestroyed)
				{
					callback_DeviceDestroyed(
						_callback->d3d10DeviceDestroyedFuncUserContext);
				}
			}
		}

		return create10(_device, _desc, _callback);
	}
	else if (SCDesc.BufferDesc.Format != _desc.sd.BufferDesc.Format)
	{
		if (_callback)
		{
			// Call releasing	
			CALLBACK_D3D10_SWAPCHAINRELEASING callback_SwapChainReleasing =
				_callback->d3d10SwapChainReleasingFunc;

			if (callback_SwapChainReleasing)
			{
				callback_SwapChainReleasing(
					_callback->d3d10SwapChainResizedFuncUserContext);
			}
		}

		HRESULT hr;

		// Create the swapchain	
		AutoRelease<IDXGISwapChain> dxgiSwapChain;
		IF_FAILED(hr = _device.dxgiFactory->CreateSwapChain(
			_device.d3d10Device,
			(DXGI_SWAP_CHAIN_DESC *)&_desc.sd,
			&dxgiSwapChain))
		{
			return hr;
		}

		_device.dxgiSwapChain = dxgiSwapChain;

		return setup_RenderTarget(_device, _desc);
	}

	return resize10(_device, _desc, _callback);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Framework_DeviceFactory, setup_RenderTarget)(
	_Out_ Framework_Device & _device,
	_In_ const D3D10_DEVICE_DESC & _desc)
{
	HRESULT hr;

	// Setup the viewport to match the backbuffer
	D3D10_VIEWPORT & vp = _device.d3d10ViewPort;
	vp.Width = _desc.sd.BufferDesc.Width;
	vp.Height = _desc.sd.BufferDesc.Height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	_device.d3d10Device->RSSetViewports(1, &vp);

	// Create the render target view
	AutoRelease<ID3D10RenderTargetView> d3d10RTV;
	IF_FAILED(hr = _device.d3d10Device->CreateRenderTargetView(
		_device.dxgiBackBuffer,
		nullptr,
		&d3d10RTV))
	{
		return hr;
	}

	if (_desc.autoCreateDepthStencil)
	{
		D3D10_TEXTURE2D_DESC descDepth;
		descDepth.Width = _desc.sd.BufferDesc.Width;
		descDepth.Height = _desc.sd.BufferDesc.Height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = _desc.autoDepthStencilFormat;
		descDepth.SampleDesc.Count = _desc.sd.SampleDesc.Count;
		descDepth.SampleDesc.Quality = _desc.sd.SampleDesc.Quality;
		descDepth.Usage = D3D10_USAGE_DEFAULT;
		descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		// Create depth stencil texture
		AutoRelease<ID3D10Texture2D> d3d10DepthStencil;
		IF_FAILED(hr = _device.d3d10Device->CreateTexture2D(
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

		AutoRelease<ID3D10DepthStencilView> d3d10DSV;
		IF_FAILED(hr = _device.d3d10Device->CreateDepthStencilView(
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

		AutoRelease<ID3D10RasterizerState> d3d10RasterizerState;
		IF_FAILED(hr = _device.d3d10Device->CreateRasterizerState(&RSDesc, &d3d10RasterizerState))
		{
			return hr;
		}

		// Set the render targets
		_device.d3d10Device->RSSetState(d3d10RasterizerState);
		_device.d3d10Device->OMSetRenderTargets(1, &d3d10RTV, d3d10DSV);

		// initialize
		_device.d3d10DSV = d3d10DSV;
		_device.d3d10DepthStencil = d3d10DepthStencil;
		_device.d3d10DefaultRasterizerState = d3d10RasterizerState;
	}

	// initialize
	_device.d3d10RTV = d3d10RTV;

	return hr;
}