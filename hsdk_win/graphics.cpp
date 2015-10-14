#include <hsdk/win/frame/graphics.h>
#include <hsdk/win/shader/vs_ui.h>
#include <hsdk/win/shader/ps_ui.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;
using namespace direct3d;


//--------------------------------------------------------------------------------------
// Grobal declare function
//--------------------------------------------------------------------------------------

// 설명 : 
DECL_FUNC_T(D3DCOLORVALUE, get_ColorToValue)(
	/* [r] */ D3DCOLOR _color);

//--------------------------------------------------------------------------------------
// Grobal D3D10 Variable
//--------------------------------------------------------------------------------------

// 설명 : 
direct3d::D3D10_Manager * g_refManager;

// 설명 : Effect used to render UI with D3D10
AutoRelease<ID3D10Effect> g_Graphics_Effect;

// 설명 : Technique: RenderUI
ID3D10EffectTechnique * g_Graphics_RenderUI_Technique;

// 설명 : Technique: RenderUI without texture
ID3D10EffectTechnique * g_Graphics_RenderUIUntex_Technique;

// 설명 :
ID3D10EffectShaderResourceVariable * g_Graphics_Texture;

// 설명 :
AutoRelease<ID3D10InputLayout> g_Graphics_inputLayout;

//--------------------------------------------------------------------------------------
AutoRelease<ID3D10RasterizerState> CULL_BACK_RASTERIZER;
AutoRelease<ID3D10DepthStencilState> FRONT_CLIPPING_DEPTH;

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Graphics, initialize)(
	/* [r] */ direct3d::D3D10_Manager & _manager)
{
	const char strUIEffectFile[] = \
		"Texture2D g_Texture;"\
		""\
		"SamplerState Sampler"\
		"{"\
		"    Filter = MIN_MAG_MIP_LINEAR;"\
		"    AddressU = Wrap;"\
		"    AddressV = Wrap;"\
		"};"\
		""\
		"BlendState UIBlend"\
		"{"\
		"    AlphaToCoverageEnable = FALSE;"\
		"    BlendEnable[0] = TRUE;"\
		"    SrcBlend = SRC_ALPHA;"\
		"    DestBlend = INV_SRC_ALPHA;"\
		"    BlendOp = ADD;"\
		"    SrcBlendAlpha = ONE;"\
		"    DestBlendAlpha = ZERO;"\
		"    BlendOpAlpha = ADD;"\
		"    RenderTargetWriteMask[0] = 0x0F;"\
		"};"\
		""\
		"BlendState NoBlending"\
		"{"\
		"    BlendEnable[0] = FALSE;"\
		"    RenderTargetWriteMask[0] = 0x0F;"\
		"};"\
		""\
		"DepthStencilState DisableDepth"\
		"{"\
		"    DepthEnable = false;"\
		"};"\
		"DepthStencilState EnableDepth"\
		"{"\
		"    DepthEnable = true;"\
		"};"\
		"struct VS_OUTPUT"\
		"{"\
		"    float4 Pos : SV_POSITION;"\
		"    float4 Dif : COLOR;"\
		"    float2 Tex : TEXCOORD;"\
		"};"\
		""\
		"VS_OUTPUT VS( float3 vPos : POSITION,"\
		"              float4 Dif : COLOR,"\
		"              float2 vTexCoord0 : TEXCOORD )"\
		"{"\
		"    VS_OUTPUT Output;"\
		""\
		"    Output.Pos = float4( vPos, 1.0f );"\
		"    Output.Dif = Dif;"\
		"    Output.Tex = vTexCoord0;"\
		""\
		"    return Output;"\
		"}"\
		""\
		"float4 PS( VS_OUTPUT In ) : SV_Target"\
		"{"\
		"    return g_Texture.Sample( Sampler, In.Tex ) * In.Dif;"\
		"}"\
		""\
		"float4 PSUntex( VS_OUTPUT In ) : SV_Target"\
		"{"\
		"    return In.Dif;"\
		"}"\
		""\
		"technique10 RenderUI"\
		"{"\
		"    pass P0"\
		"    {"\
		"        SetVertexShader( CompileShader( vs_4_0, VS() ) );"\
		"        SetGeometryShader( NULL );"\
		"        SetPixelShader( CompileShader( ps_4_0, PS() ) );"\
		"        SetDepthStencilState( DisableDepth, 0 );"\
		"        SetBlendState( UIBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );"\
		"    }"\
		"}"\
		"technique10 RenderUIUntex"\
		"{"\
		"    pass P0"\
		"    {"\
		"        SetVertexShader( CompileShader( vs_4_0, VS() ) );"\
		"        SetGeometryShader( NULL );"\
		"        SetPixelShader( CompileShader( ps_4_0, PSUntex() ) );"\
		"        SetDepthStencilState( DisableDepth, 0 );"\
		"        SetBlendState( UIBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );"\
		"    }"\
		"}"\
		"technique10 RestoreState"\
		"{"\
		"    pass P0"\
		"    {"\
		"        SetDepthStencilState( EnableDepth, 0 );"\
		"        SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );"\
		"    }"\
		"}";

	const unsigned int UIEffectFileSize = sizeof(strUIEffectFile);

	// Create the UI effect object
	HRESULT hr;
	IF_FAILED(hr = D3DX10CreateEffectFromMemory(
		strUIEffectFile,
		UIEffectFileSize,
		NULL,
		NULL,
		NULL,
		"fx_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		_manager.get_D3D10_Device(),
		NULL,
		NULL,
		&g_Graphics_Effect,
		NULL,
		NULL))
	{
		return hr;
	}

	g_Graphics_RenderUI_Technique = g_Graphics_Effect->GetTechniqueByName("RenderUI");
	g_Graphics_RenderUIUntex_Technique = g_Graphics_Effect->GetTechniqueByName("RenderUIUntex");

	g_Graphics_Texture = g_Graphics_Effect->GetVariableByName("g_Texture")->AsShaderResource();

	D3D10_PASS_DESC PassDesc;
	IF_FAILED(hr = g_Graphics_RenderUI_Technique->GetPassByIndex(0)->GetDesc(&PassDesc))
	{
		return hr;
	}

	// Create input layout
	const D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	IF_FAILED(hr = _manager.get_D3D10_Device()->CreateInputLayout(
		layout,
		3,
		PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize,
		&g_Graphics_inputLayout))
	{
		return hr;
	}

	g_refManager = &_manager;

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, destroy)(
	/* [x] */ void)
{
	g_Graphics_inputLayout.~AutoRelease();
	g_Graphics_Effect.~AutoRelease();
	g_Graphics_RenderUI_Technique = nullptr;
	g_Graphics_RenderUIUntex_Technique = nullptr;
	g_Graphics_Texture = nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Graphics, Graphics)(void)
