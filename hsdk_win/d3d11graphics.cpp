#include <hsdk/win/frame/d3d11graphics.h>
#include <hsdk/win/shader/vs_texture.h>
#include <hsdk/win/shader/ps_cuttingtexture.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;


//--------------------------------------------------------------------------------------
AutoRelease<ID3D11VertexShader> D3D11Graphics::VERTEX_SHADER;
AutoRelease<ID3D11PixelShader> D3D11Graphics::PIXEL_SHADER;
AutoRelease<ID3D11InputLayout> D3D11Graphics::INPUT_LAYOUT;
AutoRelease<ID3D11Buffer> D3D11Graphics::VS_CBUFFER;
AutoRelease<ID3D11Buffer> D3D11Graphics::PS_CBUFFER;

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(D3D11Graphics, initialize)(
	/* [none] */ void)
{
	HRESULT hr;

	// vs shader
	IF_FAILED(hr = D3D11::create_VertexShader(
		(const char *)(s_vs_texture),
		*(&VERTEX_SHADER)))
	{
		return hr;
	}

	// vs input layout
	D3D11::D3D11inputEachFormat inputfomat;
	inputfomat.offset = 0;
	inputfomat.format = DXGI_FORMAT_R32_UINT;
	inputfomat.semantic = "BLENDINDICES";

	IF_FAILED(hr = D3D11::create_inputLayout(
		(const char *)(s_vs_texture),
		&inputfomat,
		1,
		D3D11_INPUT_PER_VERTEX_DATA,
		*(&INPUT_LAYOUT)))
	{
		return hr;
	}

	// vs cbuffer
	IF_FAILED(hr = D3D11::create_ContantBuffers(
		sizeof(XMFLOAT4X4),
		D3D11_USAGE_DEFAULT,
		*(&VS_CBUFFER)))
	{
		return hr;
	}

	// pixel
	IF_FAILED(hr = D3D11::create_PixelShader(
		(const char *)(s_ps_cuttingtexture),
		*(&PIXEL_SHADER)))
	{
		return hr;
	}

	// ps cbuffer
	IF_FAILED(hr = D3D11::create_ContantBuffers(
		sizeof(XMFLOAT4),
		D3D11_USAGE_DEFAULT,
		*(&PS_CBUFFER)))
	{
		return hr;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11Graphics, void, render_Begin)(
	/* [none] */ void)
{
	D3D11::CONTEXT->VSSetShader(VERTEX_SHADER, nullptr, 0);
	D3D11::CONTEXT->IASetInputLayout(INPUT_LAYOUT);
	D3D11::CONTEXT->VSSetConstantBuffers(0, 1, &VS_CBUFFER);

	D3D11::CONTEXT->PSSetShader(PIXEL_SHADER, nullptr, 0);
	D3D11::CONTEXT->PSSetConstantBuffers(0, 1, &PS_CBUFFER);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11Graphics, void, render_End)(
	/* [none] */ void)
{
	D3D11::CONTEXT->VSSetShader(nullptr, nullptr, 0);
	D3D11::CONTEXT->IASetInputLayout(nullptr);
	D3D11::CONTEXT->PSSetShader(nullptr, nullptr, 0);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(D3D11Graphics, D3D11Graphics)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(D3D11Graphics, D3D11Graphics)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11Graphics, void, set_image)(
	/* [in] */ const wchar_t * _filename)
{
	D3D11::get_Sampler(D3D11::SAMPLER_DEFAULT, m_sampler);
	D3D11::get_Texture(_filename, m_custom);
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11Graphics, void, set_Font)(
	/* [in] */ const wchar_t * _fontname,
	/* [in] */ unsigned int _fontsize)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11Graphics, void, set_Text)(
	/* [in] */ const wchar_t * _text)
{
	m_text = _text;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11Graphics, void, update)(
	/* [none] */ void)
{
	XMStoreFloat4x4(&m_matrix,
		XMMatrixTranslation(form[0], form[1], 0.0f) *
		XMMatrixScaling(form[2], form[3], 0.0f));
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(D3D11Graphics, void, render)(
	/* [none] */ void)
{
	if (visible)
	{
		D3D11::CONTEXT->UpdateSubresource(VS_CBUFFER, 0, nullptr, &m_matrix, 0, 0);
		D3D11::CONTEXT->UpdateSubresource(PS_CBUFFER, 0, nullptr, form, 0, 0);
		D3D11::CONTEXT->PSSetShaderResources(0, 1, &m_custom);
		D3D11::CONTEXT->PSSetSamplers(0, 1, &m_sampler);
		D3D11::render_Panel(m_vbuffer);
	}
}