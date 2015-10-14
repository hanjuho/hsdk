#include <hsdk/win/frame/direct3d/d3d10_meshrenderer.h>
#include <d3d10effect.h>



using namespace hsdk;
using namespace direct3d;


//--------------------------------------------------------------------------------------
// Grobal renderer variable
//--------------------------------------------------------------------------------------

// 설명 :
ID3D10Device * g_refMeshRenderer_Device;

// 설명 : 
AutoRelease<ID3D10Effect> g_MeshRenderer_D3D10Effect;

// 설명 : Get the effect variable handles
ID3D10EffectTechnique * g_MeshRenderer_RenderSky_Technique;

//--------------------------------------------------------------------------------------
// Grobal matrix variable
//--------------------------------------------------------------------------------------

// 설명 : 
ID3D10EffectMatrixVariable * g_MeshRenderer_WorldViewProj_Matrix;

// 설명 : 
ID3D10EffectMatrixVariable * g_MeshRenderer_ViewProj_Matrix;

// 설명 : 
ID3D10EffectMatrixVariable * g_MeshRenderer_World_Matrix;

//--------------------------------------------------------------------------------------
// Grobal vector variable
//--------------------------------------------------------------------------------------

// 설명 : 
ID3D10EffectVectorVariable * g_MeshRenderer_WorldLight_Vector;

// 설명 : 
ID3D10EffectVectorVariable * g_MeshRenderer_Color_Vector;

// 설명 : 
ID3D10EffectVectorVariable * g_MeshRenderer_EyePt_Vector;

//--------------------------------------------------------------------------------------
// Grobal scalar variable
//--------------------------------------------------------------------------------------

// 설명 : 
ID3D10EffectScalarVariable * g_MeshRenderer_Time_Scalar;

// 설명 : 
ID3D10EffectScalarVariable * g_MeshRenderer_ElapsedTime_Scalar;

//--------------------------------------------------------------------------------------
// Grobal resource variable
//--------------------------------------------------------------------------------------

// 설명 : 
ID3D10EffectShaderResourceVariable * g_MeshRenderer_Diffuse_Texture;

//--------------------------------------------------------------------------------------
// Grobal resource variable
//--------------------------------------------------------------------------------------

// 설명 : 
AutoRelease<ID3D10InputLayout> g_MeshRenderer_Basic_inputLayout;

//--------------------------------------------------------------------------------------
// Grobal string variable
//--------------------------------------------------------------------------------------

