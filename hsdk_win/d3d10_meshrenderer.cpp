#include <hsdk/win/frame/direct3d/d3d10_meshrenderer.h>
#include <d3d10effect.h>



using namespace hsdk;
using namespace direct3d;


//--------------------------------------------------------------------------------------
// Grobal renderer variable
//--------------------------------------------------------------------------------------

// 설명 :
ID3D10Device * g_refDevice_1;

// 설명 : 
AutoRelease<ID3D10Effect> g_D3D10Effect;

// 설명 : Get the effect variable handles
ID3D10EffectTechnique * g_Render_Technique;

// 설명 :
ID3D10EffectTechnique * g_SkinnedBasic_Technique;

// 설명 : 
ID3D10EffectTechnique * g_GUI_Technique;

// 설명 : 
ID3D10EffectTechnique * g_GUIColor_Technique;

//--------------------------------------------------------------------------------------
// Grobal matrix variable
//--------------------------------------------------------------------------------------

// 설명 : 
ID3D10EffectMatrixVariable * g_WorldViewProj_Matrix;

// 설명 : 
ID3D10EffectMatrixVariable * g_ViewProj_Matrix;

// 설명 : 
ID3D10EffectMatrixVariable * g_World_Matrix;

//--------------------------------------------------------------------------------------
// Grobal resource variable
//--------------------------------------------------------------------------------------

// 설명 : 
ID3D10EffectShaderResourceVariable * g_Diffuse_Texture;

//--------------------------------------------------------------------------------------
// Grobal vector variable
//--------------------------------------------------------------------------------------

// 설명 : 
ID3D10EffectVectorVariable * g_TextureCapture_Vector;

// 설명 : 
ID3D10EffectVectorVariable * g_Diffuse_Vector;

//--------------------------------------------------------------------------------------
// Grobal scalar variable
//--------------------------------------------------------------------------------------

// 설명 : 
ID3D10EffectScalarVariable * g_Time_Scalar;

// 설명 : 
ID3D10EffectScalarVariable * g_ElapsedTime_Scalar;

//--------------------------------------------------------------------------------------
// Grobal resource variable
//--------------------------------------------------------------------------------------

// 설명 :
unsigned int g_Sprite_Stride = sizeof(D3D10_TextureFormat);

// 설명 :
unsigned int g_Sprite_Offset = 0;

// 설명 : 
AutoRelease<ID3D10Buffer> g_Sprite_VBuffer;

// 설명 : 
AutoRelease<ID3D10Buffer> g_Sprite_IBuffer;

// 설명 : 
AutoRelease<ID3D10InputLayout> g_Basic_inputLayout;

// 설명 : 
AutoRelease<ID3D10InputLayout> g_Skinned_inputLayout;

