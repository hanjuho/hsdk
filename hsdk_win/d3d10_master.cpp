#include <hsdk/win/frame/direct3d/d3d10_master.h>
#include <hash_map>
#include <string>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags



using namespace hsdk;
using namespace direct3d;


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------

// Create an instance of the Importer class
Assimp::Importer g_importer;

// 설명 : 
ID3D10Device * g_refDevice_0 = nullptr;

// 설명 : 
std::hash_map<std::wstring, D3D10MY_TEXTURE> g_Manager_Texture_Container;

//--------------------------------------------------------------------------------------
// D3D10_Master impl
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Master, initialize)(
	/* [r] */ ID3D10Device * _device)
{
	if (nullptr == g_refDevice_0)
	{
		g_refDevice_0 = _device;
	}
	else if (_device != g_refDevice_0)
	{
		destroy();

		g_refDevice_0 = _device;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Master, void, destroy)(
	/* [x] */ void)
{
	g_Manager_Texture_Container.clear();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Master, get_Texture)(
	/* [w] */ ID3D10ShaderResourceView ** _texture,
	/* [r] */ const wchar_t * _directory,
	/* [r] */ const D3DX10_IMAGE_INFO ** _info)
{
	// 중복 검사
	auto iter = g_Manager_Texture_Container.find(_directory);

	if (iter != g_Manager_Texture_Container.end())
	{
		// 이미 있는 경우
		(*_texture) = iter->second.texture;

		if (_info)
		{
			(*_info) = &iter->second.info;
		}
	}
	else
	{
		auto & element = g_Manager_Texture_Container[_directory];
		ZeroMemory(&element.info, sizeof(D3DX10_IMAGE_INFO));

		// 데이터가 없는 경우
		HRESULT hr;
		IF_FAILED(hr = D3DX10CreateShaderResourceViewFromFile(
			g_refDevice_0,
			_directory,
			nullptr,
			nullptr,
			&element.texture,
			nullptr))
		{
			g_Manager_Texture_Container.erase(_directory);

			return hr;
		}

		D3DX10GetImageInfoFromFile(_directory, nullptr, &element.info, nullptr);

		// added one count from AutoRelease
		(*_texture) = element.texture;

		if (_info)
		{
			(*_info) = &element.info;
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Master, const D3DX10_IMAGE_INFO *, get_Texture_info)(
	/* [r] */ const wchar_t * _directory)
{
	// 중복 검사
	auto iter = g_Manager_Texture_Container.find(_directory);

	if (iter == g_Manager_Texture_Container.end())
	{
		return nullptr;
	}

	return &iter->second.info;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Master, create_MeshSkyBox)(
	/* [w] */ D3D10_Mesh & _mesh,
	/* [r] */ float _size)
{
	_mesh.clear();

	HRESULT hr;
	IF_FAILED(hr = _mesh.setup0(
		g_refDevice_0, 6, 1))
	{
		return hr;
	}

	IF_FAILED(hr = _mesh.setup1_Mesh(0, 6, 1))
	{
		return hr;
	}

	// Build box
	D3D10_BasicFormat vBox[] = {
		// front
		{ D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
		{ D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		// top
		{ D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
		{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },
		// bottom
		{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
		{ D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		// left
		{ D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
		{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
		// rignt
		{ D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
		{ D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		// back
		{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
		{ D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) } };

	// Vertex Buffer
	D3D10_BUFFER_DESC vBufferDesc;
	vBufferDesc.ByteWidth = sizeof(vBox);
	vBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vBufferDesc.CPUAccessFlags = 0;
	vBufferDesc.MiscFlags = 0;

	IF_FAILED(hr = _mesh.setup2_Vertexbuffer(
		0, 0,
		vBufferDesc,
		vBox,
		sizeof(D3D10_BasicFormat),
		0,
		sizeof(vBox) / sizeof(D3D10_BasicFormat)))
	{
		return hr;
	}

	unsigned short iBox[] = {
		// front
		3, 1, 0,
		2, 1, 3,
		// top
		6, 4, 5,
		7, 4, 6,
		// bottom
		11, 9, 8,
		10, 9, 11,
		// left
		14, 12, 13,
		15, 12, 14,
		// rignt
		19, 17, 16,
		18, 17, 19,
		// back
		22, 20, 21,
		23, 20, 22 };

	// index Buffer
	D3D10_BUFFER_DESC iBufferDesc;
	iBufferDesc.ByteWidth = sizeof(iBox);
	iBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	iBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	iBufferDesc.CPUAccessFlags = 0;
	iBufferDesc.MiscFlags = 0;

	IF_FAILED(hr = _mesh.setup2_indexbuffer(
		0,
		iBufferDesc,
		iBox,
		DXGI_FORMAT_R16_UINT,
		sizeof(iBox) / sizeof(unsigned short)))
	{
		return hr;
	}

	for (unsigned int index = 0; index < 6; ++index)
	{
		IF_FAILED(hr = _mesh.setup2_RenderDesc(
			0, index, index, index * 6, (index * 6) + 6, 0, 0,
			D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
		{
			return hr;
		}
	}

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Master, create_MeshFromFile)(
	/* [w] */ D3D10_Mesh & _mesh,
	/* [r] */ const wchar_t * _szFileName)
{
	char fileName[512];
	wcstombs_s<512>(nullptr, fileName, _szFileName, sizeof(fileName));
		
	const aiScene * scene = g_importer.ReadFile(
		fileName,
		aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_FlipUVs |
		aiProcess_SortByPType |
		aiProcess_LimitBoneWeights);

	IF_INVALID(scene)
	{
		return E_INVALIDARG;
	}

	HRESULT hr;

	IF_FAILED(hr = _mesh.setup0(
		g_refDevice_0,
		0 == scene->mNumMaterials ? 1 : scene->mNumMaterials,
		scene->mNumMeshes))
	{
		return hr;
	}

	for (unsigned int index = 0; index < scene->mNumMaterials; ++index)
	{
		aiMaterial * matl = scene->mMaterials[index];

		const unsigned int ndt = matl->GetTextureCount(aiTextureType_DIFFUSE);
		if (ndt)
		{
			aiString aiPath;
			if (AI_SUCCESS == matl->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath))
			{
				wchar_t path[512];
				mbstowcs_s<512>(nullptr, path, aiPath.C_Str(), sizeof(path));

				_mesh.setup1_Texture(path, index, 0);
			}
		}

		const unsigned int nnt = matl->GetTextureCount(aiTextureType_NORMALS);
		if (nnt)
		{
			aiString aiPath;
			if (AI_SUCCESS == matl->GetTexture(aiTextureType_NORMALS, 0, &aiPath))
			{
				wchar_t path[512];
				mbstowcs_s<512>(nullptr, path, aiPath.C_Str(), sizeof(path));

				_mesh.setup1_Texture(path, index, 1);
			}
		}

		const unsigned int nst = matl->GetTextureCount(aiTextureType_SPECULAR);
		if (nnt)
		{
			aiString aiPath;
			if (AI_SUCCESS == matl->GetTexture(aiTextureType_SPECULAR, 0, &aiPath))
			{
				wchar_t path[512];
				mbstowcs_s<512>(nullptr, path, aiPath.C_Str(), sizeof(path));

				_mesh.setup1_Texture(path, index, 2);
			}
		}


		aiColor4D material;
		if (AI_SUCCESS != aiGetMaterialColor(matl, AI_MATKEY_COLOR_DIFFUSE, &material))
		{
			material = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
		}

		_mesh.setup1_Material(
			index, 0, { material.r, material.g, material.b, material.a });

		if (AI_SUCCESS != aiGetMaterialColor(matl, AI_MATKEY_COLOR_AMBIENT, &material))
		{
			material = aiColor4D(0.2f, 0.2f, 0.2f, 1.f);
		}

		_mesh.setup1_Material(
			index, 1, { material.r, material.g, material.b, material.a });

		if (AI_SUCCESS != aiGetMaterialColor(matl, AI_MATKEY_COLOR_SPECULAR, &material))
		{
			material = aiColor4D(0.f, 0.f, 0.f, 0.f);
		}

		_mesh.setup1_Material(
			index, 2, { material.r, material.g, material.b, material.a });

		if (AI_SUCCESS != aiGetMaterialColor(matl, AI_MATKEY_COLOR_EMISSIVE, &material))
		{
			material = aiColor4D(0.f, 0.f, 0.f, 0.f);
		}

		_mesh.setup1_Material(
			index, 3, { material.r, material.g, material.b, material.a });
		
		unsigned int max;

		float shininess, strength;
		if (AI_SUCCESS == aiGetMaterialFloatArray(matl, AI_MATKEY_SHININESS, &shininess, &(max = 1)))
		{
			if (AI_SUCCESS == aiGetMaterialFloatArray(matl, AI_MATKEY_SHININESS_STRENGTH, &strength, &(max = 1)))
			{
				shininess *= strength;
			}
		}
		else
		{
			shininess = 0.0f;
		}

		_mesh.setup1_Shininess(index, shininess);
	}
	
	if (scene->mNumMaterials == 0)
	{
		_mesh.setup1_Material(0, 0, D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));
		_mesh.setup1_Material(0, 1, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.f));
		_mesh.setup1_Material(0, 2, D3DXVECTOR4(0.f, 0.f, 0.f, 0.f));
		_mesh.setup1_Material(0, 3, D3DXVECTOR4(0.f, 0.f, 0.f, 0.f));
		_mesh.setup1_Shininess(0, 0.0f);
	}
	
	for (unsigned int index = 0; index < scene->mNumMeshes; ++index)
	{
		const aiMesh * mesh = scene->mMeshes[index];
		IF_FAILED(_mesh.setup1_Mesh(index, 1, 1))
		{
			return hr;
		}

		D3D10_PRIMITIVE_TOPOLOGY topology;
		unsigned int faceSize = 0;

		switch (mesh->mPrimitiveTypes)
		{
		case aiPrimitiveType::aiPrimitiveType_POINT:
			topology = D3D10_PRIMITIVE_TOPOLOGY_POINTLIST;
			faceSize = 1;
			break;
		case aiPrimitiveType::aiPrimitiveType_LINE:
			topology = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
			faceSize = 2;
			break;
		case aiPrimitiveType::aiPrimitiveType_TRIANGLE:
		case aiPrimitiveType::aiPrimitiveType_POLYGON:
		case aiPrimitiveType::_aiPrimitiveType_Force32Bit:
			faceSize = 3;
			topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		};

		std::vector<unsigned int> indices(mesh->mNumFaces * faceSize);
		unsigned int offsetOrSumOfindices = 0;

		for (unsigned int findex = 0; findex < mesh->mNumFaces; ++findex)
		{
			aiFace & face = mesh->mFaces[findex];

			if (face.mNumIndices != faceSize)
			{
				continue;
			}

			memcpy(
				&indices[offsetOrSumOfindices],
				face.mIndices,
				face.mNumIndices * sizeof(unsigned int));

			offsetOrSumOfindices += face.mNumIndices;
		}
		
		// index Buffer
		D3D10_BUFFER_DESC iBufferDesc;
		iBufferDesc.ByteWidth = offsetOrSumOfindices * sizeof(unsigned int);
		iBufferDesc.Usage = D3D10_USAGE_DEFAULT;
		iBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
		iBufferDesc.CPUAccessFlags = 0;
		iBufferDesc.MiscFlags = 0;

		IF_FAILED(hr = _mesh.setup2_indexbuffer(
			index,
			iBufferDesc,
			&indices[0],
			DXGI_FORMAT_R32_UINT,
			offsetOrSumOfindices))
		{
			return hr;
		}

		std::vector<D3D10_SkinnedFormat> vbuffer(mesh->mNumVertices);
		for (unsigned int vindex = 0; vindex < mesh->mNumVertices; ++vindex)
		{
			memcpy(&vbuffer[vindex].pos, &mesh->mVertices[vindex], sizeof(D3DXVECTOR3));
			memcpy(&vbuffer[vindex].norm, &mesh->mNormals[vindex], sizeof(D3DXVECTOR3));
			memcpy(&vbuffer[vindex].tex, &mesh->mTextureCoords[0][vindex], sizeof(D3DXVECTOR2));
			memcpy(&vbuffer[vindex].color, &mesh->mColors[0][vindex], sizeof(D3DXVECTOR4));
			int a = 0;
		}

		// Vertex Buffer
		D3D10_BUFFER_DESC vBufferDesc;
		vBufferDesc.ByteWidth = mesh->mNumVertices * sizeof(D3D10_SkinnedFormat);
		vBufferDesc.Usage = D3D10_USAGE_DEFAULT;
		vBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		vBufferDesc.CPUAccessFlags = 0;
		vBufferDesc.MiscFlags = 0;
		
		IF_FAILED(hr = _mesh.setup2_Vertexbuffer(
			index, 0,
			vBufferDesc,
			&vbuffer[0],
			sizeof(D3D10_SkinnedFormat),
			0, mesh->mNumVertices))
		{
			return hr;
		}

		_mesh.setup2_RenderDesc(
			index, 0, mesh->mMaterialIndex,
			0, offsetOrSumOfindices, 0, 1, topology);
	}

	g_importer.FreeScene();

	return S_OK;
}

//--------------------------------------------------------------------------------------
// open variable 
//--------------------------------------------------------------------------------------

// 설명 : 
D3D10_Master hsdk::direct3d::g_D3D10_Master;