// 설명 : 
wchar_t g_szEffect[MAX_PATH];

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_MeshRenderer, initialize)(
	/* [r] */ ID3D10Device * _d3d10Device)
{
	WIN32_FIND_DATA FindData;
	HANDLE hFile = FindFirstFile(g_szEffect, &FindData);
	FindClose(hFile);

	FindData.ftLastWriteTime;

	// Read the D3DX effect file
	wchar_t str[MAX_PATH];
	unsigned long shaderFlags = D3DXFX_NOT_CLONEABLE;
	shaderFlags |= D3DXSHADER_DEBUG;

	// V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Exercise03.fx"));

	HRESULT hr = S_OK;
	IF_FAILED(hr = D3DX10CreateEffectFromFile(
		str,
		nullptr,
		nullptr,
		"fx_4_0",
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY | D3D10_SHADER_DEBUG,
		0,
		_d3d10Device,
		nullptr,
		nullptr,
		&g_MeshRenderer_D3D10Effect,
		nullptr,
		nullptr))
	{
		return hr;
	}

	// Get the effect variable handles
	g_MeshRenderer_RenderSky_Technique =
		g_MeshRenderer_D3D10Effect->GetTechniqueByName("RenderSky10");

	g_MeshRenderer_WorldViewProj_Matrix = g_MeshRenderer_D3D10Effect->GetVariableByName("g_WorldViewProj_Matrix")->AsMatrix();
	g_MeshRenderer_ViewProj_Matrix = g_MeshRenderer_D3D10Effect->GetVariableByName("g_ViewProj_Matrix")->AsMatrix();
	g_MeshRenderer_World_Matrix = g_MeshRenderer_D3D10Effect->GetVariableByName("g_World_Matrix")->AsMatrix();

	g_MeshRenderer_WorldLight_Vector = g_MeshRenderer_D3D10Effect->GetVariableByName("g_WorldLight_Vector")->AsVector();
	g_MeshRenderer_Color_Vector = g_MeshRenderer_D3D10Effect->GetVariableByName("g_vColor")->AsVector();
	g_MeshRenderer_EyePt_Vector = g_MeshRenderer_D3D10Effect->GetVariableByName("g_vEyePt")->AsVector();

	g_MeshRenderer_Time_Scalar = g_MeshRenderer_D3D10Effect->GetVariableByName("g_fTime")->AsScalar();
	g_MeshRenderer_ElapsedTime_Scalar = g_MeshRenderer_D3D10Effect->GetVariableByName("g_fElapsedTime")->AsScalar();

	g_MeshRenderer_Diffuse_Texture = g_MeshRenderer_D3D10Effect->GetVariableByName("g_Diffuse_Texture")->AsShaderResource();

	// Create a Vertex Decl for the terrain and basic meshes
	const D3D10_INPUT_ELEMENT_DESC basiclayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D10_PASS_DESC PassDesc;
	g_MeshRenderer_RenderSky_Technique->GetPassByIndex(0)->GetDesc(&PassDesc);
	IF_FAILED(hr = _d3d10Device->CreateInputLayout(
		basiclayout,
		sizeof(basiclayout) / sizeof(basiclayout[0]),
		PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize,
		&g_MeshRenderer_Basic_inputLayout))
	{
		return hr;
	}

	g_refMeshRenderer_Device = _d3d10Device;

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_MeshRenderer, void, destroy)(
	/* [x] */ void)
{
	g_MeshRenderer_D3D10Effect.~AutoRelease();
	g_MeshRenderer_Basic_inputLayout.~AutoRelease();
	g_MeshRenderer_RenderSky_Technique = nullptr;
	g_MeshRenderer_WorldViewProj_Matrix = nullptr;
	g_MeshRenderer_ViewProj_Matrix = nullptr;
	g_MeshRenderer_World_Matrix = nullptr;
	g_MeshRenderer_WorldLight_Vector = nullptr;
	g_MeshRenderer_Color_Vector = nullptr;
	g_MeshRenderer_EyePt_Vector = nullptr;
	g_MeshRenderer_Time_Scalar = nullptr;
	g_MeshRenderer_ElapsedTime_Scalar = nullptr;
	g_MeshRenderer_Diffuse_Texture = nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_MeshRenderer, void, render_SkyBox)(
	/* [r] */ const D3DXMATRIX & _world,
	/* [r] */ const D3D10_Mesh & _mesh)
{
	g_refMeshRenderer_Device->IASetInputLayout(g_MeshRenderer_Basic_inputLayout);

	const unsigned int numMeshs =
		_mesh.get_NumMeshes();

	for (unsigned int imesh = 0; imesh < numMeshs; ++imesh)
	{
		const D3D10MY_MESH & mesh =
			_mesh.get_Mesh(imesh);

		g_refMeshRenderer_Device->IASetVertexBuffers(
			0, mesh.vertexbuffers.size(),
			&mesh.vertexbuffers[0],
			&mesh.vertexbuffers_Strides[0],
			&mesh.vertexbuffers_Offsets[0]);

		g_refMeshRenderer_Device->IASetIndexBuffer(
			mesh.indexbuffer.indexbuffer,
			mesh.indexbuffer.indexType, 0);

		const unsigned int numRenders =
			mesh.render_Descs.size();

		for (unsigned int irender = 0; irender < numRenders; ++irender)
		{
			const D3D10MY_RENDER_DESC & desc =
				mesh.render_Descs[irender];

			const D3D10MY_MATERIAL & material =
				_mesh.get_Material(desc.material_id);

			g_refMeshRenderer_Device->IASetPrimitiveTopology(desc.primitiveType);

			g_MeshRenderer_Diffuse_Texture->SetResource(material.diffuseRV);

			g_MeshRenderer_RenderSky_Technique->GetPassByIndex(0)->Apply(0);

			g_refMeshRenderer_Device->DrawIndexed(
				desc.indexCount,
				desc.indexStart,
				desc.vertexbufferStart);
		}
	}
}