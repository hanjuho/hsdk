//--------------------------------------------------------------------------------------
// Exercise03.fx
// PIX Workshop GDC 2007
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "shader.fx"

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
technique10 Basic_0
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, VS_0()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_0()));

		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullBack);
	}
	pass p1
	{
		SetVertexShader(CompileShader(vs_4_0, VS_0()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_0()));

		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(UIBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullNone);
	}
}

technique10 Skinned_0
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, VS_Skinned()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_0()));

		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullBack);
	}
	pass p1
	{
		SetVertexShader(CompileShader(vs_4_0, VS_Skinned()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_0()));

		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(UIBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullNone);
	}
}

technique10 SkyBox_0
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, VS_SkyBox()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_SkyBox()));

		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullBack);
	}
}