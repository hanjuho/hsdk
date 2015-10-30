//-----------------------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------------------
matrix   g_mWorld;                  // World matrix for object
matrix   g_mView;                   // View matrix for object
matrix   g_mProj;                   // Projection matrix for object
matrix   g_mWorldView;              // World * View matrix
matrix   g_mWorldViewProj;          // World * View * Projection matrix

matrix   g_mTexture;

//-----------------------------------------------------------------------------------------
// Color
//-----------------------------------------------------------------------------------------
float4 g_vDiffuse;
float4 g_vAmbient;
float4 g_vSpecular;
float4 g_vEmissive;
float g_shininess;

//-----------------------------------------------------------------------------------------
// light
//-----------------------------------------------------------------------------------------
float3 g_vLightDir;

//-----------------------------------------------------------------------------------------
// Texture
//-----------------------------------------------------------------------------------------

// 설명 : 
Texture2D g_texDiffuse;

// 설명 : 
sampler2D DiffuseSampler = sampler_state
{
	Texture = (g_texDiffuse);
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

// 설명 : 
Texture2D g_texNormal;

// 설명 : 
sampler2D NormalSampler = sampler_state
{
	Texture = (g_texNormal);
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

// 설명 : 
Texture2D g_texSpecular;

// 설명 : 
sampler2D SpecularSampler = sampler_state
{
	Texture = (g_texSpecular);
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

//-----------------------------------------------------------------------------------------
// SkyBox
//-----------------------------------------------------------------------------------------

// 설명 : 
matrix g_mSkyBox;

// 설명 : 
TextureCube g_texSkyBox;

// 설명 : 
samplerCUBE SkyBoxSampler = sampler_state
{
	Texture = (g_texSkyBox);
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

//-----------------------------------------------------------------------------------------
// Bone
//-----------------------------------------------------------------------------------------
matrix g_mBone[80];

//-----------------------------------------------------------------------------------------
// flag, parameter
//-----------------------------------------------------------------------------------------
uint g_vsFlag;
float g_vsTime;

// vs flag
#define VS_CALLFUNCTION_0 1024

uint g_psFlag;
float g_psTime;

// ps flag
#define PS_TEXTURE_0 1	// diffuse
#define PS_TEXTURE_1 2	// normal
#define PS_TEXTURE_2 4	// specular
#define PS_TEXTURE_3 8	// 미정

#define PS_MARERIAL_0 16	// diffuse
#define PS_MARERIAL_1 32	// ambient
#define PS_MARERIAL_2 64	// specular
#define PS_MARERIAL_3 128	// emissive

#define PS_LIGHT 256

#define PS_CALLFUNCTION_0 4096

#define PS_TEXMATRIX_0 65536