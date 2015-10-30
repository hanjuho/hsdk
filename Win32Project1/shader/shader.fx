#include "format.fx"
#include "grobal.fx"
#include "function.fx"

//-----------------------------------------------------------------------------------------
// VertexShader:
//-----------------------------------------------------------------------------------------
PS_BASIC_INPUT VS_0(VS_BASIC_INPUT input)
{
	PS_BASIC_INPUT output;
	
	output.Pos = mul(input.Pos, g_mWorldViewProj);
	output.Nor = normalize(mul(input.Nor, (float3x3)g_mWorld));
	output.Tex = input.Tex;

	if (g_vsFlag & VS_CALLFUNCTION_0)
	{
		output.Pos.xyz = computeDestroy(output.Pos.xyz, 
			normalize(float3(input.Pos.xy, 0.0f)), g_vsTime);
	}

	return output;
}

//-----------------------------------------------------------------------------------------
// VertexShader:
//-----------------------------------------------------------------------------------------
PS_BASIC_INPUT VS_Skinned(VS_SKINNED_INPUT input)
{
	PS_BASIC_INPUT output;

	// 변수 선언
	float3 pos = 0.0f; float3 nor = 0.0f; uint i; float w; uint index;

	for (index = 0; index < 4; ++index)
	{
		i = input.Bindexs[index];
		w = input.Bweight[index];

		// Bone's VertexBuffer에는 가중치를 주는 뼈의 index와 weight가 순서대로 저장.
		// 0이 등장했다는 것이 이후에도 없을 것을 확신.
		if (w == 0)
		{
			break;
		}

		pos += mul(input.Pos, (float4x3)g_mBone[i]) * w;
		nor += mul(input.Nor, (float3x3)g_mBone[i]) * w;
	}

	// index가 0라는 것은 아무도 가중치를 주지 않았다는 것이고 이는 뼈가 없음을 의미.
	if (0 == index)
	{
		pos = input.Pos.xyz;
		nor = input.Nor;
	}

	output.Pos = mul(float4(pos, 1.0f), g_mWorldViewProj);
	output.Nor = normalize(mul(nor, (float3x3)g_mWorld));
	output.Tex = input.Tex;

	if (g_vsFlag & VS_CALLFUNCTION_0)
	{
		output.Pos.xyz = computeDestroy(output.Pos.xyz, output.Nor, g_vsTime);
	}

	return output;
}

//-----------------------------------------------------------------------------------------
// VertexShader:
//-----------------------------------------------------------------------------------------
PS_SKYBOX_INPUT VS_SkyBox(float4 Pos : POSITION)
{
	PS_SKYBOX_INPUT output;

	output.Pos = mul(Pos, g_mWorldViewProj);
	//output.Pos.z = output.Pos.w;

	output.Tex = Pos.xyz;

	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader:
//-----------------------------------------------------------------------------------------
float4 PS_0(PS_BASIC_INPUT input) : COLOR0
{
	float4 textureColor = 1;
	float4 diffuse = 1;

	// color
	if (g_psFlag & PS_MARERIAL_0)
	{
		diffuse = g_vDiffuse;
	}

	//
	if (g_psFlag & PS_CALLFUNCTION_0)
	{
		if (computeCircleAlpha(input.Tex, g_psTime))
		{
			diffuse.w *= 0.5f;
		}
	}

	if (g_psFlag & PS_TEXMATRIX_0)
	{
		input.Tex = mul(float4(input.Tex, 0.0f, 1.0f), g_mTexture).xy;
	}

	// texture color
	if (g_psFlag & PS_TEXTURE_0)
	{
		//
		textureColor = tex2D(DiffuseSampler, input.Tex);

		if (g_psFlag & PS_TEXTURE_1)
		{
			// Sample the pixel in the bump map.
			float4 bumpMap = tex2D(NormalSampler, input.Tex);;

				// Expand the range of the normal value from (0, +1) to (-1, +1).
				bumpMap = (bumpMap * 2.0f) - 1.0f;

			// Calculate the normal from the data in the bump map.
			float3 bumpNormal = input.Nor + bumpMap.xyz;

				// Normalize the resulting bump normal.
				bumpNormal = normalize(bumpNormal);

			//
			diffuse *= length(bumpNormal);
		}
	}

	return diffuse * textureColor;
}

//-----------------------------------------------------------------------------------------
// PixelShader:
//-----------------------------------------------------------------------------------------
float4 PS_SkyBox(PS_SKYBOX_INPUT input) : COLOR0
{
	float4 textureColor = 1;
	float4 diffuse = 1;

	// color
	if (g_psFlag & PS_MARERIAL_0)
	{
		diffuse = g_vDiffuse;
	}

	//
	if (g_psFlag & PS_CALLFUNCTION_0)
	{
		if (computeCircleAlpha(input.Tex.xy, g_psTime))
		{
			diffuse.w *= 0.5f;
		}
	}

	if (g_psFlag & PS_TEXMATRIX_0)
	{
		input.Tex = mul(float4(input.Tex, 1.0f), g_mTexture).xyz;
	}

	// texture color
	if (g_psFlag & PS_TEXTURE_0)
	{
		textureColor = texCUBE(SkyBoxSampler, input.Tex);
	}

	return diffuse * textureColor;
}