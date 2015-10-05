#include <hsdk/win/frame/graphics.h>
#include <hsdk/win/shader/vs_ui.h>
#include <hsdk/win/shader/ps_ui.h>



using namespace hsdk;
using namespace d3d11;
using namespace i::frame;
using namespace win::frame;


// grobal
struct InputLayoutFormat
{
	XMFLOAT3 position;
	XMFLOAT2 texcoord;
};


//--------------------------------------------------------------------------------------
AutoRelease<ID3D11RasterizerState> Graphics::CULL_BACK_RASTERIZER;
AutoRelease<ID3D11DepthStencilState> Graphics::FRONT_CLIPPING_DEPTH;
AutoRelease<ID3D11VertexShader> Graphics::VERTEX_SHADER;
AutoRelease<ID3D11PixelShader> Graphics::PIXEL_SHADER;
AutoRelease<ID3D11InputLayout> Graphics::INPUT_LAYOUT;
AutoRelease<ID3D11Buffer> Graphics::VS_WIDE_CBUFFER;
AutoRelease<ID3D11Buffer> Graphics::VS_CLIP_CBUFFER;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Graphics, initialize)(
	/* [x] */ void)
{
	HRESULT hr;

	// rasterizer state
	D3D11_RASTERIZER_DESC rasterDesc;

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	IF_FAILED(hr = D3D11::DEVICE->CreateRasterizerState(
		&rasterDesc,
		&CULL_BACK_RASTERIZER))
	{
		return hr;
	}

	// depth state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	IF_FAILED(hr = D3D11::DEVICE->CreateDepthStencilState(
		&depthStencilDesc,
		&FRONT_CLIPPING_DEPTH))
	{
		return hr;
	}

	// vs shader
	IF_FAILED(hr = D3D11::create_VertexShaderForHeader(
		*(&VERTEX_SHADER),
		(const char *)(s_vs_ui),
		sizeof(s_vs_ui)))
	{
		return hr;
	}

	// vs input layout
	D3D11::D3D11inputEachFormat inputfomats[2];

	inputfomats[0].offset = offsetof(InputLayoutFormat, position);
	inputfomats[0].format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputfomats[0].semantic = "POSITION";

	inputfomats[1].offset = offsetof(InputLayoutFormat, texcoord);
	inputfomats[1].format = DXGI_FORMAT_R32G32_FLOAT;
	inputfomats[1].semantic = "TEXCOORD";

	IF_FAILED(hr = D3D11::create_inputLayoutForHeader(
		*(&INPUT_LAYOUT),
		(const char *)(s_vs_ui),
		sizeof(s_vs_ui),
		inputfomats,
		2,
		D3D11_INPUT_PER_VERTEX_DATA))
	{
		return hr;
	}

	// vs cbuffer
	IF_FAILED(hr = D3D11::create_ContantBuffers(
		*(&VS_WIDE_CBUFFER),
		sizeof(XMFLOAT4),
		D3D11_USAGE_DEFAULT))
	{
		return hr;
	}

	// ps cbuffer
	IF_FAILED(hr = D3D11::create_ContantBuffers(
		*(&VS_CLIP_CBUFFER),
		sizeof(XMFLOAT4),
		D3D11_USAGE_DEFAULT))
	{
		return hr;
	}

	// pixel
	IF_FAILED(hr = D3D11::create_PixelShaderForHeader(
		*(&PIXEL_SHADER),
		(const char *)(s_ps_ui),
		sizeof(s_ps_ui)))
	{
		return hr;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Graphics, void, destroy)(
	/* [x] */ void)
{
	CULL_BACK_RASTERIZER.~AutoRelease();
	FRONT_CLIPPING_DEPTH.~AutoRelease();

	VS_WIDE_CBUFFER.~AutoRelease();
	VS_CLIP_CBUFFER.~AutoRelease();

	INPUT_LAYOUT.~AutoRelease();

	VERTEX_SHADER.~AutoRelease();
	PIXEL_SHADER.~AutoRelease();
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Graphics, void, shader_on)(
	/* [x] */ void)
{
	D3D11::CONTEXT->OMSetDepthStencilState(FRONT_CLIPPING_DEPTH, 0);
	D3D11::CONTEXT->RSSetState(CULL_BACK_RASTERIZER);

	D3D11::CONTEXT->VSSetShader(VERTEX_SHADER, nullptr, 0);
	D3D11::CONTEXT->PSSetShader(PIXEL_SHADER, nullptr, 0);

	D3D11::CONTEXT->IASetInputLayout(INPUT_LAYOUT);

	D3D11::CONTEXT->VSSetConstantBuffers(0, 1, &VS_WIDE_CBUFFER);
	D3D11::CONTEXT->VSSetConstantBuffers(1, 1, &VS_CLIP_CBUFFER);

	D3D11::CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Graphics, void, shader_off)(
	/* [x] */ void)
{
	D3D11::CONTEXT->OMSetDepthStencilState(nullptr, 0);
	D3D11::CONTEXT->RSSetState(nullptr);

	D3D11::CONTEXT->VSSetShader(nullptr, nullptr, 0);
	D3D11::CONTEXT->PSSetShader(nullptr, nullptr, 0);

	D3D11::CONTEXT->IASetInputLayout(nullptr);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Graphics, void, set_Wide)(
	/* [r] */ const float(&_rectangle)[4])
{
	D3D11::CONTEXT->UpdateSubresource(VS_WIDE_CBUFFER, 0, nullptr, _rectangle, 0, 0);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Graphics, void, set_Clip)(
	/* [r] */ const float(&_clip)[4])
{
	D3D11::CONTEXT->UpdateSubresource(VS_CLIP_CBUFFER, 0, nullptr, _clip, 0, 0);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Graphics, Graphics)(void)
