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
ID3D10EffectTechnique * g_Basic_Technique;

// 설명 :
ID3D10EffectTechnique * g_Skinned0_Technique;

// 설명 : Get the effect variable handles
ID3D10EffectTechnique * g_SkyBox0_Technique;

//--------------------------------------------------------------------------------------
// Grobal matrix variable
//--------------------------------------------------------------------------------------

D3D10_RenderVariable g_RenderVariable;

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
std::wstring g_EffectPath;

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Renderer, initialize_Shader)(
	_X_ const wchar_t * _directory)
{
	HRESULT hr = E_FAIL;

	if (g_D3D10Effect && (g_EffectPath == _directory))
	{
		return S_OK;
	}

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

	g_Basic_Technique = effect->GetTechniqueByName("Basic_0");
	g_Skinned0_Technique = effect->GetTechniqueByName("Skinned_0");
	g_SkyBox0_Technique = effect->GetTechniqueByName("SkyBox_0");

	g_RenderVariable.mWorld = effect->GetVariableByName("g_mWorld")->AsMatrix();
	g_RenderVariable.mView = effect->GetVariableByName("g_mView")->AsMatrix();
	g_RenderVariable.mProj = effect->GetVariableByName("g_mProj")->AsMatrix();
	g_RenderVariable.mWorldView = effect->GetVariableByName("g_mWorldView")->AsMatrix();
	g_RenderVariable.mWorldViewProj = effect->GetVariableByName("g_mWorldViewProj")->AsMatrix();
	g_RenderVariable.mTexture = effect->GetVariableByName("g_mTexture")->AsMatrix();
	g_RenderVariable.vDiffuse = effect->GetVariableByName("g_vDiffuse")->AsVector();
	g_RenderVariable.vAmbient = effect->GetVariableByName("g_vAmbient")->AsVector();
	g_RenderVariable.vSpecular = effect->GetVariableByName("g_vSpecular")->AsVector();
	g_RenderVariable.vEmissive = effect->GetVariableByName("g_vEmissive")->AsVector();
	g_RenderVariable.shininess = effect->GetVariableByName("g_shininess")->AsScalar();
	g_RenderVariable.vLightDir = effect->GetVariableByName("g_vLightDir")->AsVector();
	g_RenderVariable.texDiffuse = effect->GetVariableByName("g_texDiffuse")->AsShaderResource();
	g_RenderVariable.texNormal = effect->GetVariableByName("g_texNormal")->AsShaderResource();
	g_RenderVariable.texSpecular = effect->GetVariableByName("g_texSpecular")->AsShaderResource();
	g_RenderVariable.mSkyBox = effect->GetVariableByName("g_mSkyBox")->AsMatrix();
	g_RenderVariable.texSkyBox = effect->GetVariableByName("g_texSkyBox")->AsShaderResource();
	g_RenderVariable.mBone = effect->GetVariableByName("g_mBone")->AsMatrix();
	g_RenderVariable.vsFlag = effect->GetVariableByName("g_vsFlag")->AsScalar();
	g_RenderVariable.vsTime = effect->GetVariableByName("g_vsTime")->AsScalar();
	g_RenderVariable.psFlag = effect->GetVariableByName("g_psFlag")->AsScalar();
	g_RenderVariable.psTime = effect->GetVariableByName("g_psTime")->AsScalar();

	g_D3D10Effect = effect;
	g_EffectPath = _directory;

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
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(D3D10_BasicFormat, nor), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(D3D10_BasicFormat, tex), D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	//
	const D3D10_INPUT_ELEMENT_DESC skinnedlayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(D3D10_SkinnedFormat, pos), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(D3D10_SkinnedFormat, nor), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(D3D10_SkinnedFormat, tex), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(D3D10_SkinnedFormat, color), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, offsetof(D3D10_SkinnedFormat, bindexs), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(D3D10_SkinnedFormat, bweight), D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	//
	const D3D10_INPUT_ELEMENT_DESC skyboxlayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(D3D10_SkyBoxFormat, pos), D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D10_PASS_DESC PassDesc;

	g_Basic_Technique->GetPassByIndex(0)->GetDesc(&PassDesc);
	IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateInputLayout(
		basiclayout,
		sizeof(basiclayout) / sizeof(basiclayout[0]),
		PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize,
		&Basic_inputLayout))
	{
		return hr;
	}

	g_Skinned0_Technique->GetPassByIndex(0)->GetDesc(&PassDesc);
	IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateInputLayout(
		skinnedlayout,
		sizeof(skinnedlayout) / sizeof(skinnedlayout[0]),
		PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize,
		&Skinned_inputLayout))
	{
		return hr;
	}

	g_SkyBox0_Technique->GetPassByIndex(0)->GetDesc(&PassDesc);
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
CLASS_IMPL_FUNC(D3D10_Renderer, initialize_TextureCacheBuffer)(
	_X_ void)
{
	HRESULT hr = E_FAIL;

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
		0, 1, 2 };

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

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Renderer, initialize)(
	_X_ const wchar_t * _shaderFilePath)
{
	HRESULT hr = E_FAIL;

	std::wstring directory = _shaderFilePath;
	directory.append(L"main.fx");

	IF_FAILED(hr = initialize_Shader(directory.c_str()))
	{
		return hr;
	}

	IF_FAILED(hr = initialize_Layout())
	{
		return hr;
	}

	IF_FAILED(hr = initialize_TextureCacheBuffer())
	{
		return hr;
	}

	g_refDevice_1 = framework::g_Framework_Device.d3d10Device;

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, destroy)(
	_X_ void)
{
	g_Sprite_VBuffer.~AutoRelease();
	g_Sprite_IBuffer.~AutoRelease();
	g_Basic_inputLayout.~AutoRelease();
	g_Skinned_inputLayout.~AutoRelease();
	g_Skybox_inputLayout.~AutoRelease();
	g_D3D10Effect.~AutoRelease();

	g_refDevice_1 = nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, render_Skinned)(
	_In_ D3D10_Mesh & _mesh,
	_In_ D3D10_Animation & _animation,
	_In_ D3D10_Animation_Recorder & _pos,
	_In_ unsigned int _pass)
{
	g_refDevice_1->IASetInputLayout(g_Skinned_inputLayout);

	auto begin = _mesh.meshs.begin();
	auto end = _mesh.meshs.end();

	while (begin != end)
	{
		g_refDevice_1->IASetVertexBuffers(0, 1,
			&begin->vetexbuffer.vertexbuffer,
			&begin->vetexbuffer.vertexbuffers_Strides,
			&begin->vetexbuffer.vertexbuffers_Offsets);

		g_refDevice_1->IASetIndexBuffer(
			begin->indexbuffer.indexbuffer,
			begin->indexbuffer.indexType, 0);

		const unsigned int boneSize = begin->boneNode.size();

		if (boneSize)
		{
			D3DXMATRIX boneMatrices[80];

			auto & node = begin->boneNode;
			animation::build_MeshBoneMatrix(
				boneMatrices,
				&node[0],
				node.size(),
				_animation,
				_pos);

			set_MatricesBone(boneMatrices, 80);
		}

		for (unsigned int irender = 0; irender < begin->subsets.size(); ++irender)
		{
			const D3D10MY_RENDER_DESC & desc = begin->subsets[irender];
			const D3D10MY_MATERIAL & material = _mesh.materials[desc.material_id];
			set_TextureDiffuse(material.diffuseRV);
			set_TextureNormal(material.normalRV);
			set_TextureNormal(material.normalRV);
			set_ColorDiffuse((float *)&material.diffuse);
			set_ColorAmbient((float *)&material.ambient);
			set_ColorSpecular((float *)&material.specular);
			set_ColorEmissive((float *)&material.emissive);
			set_ScalarShininess(material.shininess);
			g_refDevice_1->IASetPrimitiveTopology(desc.primitiveType);
			g_Skinned0_Technique->GetPassByIndex(_pass)->Apply(0);
			g_refDevice_1->DrawIndexed(
				desc.indexCount,
				desc.indexStart,
				desc.vertexbufferStart);
		}

		++begin;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, render_Mesh)(
	_In_ D3D10_Mesh & _mesh)
{
	g_refDevice_1->IASetInputLayout(g_Basic_inputLayout);

	auto begin = _mesh.meshs.begin();
	auto end = _mesh.meshs.end();

	while (begin != end)
	{
		g_refDevice_1->IASetVertexBuffers(0, 1,
			&begin->vetexbuffer.vertexbuffer,
			&begin->vetexbuffer.vertexbuffers_Strides,
			&begin->vetexbuffer.vertexbuffers_Offsets);

		g_refDevice_1->IASetIndexBuffer(
			begin->indexbuffer.indexbuffer,
			begin->indexbuffer.indexType, 0);

		for (unsigned int irender = 0; irender < begin->subsets.size(); ++irender)
		{
			const D3D10MY_RENDER_DESC & desc = begin->subsets[irender];
			set_ColorDiffuse(_mesh.materials[desc.material_id].diffuse);
			set_TextureDiffuse(_mesh.materials[desc.material_id].diffuseRV);
			g_refDevice_1->IASetPrimitiveTopology(desc.primitiveType);
			g_Basic_Technique->GetPassByIndex(0)->Apply(0);
			g_refDevice_1->DrawIndexed(
				desc.indexCount,
				desc.indexStart,
				desc.vertexbufferStart);
		}

		++begin;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, render_SkyBox)(
	_In_ D3D10_Mesh & _mesh)
{
	g_refDevice_1->IASetInputLayout(g_Skybox_inputLayout);

	auto begin = _mesh.meshs.begin();
	auto end = _mesh.meshs.end();

	while (begin != end)
	{
		g_refDevice_1->IASetVertexBuffers(0, 1,
			&begin->vetexbuffer.vertexbuffer,
			&begin->vetexbuffer.vertexbuffers_Strides,
			&begin->vetexbuffer.vertexbuffers_Offsets);

		g_refDevice_1->IASetIndexBuffer(
			begin->indexbuffer.indexbuffer,
			begin->indexbuffer.indexType, 0);

		for (unsigned int irender = 0; irender < begin->subsets.size(); ++irender)
		{
			const D3D10MY_RENDER_DESC & desc = begin->subsets[irender];
			set_ColorDiffuse(_mesh.materials[desc.material_id].diffuse);
			set_TextureSkyBox(_mesh.materials[desc.material_id].diffuseRV);
			g_refDevice_1->IASetPrimitiveTopology(desc.primitiveType);
			g_SkyBox0_Technique->GetPassByIndex(0)->Apply(0);
			g_refDevice_1->DrawIndexed(
				desc.indexCount,
				desc.indexStart,
				desc.vertexbufferStart);
		}

		++begin;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, render_UIRectangle)(
	_In_ D3DXVECTOR4 * _color)
{
	set_ColorDiffuse((float *)_color);

	g_refDevice_1->IASetInputLayout(g_Basic_inputLayout);
	g_refDevice_1->IASetVertexBuffers(0, 1, &g_Sprite_VBuffer, &g_Sprite_Stride, &g_Sprite_Offset);
	g_refDevice_1->IASetIndexBuffer(g_Sprite_IBuffer, DXGI_FORMAT_R16_UINT, 0);
	g_refDevice_1->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_Basic_Technique->GetPassByIndex(1)->Apply(0);
	g_refDevice_1->DrawIndexed(6, 0, 0);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, render_UITexture)(
	_In_ ID3D10ShaderResourceView * _texture,
	_In_ const D3DXMATRIX * _texcoord)
{
	set_MatrixTexture(_texcoord);
	set_TextureDiffuse(_texture);

	g_refDevice_1->IASetInputLayout(g_Basic_inputLayout);
	g_refDevice_1->IASetVertexBuffers(0, 1, &g_Sprite_VBuffer, &g_Sprite_Stride, &g_Sprite_Offset);
	g_refDevice_1->IASetIndexBuffer(g_Sprite_IBuffer, DXGI_FORMAT_R16_UINT, 0);
	g_refDevice_1->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_Basic_Technique->GetPassByIndex(1)->Apply(0);
	g_refDevice_1->DrawIndexed(6, 0, 0);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, render_Font)(
	_In_ D3D10MY_CONTEXT & _context,
	_In_ ID3D10ShaderResourceView * _fontTable)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_MatrixWorld)(
	_In_ const D3DXMATRIX * _matrix)
{
	g_RenderVariable.mWorld->SetMatrix((float *)_matrix);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_MatrixView)(
	_In_ const D3DXMATRIX * _matrix)
{
	g_RenderVariable.mView->SetMatrix((float *)_matrix);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_MatrixProj)(
	_In_ const D3DXMATRIX * _matrix)
{
	g_RenderVariable.mProj->SetMatrix((float *)_matrix);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_MatrixWorldView)(
	_In_ const D3DXMATRIX * _matrix)
{
	g_RenderVariable.mWorldView->SetMatrix((float *)_matrix);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_MatrixWorldViewProj)(
	_In_ const D3DXMATRIX * _matrix)
{
	g_RenderVariable.mWorldViewProj->SetMatrix((float *)_matrix);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_MatrixTexture)(
	_In_ const D3DXMATRIX * _matrix)
{
	g_RenderVariable.mTexture->SetMatrix((float *)_matrix);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_ColorDiffuse)(
	_In_ float * _color)
{
	g_RenderVariable.vDiffuse->SetFloatVector(_color);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_ColorAmbient)(
	_In_ float * _color)
{
	g_RenderVariable.vAmbient->SetFloatVector(_color);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_ColorSpecular)(
	_In_ float * _color)
{
	g_RenderVariable.vSpecular->SetFloatVector(_color);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_ColorEmissive)(
	_In_ float * _color)
{
	g_RenderVariable.vEmissive->SetFloatVector(_color);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_ScalarShininess)(
	_In_ float _value)
{
	g_RenderVariable.shininess->SetFloat(_value);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_LightDir)(
	_In_ float * _color)
{
	g_RenderVariable.vLightDir->SetFloatVector(_color);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_TextureDiffuse)(
	_In_ ID3D10ShaderResourceView * _texture)
{
	g_RenderVariable.texDiffuse->SetResource(_texture);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_TextureNormal)(
	_In_ ID3D10ShaderResourceView * _texture)
{
	g_RenderVariable.texNormal->SetResource(_texture);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_TextureSpecular)(
	_In_ ID3D10ShaderResourceView * _texture)
{
	g_RenderVariable.texSpecular->SetResource(_texture);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_MatrixSkyBox)(
	_In_ const D3DXMATRIX * _matrix)
{
	g_RenderVariable.mSkyBox->SetMatrix((float *)_matrix);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_TextureSkyBox)(
	_In_ ID3D10ShaderResourceView * _texture)
{
	g_RenderVariable.texSkyBox->SetResource(_texture);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_MatricesBone)(
	_In_ const D3DXMATRIX * _matrices,
	_In_ unsigned int _size)
{
	g_RenderVariable.mBone->SetMatrixArray((float *)_matrices, 0, _size);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_ScalarVSFlag)(
	_In_ unsigned int _value)
{
	g_RenderVariable.vsFlag->SetInt(_value);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_ScalarVSTime)(
	_In_ float _value)
{
	g_RenderVariable.vsTime->SetFloat(_value);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_ScalarPSFlag)(
	_In_ unsigned int _value)
{
	g_RenderVariable.psFlag->SetInt(_value);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Renderer, void, set_ScalarPSTime)(
	_In_ float _value)
{
	g_RenderVariable.psTime->SetFloat(_value);
}

//--------------------------------------------------------------------------------------
// open variable 
//--------------------------------------------------------------------------------------

// 설명 : 
D3D10_Renderer hsdk::direct3d::g_D3D10_Renderer;

// 설명 :
const D3D10_RenderVariable & hsdk::direct3d::g_D3D10_RenderVariable = g_RenderVariable;

// 설명 : 
const D3DXMATRIX hsdk::direct3d::g_D3D10_identityMatrix = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0, };

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

// 설명 :
D3DXMATRIX hsdk::direct3d::g_D3D10_ViewProjMatrix = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0, };