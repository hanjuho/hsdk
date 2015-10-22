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

struct VS_SKINNED_INPUT
{
	float4 Pos		: POSITION;
	float3 Norm		: NORMAL;
	float2 Tex		: TEXCOORD0;
	float4 Color	: COLOR0;
	uint4 Bindexs	: BLENDINDICES;
	float4 Bweight	: BLENDWEIGHT;
};

struct VS_TEXTURE_INPUT
{
	float4 Pos		: POSITION;
	float3 Norm		: NORMAL;
	uint Bindex		: BLENDINDICE;
};

struct PS_BASIC_INPUT
{
	float4 Pos		: SV_POSITION;
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
// scalar
//-----------------------------------------------------------------------------------------
float g_fTime;
float g_fElapsedTime;

//-----------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------
float4x4 g_worldBoneMatrixs[256];

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
// VertexShader: VSSkinnedBasic
//-----------------------------------------------------------------------------------------
PS_BASIC_INPUT VSSkinnedBasic(VS_SKINNED_INPUT input)
{
	VS_BASIC_INPUT output;

	float3 pos = 0.0;
		float3 norm = 0.0;

		uint i = 0;
	uint w = 0;

	int index;
	for (index = 0; index < 4; ++index)
	{
		i = input.Bindexs[index];
		w = input.Bweight[index];

		// Bone's VertexBuffer에는 가중치를 주는 뼈의 index와 weight가 순서대로 저장.
		// 0이 등장했다는 것이 이후에도 없을 것을 확신.
		if (0 == w)
		{
			break;
		}

		pos += mul(input.Pos, (float4x3)g_worldBoneMatrixs[i]) * w;
		norm += mul(input.Norm, (float3x3)g_worldBoneMatrixs[i]) * w;
	}

	// index가 0라는 것은 아무도 가중치를 주지 않았다는 것이고 이는 뼈가 없음을 의미.
	if (0 == index)
	{
		pos = input.Pos.xyz;
		norm = input.Norm;
	}

	output.Pos = mul(float4(pos, 1.0f), g_WorldViewProj_Matrix);
	output.Norm = normalize(mul(norm, (float3x3)g_World_Matrix));
	output.Tex = input.Tex;

	return output;
}

//-----------------------------------------------------------------------------------------
// VertexShader: VSSkyBox
//-----------------------------------------------------------------------------------------
PS_BASIC_INPUT VSSkyBox(VS_BASIC_INPUT input)
{
	PS_BASIC_INPUT output;

	output.Pos = mul(input.Pos, (float3x4)g_WorldViewProj_Matrix);
	output.Pos.z = output.Pos.w;
	output.Norm = normalize(mul(input.Norm, (float3x3)g_World_Matrix));
	output.Tex = input.Tex;

	return output;
}

//-----------------------------------------------------------------------------------------
// VertexShader: VSGUI
//-----------------------------------------------------------------------------------------
PS_BASIC_INPUT VSGUI(VS_TEXTURE_INPUT input)
{
	VS_BASIC_INPUT output;

	output.Pos = mul(input.Pos, g_WorldViewProj_Matrix);
	output.Norm = normalize(mul(input.Norm, (float3x3)g_World_Matrix));

	switch (input.Bindex)
	{
	case 0:
		output.Tex = float2(g_TextureCapture.xy);
		break;
	case 1:
		output.Tex = float2(g_TextureCapture.yz);
		break;
	case 2:
		output.Tex = float2(g_TextureCapture.zw);
		break;
	case 3:
		output.Tex = float2(g_TextureCapture.wx);
		break;
	}

	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSkinnedBasic
//-----------------------------------------------------------------------------------------
float4 PSSkinnedBasic(PS_BASIC_INPUT input) : COLOR0
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSkyBox
//-----------------------------------------------------------------------------------------
float4 PSSkyBox(PS_BASIC_INPUT input) : COLOR0
{
	return tex2D(DiffuseSampler, input.Tex);
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSGUI
//-----------------------------------------------------------------------------------------
float4 PSGUI(PS_BASIC_INPUT input) : COLOR0
{
	float4 diffuse = tex2D(DiffuseSampler, input.Tex);

	float a = (atan2(0.5f - input.Tex.x, input.Tex.y - 0.5f) * 0.159154f) + 0.5f;
	if (a < g_fTime)
	{
		return diffuse;
	}
	else
	{
		diffuse.w *= 0.5f;
		return diffuse;
	}
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSGUIColor
//-----------------------------------------------------------------------------------------
float4 PSGUIColor(PS_BASIC_INPUT input) : COLOR0
{
	return g_Diffuse * g_fTime;
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
technique10 SkinnedBasic0
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, VSSkinnedBasic()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSSkinnedBasic()));

		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullBack);
	}
}

technique10 SkyBox0
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, VSSkyBox()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSSkyBox()));

		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullBack);
	}
}

technique10 GUI0
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, VSGUI()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSGUI()));

		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(UIBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		//SetRasterizerState(CullFront);
	}
}

technique10 GUIColor0
{
	pass p0
	{

		SetVertexShader(CompileShader(vs_4_0, VSGUI()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSGUIColor()));

		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(UIBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		//SetRasterizerState(CullFront);
	}
}