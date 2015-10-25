#include <hsdk/win/direct3d/d3d10_renderer.h>
#include <hsdk/win/framework.h>
#include <d3d10effect.h>



using namespace hsdk::direct3d;


//--------------------------------------------------------------------------------------
// Grobal renderer variable
//--------------------------------------------------------------------------------------

// 설명 :
ID3D10Device * g_refDevice_1;

// 설명 : 
hsdk::AutoRelease<ID3D10Effect> g_D3D10Effect;

// 설명 :
ID3D10EffectTechnique * g_SkinnedBasic_Technique;

// 설명 : Get the effect variable handles
ID3D10EffectTechnique * g_SkyBox_Technique;

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

// 설명 : 
ID3D10EffectMatrixVariable * g_TexCoord_Matrix;

// 설명 : 
ID3D10EffectMatrixVariable * g_Bone_World_Matrixs;

//--------------------------------------------------------------------------------------
// Grobal resource variable
//--------------------------------------------------------------------------------------

// 설명 : 
ID3D10EffectShaderResourceVariable * g_Diffuse_Texture;

// 설명 : 
ID3D10EffectShaderResourceVariable * g_SkyBox_Texture;

//--------------------------------------------------------------------------------------
// Grobal vector variable
//--------------------------------------------------------------------------------------

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
unsigned int g_Sprite_Stride = sizeof(D3D10_BasicFormat);

// 설명 :
unsigned int g_Sprite_Offset = 0;

// 설명 : 
hsdk::AutoRelease<ID3D10Buffer> g_Sprite_VBuffer;

// 설명 : 
hsdk::AutoRelease<ID3D10Buffer> g_Sprite_IBuffer;

// 설명 : 
hsdk::AutoRelease<ID3D10InputLayout> g_Basic_inputLayout;

// 설명 : 
hsdk::AutoRelease<ID3D10InputLayout> g_Skinned_inputLayout;

// 설명 : 
hsdk::AutoRelease<ID3D10InputLayout> g_Skybox_inputLayout;

//--------------------------------------------------------------------------------------
// Grobal string variable
//--------------------------------------------------------------------------------------

