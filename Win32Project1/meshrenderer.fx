//--------------------------------------------------------------------------------------
// Exercise03.fx
// PIX Workshop GDC 2007
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// o/__   <-- Breakdancin' Bob will guide you through the exercise
// |  (\    
//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
struct VS_BASIC_INPUT
{
	float4 Pos		: POSITION;
	float3 Norm		: NORMAL;
	float2 Tex		: TEXCOORD0;
};

struct VS_TEXTURE_INPUT
{
	float4 Pos		: POSITION;
	float3 Norm		: NORMAL;
	uint index		: BLENDINDICES;
};

struct PS_BASIC_INPUT
{
	float4 Pos		: POSITION;
	float3 Norm		: TEXCOORD0;
	float2 Tex		: TEXCOORD1;
};

//-----------------------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------------------
matrix g_WorldViewProj_Matrix;
matrix g_ViewProj_Matrix;
matrix g_World_Matrix;

//-----------------------------------------------------------------------------------------
// Textures
//-----------------------------------------------------------------------------------------
Texture2D g_Diffuse_Texture;

//-----------------------------------------------------------------------------------------
// image capture
//-----------------------------------------------------------------------------------------
float4 g_TextureCapture;

//-----------------------------------------------------------------------------------------
// color
//-----------------------------------------------------------------------------------------
float4 g_Diffuse;
//-----------------------------------------------------------------------------------------
// Samplers
//-----------------------------------------------------------------------------------------
sampler2D DiffuseSampler = sampler_state
{

	Texture = (g_Diffuse_Texture);
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

//-----------------------------------------------------------------------------------------
// VertexShader: VSBasic
//-----------------------------------------------------------------------------------------
PS_BASIC_INPUT VSBasic(VS_BASIC_INPUT input)
{
	VS_BASIC_INPUT output;

	output.Pos = mul(input.Pos, g_WorldViewProj_Matrix);
	output.Norm = normalize(mul(input.Norm, (float3x3)g_World_Matrix));
	output.Tex = input.Tex;

	return output;
}

PS_BASIC_INPUT VSTexture(VS_TEXTURE_INPUT input)
{
	VS_BASIC_INPUT output;

	output.Pos = mul(input.Pos, g_WorldViewProj_Matrix);
	output.Norm = normalize(mul(input.Norm, (float3x3)g_World_Matrix));

	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSBasic
//-----------------------------------------------------------------------------------------
float4 PSBasic(PS_BASIC_INPUT input) : COLOR0
{
	//return float4( input.Tex, 0, 1 );
	//float2 tex = input.Tex * float2(0.5,0.5);
	//tex.y += 0.5;
	return tex2D(DiffuseSampler, input.Tex);
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSColor
//-----------------------------------------------------------------------------------------
float4 PSColor(PS_BASIC_INPUT input) : COLOR0
{
	return g_Diffuse;
}

//-----------------------------------------------------------------------------------------
// Render state
//-----------------------------------------------------------------------------------------
BlendState AdditiveBlending
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

BlendState UIBlend
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

BlendState NoBlending
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = FALSE;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
};

DepthStencilState DisableDepthWrite
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
};

RasterizerState CullBack
{
	CullMode = BACK;
};

RasterizerState CullFront
{
	CullMode = FRONT;
};

RasterizerState CullNone
{
	CullMode = NONE;
};

//-----------------------------------------------------------------------------------------
// Technique: Renderer
//-----------------------------------------------------------------------------------------
technique10 Render0
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, VSBasic()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSBasic()));

		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullBack);
	}
}

technique10 Texture0
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, VSTexture()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSBasic()));

		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullBack);
	}
}