: m_imageW(1.0f), m_imageH(1.0f), my_refCustom(nullptr), my_refSampler(nullptr)
{
	m_uvRectangle[0] = 0.0f;
	m_uvRectangle[1] = 0.0f;
	m_uvRectangle[2] = 1.0f;
	m_uvRectangle[3] = 1.0f;

	HRESULT hr;
	IF_FAILED(hr = g_refManager->create_UIPanel(
		&my_Panel,
		D3D10_USAGE_DYNAMIC))
	{
		throw hr;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(Graphics, Graphics)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_image)(
	/* [r] */ const wchar_t * _filename)
{
	g_refManager->get_Sampler(&my_refSampler, direct3d::SAMPLER_DEFAULT);
	g_refManager->get_Texture(&my_refCustom, _filename);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_imageDetail)(
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
CLASS_IMPL_FUNC_T(Graphics, void, set_Font)(
	/* [r] */ const wchar_t * _fontname,
	/* [r] */ unsigned int _fontsize)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_Text)(
	/* [r] */ const wchar_t * _text)
{
	m_Text = _text;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, update)(
	/* [r] */ const float(&_rectangle)[4])
{
	// Convert the rect from screen coordinates to clip space coordinates.
	float Left, Right, Top, Bottom;

	Left = ((_rectangle[0] / g_refManager->get_Width()) * 2.0f) - 1.0f;
	Right = ((_rectangle[2] / g_refManager->get_Width()) * 2.0f) - 1.0f;

	Top = 1.0f - ((_rectangle[1] / g_refManager->get_Height()) * 2.0f);
	Bottom = 1.0f - ((_rectangle[3] / g_refManager->get_Height()) * 2.0f);

	float uv[4] = {
		m_uvRectangle[0] / m_imageW,
		m_uvRectangle[1] / m_imageH,
		(m_uvRectangle[0] + m_uvRectangle[2]) / m_imageW,
		(m_uvRectangle[1] + m_uvRectangle[3]) / m_imageH
	};

	D3D10_SCREEN_VERTEX vertices[4] =
	{
		{ Left, Top, 0.5f, get_ColorToValue(0), uv[0], uv[1] },
		{ Right, Top, 0.5f, get_ColorToValue(0), uv[2], uv[1] },
		{ Left, Bottom, 0.5f, get_ColorToValue(0), uv[0], uv[3] },
		{ Right, Bottom, 0.5f, get_ColorToValue(0), uv[2], uv[3] },
	};

	D3D10_SCREEN_VERTEX* pVB;
	if (SUCCEEDED(my_Panel->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&pVB)))
	{
		CopyMemory(pVB, vertices, sizeof(vertices));
		my_Panel->Unmap();
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, render)(
	/* [x] */ void)
{
	ID3D10Device * const device =
		g_refManager->get_D3D10_Device();

	// Set the quad VB as current
	UINT stride = sizeof(D3D10_SCREEN_VERTEX);
	UINT offset = 0;

	device->IASetVertexBuffers(0, 1, &my_Panel, &stride, &offset);
	device->IASetInputLayout(g_Graphics_inputLayout);
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Draw dialog background
	D3D10_TECHNIQUE_DESC techDesc;
	g_Graphics_RenderUIUntex_Technique->GetDesc(&techDesc);
	for (unsigned int index = 0; index < techDesc.Passes; ++index)
	{
		g_Graphics_RenderUI_Technique->GetPassByIndex(index)->Apply(0);
		device->Draw(4, 0);
	}
}

//--------------------------------------------------------------------------------------
// Grobal implements function
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(D3DCOLORVALUE, get_ColorToValue)(
	/* [r] */ D3DCOLOR _color)
{
	D3DCOLORVALUE value = {
		((_color >> 16) & 0xFF) / 255.0f,
		((_color >> 8) & 0xFF) / 255.0f,
		(_color & 0xFF) / 255.0f,
		((_color >> 24) & 0xFF) / 255.0f };

	return value;
}
