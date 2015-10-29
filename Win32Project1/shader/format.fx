//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
struct VS_BASIC_INPUT
{
	float4 Pos		: POSITION;
	float3 Nor		: NORMAL;
	float2 Tex		: TEXCOORD0;
};

struct VS_SKINNED_INPUT
{
	float4	Pos		: POSITION;
	float3	Nor		: NORMAL;
	float2	Tex		: TEXCOORD0;
	uint4	Bindexs	: BLENDINDICES;
	float4	Bweight	: BLENDWEIGHT;
};

struct PS_BASIC_INPUT
{
	float4 Pos		: SV_POSITION;
	float3 Nor		: TEXCOORD0;
	float2 Tex		: TEXCOORD1;
};

struct PS_SKYBOX_INPUT
{
	float4 Pos		: SV_POSITION;
	float3 Tex		: TEXCOORD0;
};