// 설명 : 
AutoRelease<ID3D10InputLayout> g_Texture_inputLayout;

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

	AutoRelease<ID3D10Effect> effect;
	AutoRelease<ID3D10Blob> error;
	IF_FAILED(hr = D3DX10CreateEffectFromFile(
		L"meshrenderer.fx",
		nullptr,
		nullptr,
		"fx_4_0",
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY | D3D10_SHADER_DEBUG,
		0,
		_d3d10Device,
		nullptr,
		nullptr,
		&effect,
		&error,
		nullptr))
	{
		std::string err =
			std::string(LPCSTR(error->GetBufferPointer()), error->GetBufferSize());

		MessageBoxA(nullptr, err.c_str(), "warning", MB_OK);

		return hr;
	}

	//
	const D3D10_INPUT_ELEMENT_DESC basiclayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(D3D10_BasicFormat, pos), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(D3D10_BasicFormat, norm), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(D3D10_BasicFormat, tex), D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Get the effect variable handles
	g_Render_Technique =
		effect->GetTechniqueByName("Render0");

	D3D10_PASS_DESC PassDesc;
	g_Render_Technique->GetPassByIndex(0)->GetDesc(&PassDesc);

	AutoRelease<ID3D10InputLayout> Basic_inputLayout;
	IF_FAILED(hr = _d3d10Device->CreateInputLayout(
		basiclayout,
		sizeof(basiclayout) / sizeof(basiclayout[0]),
		PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize,
		&Basic_inputLayout))
	{
		return hr;
	}

	//
	const D3D10_INPUT_ELEMENT_DESC skinnedlayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(D3D10_SkinnedFormat, pos), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(D3D10_SkinnedFormat, norm), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(D3D10_SkinnedFormat, tex), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(D3D10_SkinnedFormat, color), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICE", 0, DXGI_FORMAT_R32_UINT, 0, offsetof(D3D10_SkinnedFormat, bindex), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(D3D10_SkinnedFormat, bweight), D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	g_SkinnedBasic_Technique =
		effect->GetTechniqueByName("SkinnedBasic0");

	g_SkinnedBasic_Technique->GetPassByIndex(0)->GetDesc(&PassDesc);

	AutoRelease<ID3D10InputLayout> Skinned_inputLayout;
	IF_FAILED(hr = _d3d10Device->CreateInputLayout(
		skinnedlayout,
		sizeof(skinnedlayout) / sizeof(skinnedlayout[0]),
		PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize,
		&Skinned_inputLayout))
	{
		return hr;
	}

	//
	const D3D10_INPUT_ELEMENT_DESC texturelayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(D3D10_TextureFormat, pos), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(D3D10_TextureFormat, norm), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICE", 0, DXGI_FORMAT_R32_UINT, 0, offsetof(D3D10_TextureFormat, bindex), D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Get the effect variable handles
	g_GUI_Technique =
		effect->GetTechniqueByName("GUI0");

	g_GUIColor_Technique =
		effect->GetTechniqueByName("GUIColor0");

	g_GUI_Technique->GetPassByIndex(0)->GetDesc(&PassDesc);

	AutoRelease<ID3D10InputLayout> Texture_inputLayout;
	IF_FAILED(hr = _d3d10Device->CreateInputLayout(
		texturelayout,
		sizeof(texturelayout) / sizeof(texturelayout[0]),
		PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize,
		&Texture_inputLayout))
	{
		return hr;
	}

	g_WorldViewProj_Matrix =
		effect->GetVariableByName("g_WorldViewProj_Matrix")->AsMatrix();

	g_ViewProj_Matrix =
		effect->GetVariableByName("g_ViewProj_Matrix")->AsMatrix();

	g_World_Matrix =
		effect->GetVariableByName("g_World_Matrix")->AsMatrix();

	g_Diffuse_Texture =
		effect->GetVariableByName("g_Diffuse_Texture")->AsShaderResource();

	g_TextureCapture_Vector =
		effect->GetVariableByName("g_TextureCapture")->AsVector();

	g_Diffuse_Vector =
		effect->GetVariableByName("g_Diffuse")->AsVector();

	g_Time_Scalar =
		effect->GetVariableByName("g_fTime")->AsScalar();

	g_ElapsedTime_Scalar =
		effect->GetVariableByName("g_fElapsedTime")->AsScalar();

	g_D3D10Effect = effect;
	g_Basic_inputLayout = Basic_inputLayout;
	g_Skinned_inputLayout = Skinned_inputLayout;
	g_Texture_inputLayout = Texture_inputLayout;
	g_refDevice_1 = _d3d10Device;

	// Build box
	D3D10_TextureFormat vBox[] = {
		// front
		{ D3DXVECTOR3(-1.0f, -1.0f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), 1 },
		{ D3DXVECTOR3(1.0f, -1.0f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), 2 },
		{ D3DXVECTOR3(1.0f, 1.0f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), 3 },
		{ D3DXVECTOR3(-1.0f, 1.0f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), 0 } };

	// Vertex Buffer
	D3D10_BUFFER_DESC vBufferDesc;
	vBufferDesc.ByteWidth = sizeof(vBox);
	vBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vBufferDesc.CPUAccessFlags = 0;
	vBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vBox;

	AutoRelease<ID3D10Buffer> vbuffer;
	IF_FAILED(hr = g_refDevice_1->CreateBuffer(
		&vBufferDesc,
		&vinitData,
		&vbuffer))
	{
		return hr;
	}

	unsigned short iBox[] = {
		// front
		3, 1, 0,
		2, 1, 3 };

	// index Buffer
	D3D10_BUFFER_DESC iBufferDesc;
	iBufferDesc.ByteWidth = sizeof(iBox);
	iBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	iBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	iBufferDesc.CPUAccessFlags = 0;
	iBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = iBox;

	AutoRelease<ID3D10Buffer>ibuffer;
	IF_FAILED(hr = g_refDevice_1->CreateBuffer(
		&iBufferDesc,
		&iinitData,
		&ibuffer))
	{
		return hr;
	}

	g_Sprite_VBuffer = vbuffer;
	g_Sprite_IBuffer = ibuffer;

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_MeshRenderer, void, destroy)(
	/* [x] */ void)
{
	g_D3D10Effect.~AutoRelease();
	g_Basic_inputLayout.~AutoRelease();
	g_Texture_inputLayout.~AutoRelease();
	g_Skinned_inputLayout.~AutoRelease();
	g_Sprite_VBuffer.~AutoRelease();
	g_Sprite_IBuffer.~AutoRelease();

	g_Render_Technique = nullptr;
	g_GUI_Technique = nullptr;
	g_GUIColor_Technique = nullptr;
	g_WorldViewProj_Matrix = nullptr;
	g_ViewProj_Matrix = nullptr;
	g_World_Matrix = nullptr;
	g_Diffuse_Texture = nullptr;
	g_Diffuse_Vector = nullptr;
	g_Time_Scalar = nullptr;
	g_ElapsedTime_Scalar = nullptr;
	g_refDevice_1 = nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_MeshRenderer, void, render_Skinned)(
	/* [r] */ D3DXMATRIX & _world,
	/* [r] */ const D3D10_Mesh & _mesh)
{
	g_WorldViewProj_Matrix->SetMatrix((float *)(_world));
	g_World_Matrix->SetMatrix((float *)(_world));

	g_refDevice_1->IASetInputLayout(g_Skinned_inputLayout);

	const unsigned int numMeshs =
		_mesh.get_NumMeshes();

	for (unsigned int imesh = 0; imesh < numMeshs; ++imesh)
	{
		const D3D10MY_MESH & mesh =
			_mesh.get_Mesh(imesh);

		g_refDevice_1->IASetVertexBuffers(
			0, mesh.vertexbuffers.size(),
			&mesh.vertexbuffers[0],
			&mesh.vertexbuffers_Strides[0],
			&mesh.vertexbuffers_Offsets[0]);

		g_refDevice_1->IASetIndexBuffer(
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

			g_refDevice_1->IASetPrimitiveTopology(desc.primitiveType);

			g_Diffuse_Texture->SetResource(material.diffuseRV);

			g_SkinnedBasic_Technique->GetPassByIndex(0)->Apply(0);

			g_refDevice_1->DrawIndexed(
				desc.indexCount,
				desc.indexStart,
				desc.vertexbufferStart);
		}
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_MeshRenderer, void, render_SkyBox)(
	/* [r] */ D3DXMATRIX & _world,
	/* [r] */ const D3D10_Mesh & _mesh)
{
	g_WorldViewProj_Matrix->SetMatrix((float *)(_world));
	g_World_Matrix->SetMatrix((float *)(_world));

	g_refDevice_1->IASetInputLayout(g_Basic_inputLayout);

	const unsigned int numMeshs =
		_mesh.get_NumMeshes();

	for (unsigned int imesh = 0; imesh < numMeshs; ++imesh)
	{
		const D3D10MY_MESH & mesh =
			_mesh.get_Mesh(imesh);

		g_refDevice_1->IASetVertexBuffers(
			0, mesh.vertexbuffers.size(),
			&mesh.vertexbuffers[0],
			&mesh.vertexbuffers_Strides[0],
			&mesh.vertexbuffers_Offsets[0]);

		g_refDevice_1->IASetIndexBuffer(
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

			g_refDevice_1->IASetPrimitiveTopology(desc.primitiveType);

			g_Diffuse_Texture->SetResource(material.diffuseRV);

			g_Render_Technique->GetPassByIndex(0)->Apply(0);

			g_refDevice_1->DrawIndexed(
				desc.indexCount,
				desc.indexStart,
				desc.vertexbufferStart);
		}
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_MeshRenderer, void, render_UIRectangle)(
	/* [r] */ D3DXMATRIX & _world,
	/* [r] */ D3DXVECTOR4 & _color,
	/* [r] */ float _persent)
{
	g_WorldViewProj_Matrix->SetMatrix((float *)(_world));
	g_World_Matrix->SetMatrix((float *)(_world));
	g_Diffuse_Vector->SetFloatVector(_color);
	g_Time_Scalar->SetFloat(_persent);

	g_refDevice_1->IASetInputLayout(
		g_Texture_inputLayout);

	g_refDevice_1->IASetVertexBuffers(
		0, 1,
		&g_Sprite_VBuffer,
		&g_Sprite_Stride,
		&g_Sprite_Offset);

	g_refDevice_1->IASetIndexBuffer(
		g_Sprite_IBuffer,
		DXGI_FORMAT_R16_UINT, 0);

	g_refDevice_1->IASetPrimitiveTopology(
		D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_GUIColor_Technique->GetPassByIndex(0)->Apply(0);

	g_refDevice_1->DrawIndexed(6, 0, 0);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_MeshRenderer, void, render_UITexture)(
	/* [r] */ D3DXMATRIX & _world,
	/* [r] */ ID3D10ShaderResourceView * _texture,
	/* [r] */ D3DXVECTOR4 & _clip,
	/* [r] */ float _persent)
{
	g_WorldViewProj_Matrix->SetMatrix((float *)(_world));
	g_World_Matrix->SetMatrix((float *)(_world));
	g_TextureCapture_Vector->SetFloatVector(_clip);
	g_Time_Scalar->SetFloat(_persent);

	g_refDevice_1->IASetInputLayout(
		g_Texture_inputLayout);

	g_refDevice_1->IASetVertexBuffers(
		0, 1,
		&g_Sprite_VBuffer,
		&g_Sprite_Stride,
		&g_Sprite_Offset);

	g_refDevice_1->IASetIndexBuffer(
		g_Sprite_IBuffer,
		DXGI_FORMAT_R16_UINT, 0);

	g_refDevice_1->IASetPrimitiveTopology(
		D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_Diffuse_Texture->SetResource(_texture);

	g_GUI_Technique->GetPassByIndex(0)->Apply(0);

	g_refDevice_1->DrawIndexed(6, 0, 0);
}

//--------------------------------------------------------------------------------------
// open variable 
//--------------------------------------------------------------------------------------

// 설명 : 
D3D10_MeshRenderer hsdk::direct3d::g_D3D10_MeshRenderer;

// 설명 :
D3DXMATRIX hsdk::direct3d::g_D3D10_ViewMatrix = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0, };

// 설명 :
D3DXMATRIX hsdk::direct3d::g_D3D10_ProjMatrix = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0, };