: m_imageW(1.0f), m_imageH(1.0f), m_Custom(nullptr), m_Sampler(nullptr)
{
	m_uvRectangle[0] = 0.0f;
	m_uvRectangle[1] = 0.0f;
	m_uvRectangle[2] = 1.0f;
	m_uvRectangle[3] = 1.0f;

	HRESULT hr;
	IF_FAILED(hr = D3D11::create_Panel(*(&m_Panel), {
		XMFLOAT2(0.0f, 0.0f),
		XMFLOAT2(1.0f, 0.0f),
		XMFLOAT2(1.0f, 1.0f),
		XMFLOAT2(0.0f, 1.0f)
	}, D3D11_USAGE_DYNAMIC))
	{
		throw hr;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(Graphics, Graphics)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Graphics, void, set_image)(
	/* [r] */ const wchar_t * _filename)
{
	D3D11::get_Sampler(m_Sampler, D3D11::SAMPLER_DEFAULT);
	D3D11::get_Texture(m_Custom, _filename);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Graphics, void, set_imageDetail)(
	/* [r] */ float _imageW,
	/* [r] */ float _imageH,
	/* [r] */ const float(&_rectangle)[4])
{
	m_imageW = _imageW;
	m_imageH = _imageH;

	m_uvRectangle[0] = _rectangle[0];
	m_uvRectangle[1] = _rectangle[1];
	m_uvRectangle[2] = _rectangle[2];
	m_uvRectangle[3] = _rectangle[3];
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Graphics, void, set_Font)(
	/* [r] */ const wchar_t * _fontname,
	/* [r] */ unsigned int _fontsize)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Graphics, void, set_Text)(
	/* [r] */ const wchar_t * _text)
{
	m_Text = _text;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Graphics, void, update_Panel)(
	/* [r] */ const float(&_rectangle)[4])
{
	// 버텍스 버퍼 갱신
	D3D11_MAPPED_SUBRESOURCE map_subresource;
	IF_SUCCEEDED(D3D11::CONTEXT->Map(m_Panel, 0, D3D11_MAP_WRITE_DISCARD, 0, &map_subresource))
	{
		D3D11::panel_UV * dataPtr;
		dataPtr = (D3D11::panel_UV*)map_subresource.pData;
		if (dataPtr)
		{
			float uv[4] = {
				m_uvRectangle[0] / m_imageW,
				m_uvRectangle[1] / m_imageH,
				(m_uvRectangle[0] + m_uvRectangle[2]) / m_imageW,
				(m_uvRectangle[1] + m_uvRectangle[3]) / m_imageH
			};

			float form[4] = {
				_rectangle[0],
				_rectangle[1],
				_rectangle[0] + _rectangle[2],
				_rectangle[1] + _rectangle[3],
			};

			dataPtr[0].pos = XMFLOAT3(form[0], form[1], 0.0f);
			dataPtr[0].tex = XMFLOAT2(uv[0], uv[1]);

			dataPtr[1].pos = XMFLOAT3(form[2], form[1], 0.0f);
			dataPtr[1].tex = XMFLOAT2(uv[2], uv[1]);

			dataPtr[2].pos = XMFLOAT3(form[2], form[3], 0.0f);
			dataPtr[2].tex = XMFLOAT2(uv[2], uv[3]);

			dataPtr[3].pos = XMFLOAT3(form[0], form[3], 0.0f);
			dataPtr[3].tex = XMFLOAT2(uv[0], uv[3]);
		}

		D3D11::CONTEXT->Unmap(m_Panel, 0);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Graphics, void, render_Panel)(
	/* [x] */ void)
{
	if (m_Custom)
	{
		D3D11::CONTEXT->PSSetShaderResources(0, 1, &m_Custom);
		D3D11::CONTEXT->PSSetSamplers(0, 1, &m_Sampler);
		D3D11::render_Panel(m_Panel);
	}
}