// 설명 : 
wchar_t g_szEffect[MAX_PATH];

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Renderer, initialize_Shader)(
	_X_ const wchar_t * _directory)
{
	HRESULT hr = E_FAIL;

	AutoRelease<ID3D10Effect> effect;
	AutoRelease<ID3D10Blob> error;

	IF_FAILED(hr = D3DX10CreateEffectFromFile(
		_directory,
		nullptr,
		nullptr,
		"fx_4_0",
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY | D3D10_SHADER_DEBUG,
		0,
		framework::g_Framework_Device.d3d10Device,
		nullptr,
		nullptr,
		&effect,
		&error,
		nullptr))
	{
		if (error)
		{
			std::string err = std::string(LPCSTR(error->GetBufferPointer()), error->GetBufferSize());
			MessageBoxA(nullptr, err.c_str(), "warning", MB_OK);
		}
		else
		{
			MessageBoxA(nullptr, "not found FX file", "warning", MB_OK);
		}

		return hr;
	}

	g_SkinnedBasic_Technique =
		effect->GetTechniqueByName("SkinnedBasic0");

	g_SkyBox_Technique =
		effect->GetTechniqueByName("SkyBox0");

	g_GUI_Technique =
		effect->GetTechniqueByName("GUI0");

	g_GUIColor_Technique =
		effect->GetTechniqueByName("GUIColor0");
	
	g_WorldViewProj_Matrix =
		effect->GetVariableByName("g_WorldViewProj_Matrix")->AsMatrix();

	g_ViewProj_Matrix =
		effect->GetVariableByName("g_ViewProj_Matrix")->AsMatrix();

	g_World_Matrix =
		effect->GetVariableByName("g_World_Matrix")->AsMatrix();

	g_TexCoord_Matrix =
		effect->GetVariableByName("g_TexCoord_Matrix")->AsMatrix();

	g_Bone_World_Matrixs =
		effect->GetVariableByName("g_Bone_World_Matrixs")->AsMatrix();

	g_Diffuse_Texture =
		effect->GetVariableByName("g_Diffuse_Texture")->AsShaderResource();

	g_SkyBox_Texture =
		effect->GetVariableByName("g_SkyBox_Texture")->AsShaderResource();
	
	g_Diffuse_Vector =
		effect->GetVariableByName("g_Diffuse")->AsVector();

	g_Time_Scalar =
		effect->GetVariableByName("g_fTime")->AsScalar();

	g_ElapsedTime_Scalar =
		effect->GetVariableByName("g_fElapsedTime")->AsScalar();
	
	g_D3D10Effect = effect;

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Renderer, initialize_Layout)(
	_X_ void)
{
	HRESULT hr = E_FAIL;

	AutoRelease<ID3D10InputLayout> Basic_inputLayout;
	AutoRelease<ID3D10InputLayout> Skinned_inputLayout;
	AutoRelease<ID3D10InputLayout> Skybox_inputLayout;

	//
	const D3D10_INPUT_ELEMENT_DESC basiclayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(D3D10_BasicFormat, pos), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(D3D10_BasicFormat, norm), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(D3D10_BasicFormat, tex), D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	//
	const D3D10_INPUT_ELEMENT_DESC skinnedlayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(D3D10_SkinnedFormat, pos), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(D3D10_SkinnedFormat, norm), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(D3D10_SkinnedFormat, tex), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(D3D10_SkinnedFormat, color), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32_UINT, 0, offsetof(D3D10_SkinnedFormat, bindexs), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(D3D10_SkinnedFormat, bweight), D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};
	
	//
	const D3D10_INPUT_ELEMENT_DESC skyboxlayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(D3D10_SkyBoxFormat, pos), D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D10_PASS_DESC PassDesc;
	
	g_GUI_Technique->GetPassByIndex(0)->GetDesc(&PassDesc);
	IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateInputLayout(
		basiclayout,
		sizeof(basiclayout) / sizeof(basiclayout[0]),
		PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize,
		&Basic_inputLayout))
	{
		return hr;
	}

	g_SkinnedBasic_Technique->GetPassByIndex(0)->GetDesc(&PassDesc);
	IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateInputLayout(
		skinnedlayout,
		sizeof(skinnedlayout) / sizeof(skinnedlayout[0]),
		PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize,
		&Skinned_inputLayout))
	{
		return hr;
	}

	g_SkyBox_Technique->GetPassByIndex(0)->GetDesc(&PassDesc);
	IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateInputLayout(
		skyboxlayout,
		sizeof(skyboxlayout) / sizeof(skyboxlayout[0]),
		PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize,
		&Skybox_inputLayout))
	{
		return hr;
	}

	g_Basic_inputLayout = Basic_inputLayout;
	g_Skinned_inputLayout = Skinned_inputLayout;
	g_Skybox_inputLayout = Skybox_inputLayout;

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Renderer, initialize)(
	_X_ const wchar_t * _shaderFilePath)
{
	HRESULT hr = E_FAIL;

	std::wstring directory = _shaderFilePath;
	directory.append(L"meshrenderer.fx");

	IF_FAILED(hr = initialize_Shader(directory.c_str()))
	{
		return hr;
	}

	IF_FAILED(hr = initialize_Layout())
	{
		return hr;
	}
	
	// build plane
	D3D10_BasicFormat vBox[] = {
		{ D3DXVECTOR3(-1.0f, -1.0f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR3(1.0f, -1.0f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 1.0f) },
		{ D3DXVECTOR3(1.0f, 1.0f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR3(-1.0f, 1.0f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 0.0f) } };

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
	IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateBuffer(
		&vBufferDesc,
		&vinitData,
		&vbuffer))
	{
		return hr;
	}

	unsigned short iBox[] = {
		0, 2, 3,
		0, 1, 2};

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
	IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateBuffer(
		&iBufferDesc,
		&iinitData,
		&ibuffer))
	{
		return hr;
	}

	g_Sprite_VBuffer = vbuffer;
	g_Sprite_IBuffer = ibuffer;

	g_refDevice_1 = framework::g_Framework_Device.d3d10Device;

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, destroy)(
	_X_ void)
{
	g_D3D10Effect.~AutoRelease();
	g_Basic_inputLayout.~AutoRelease();
	g_Skinned_inputLayout.~AutoRelease();
	g_Skybox_inputLayout.~AutoRelease();
	g_Sprite_VBuffer.~AutoRelease();
	g_Sprite_IBuffer.~AutoRelease();

	g_SkinnedBasic_Technique = nullptr;
	g_SkyBox_Technique = nullptr;
	g_GUI_Technique = nullptr;
	g_GUIColor_Technique = nullptr;
	g_WorldViewProj_Matrix = nullptr;
	g_ViewProj_Matrix = nullptr;
	g_World_Matrix = nullptr;
	g_TexCoord_Matrix = nullptr;
	g_Bone_World_Matrixs = nullptr;
	g_Diffuse_Texture = nullptr;
	g_Diffuse_Vector = nullptr;
	g_Time_Scalar = nullptr;
	g_ElapsedTime_Scalar = nullptr;
	g_refDevice_1 = nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, render_Skinned)(
	_In_ D3DXMATRIX & _world,
	_In_ const D3D10_Mesh & _mesh,
	_In_ D3DXMATRIX * _boneMatrixs,
	_In_ unsigned int _matrixSize)
{
	g_Bone_World_Matrixs->SetMatrixArray((float *)_boneMatrixs, 0, _matrixSize);
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
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, render_SkyBox)(
	_In_ D3DXMATRIX & _world,
	_In_ const D3D10_Mesh & _mesh)
{
	g_WorldViewProj_Matrix->SetMatrix((float *)(_world));
	g_World_Matrix->SetMatrix((float *)(_world));

	g_refDevice_1->IASetInputLayout(g_Skybox_inputLayout);

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

			g_refDevice_1->IASetPrimitiveTopology(desc.primitiveType);

			g_SkyBox_Texture->SetResource(
				_mesh.get_Material(desc.material_id).diffuseRV);

			g_SkyBox_Technique->GetPassByIndex(0)->Apply(0);

			g_refDevice_1->DrawIndexed(
				desc.indexCount,
				desc.indexStart,
				desc.vertexbufferStart);
		}
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, render_UIRectangle)(
	_In_ D3DXMATRIX & _world,
	_In_ D3DXVECTOR4 & _color,
	_In_ float _persent)
{
	g_WorldViewProj_Matrix->SetMatrix((float *)(_world));
	g_World_Matrix->SetMatrix((float *)(_world));

	g_Diffuse_Vector->SetFloatVector(_color);

	g_Time_Scalar->SetFloat(_persent);

	g_refDevice_1->IASetInputLayout(
		g_Basic_inputLayout);

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
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, render_UITexture)(
	_In_ D3DXMATRIX & _world,
	_In_ ID3D10ShaderResourceView * _texture,
	_In_ D3DXMATRIX & _texcoord,
	_In_ float _persent)
{
	g_WorldViewProj_Matrix->SetMatrix((float *)(_world));
	g_World_Matrix->SetMatrix((float *)(_world));

	g_TexCoord_Matrix->SetMatrix(_texcoord);
	g_Time_Scalar->SetFloat(_persent);

	g_refDevice_1->IASetInputLayout(
		g_Basic_inputLayout);

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
D3D10_Renderer hsdk::direct3d::g_D3D10_Renderer;

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