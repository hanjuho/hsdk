#include <hsdk/win/direct3d/d3d10_factory.h>
#include <hsdk/win/wictextureloader.h>
#include <hsdk/win/framework.h>
#include <hash_map>
#include <list>
#include <stack>
#include <string>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags



using namespace hsdk::direct3d;


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------

// 설명 : 
DECL_STRUCT(D3D10MY_TEXTURE)
{

	// 설명 : 
	D3DX10_IMAGE_INFO info;

	// 설명 : 
	hsdk::AutoRelease<ID3D10ShaderResourceView> texture;

};

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------

// Create an instance of the Importer class
Assimp::Importer g_importer;

// 설명 : 
std::hash_map<std::wstring, D3D10MY_TEXTURE> g_D3D10Texture_Container;

//--------------------------------------------------------------------------------------
// Grobal decl function
//--------------------------------------------------------------------------------------

// 설명 :
DECL_FUNC_T(void, compute_CountOfBonesFromAiNode)(
	_In_ aiNode * _node,
	_In_ std::list<aiNode *> & _list);

// 설명 :
DECL_FUNC_T(unsigned int, build_MeshAnimationFromAiNode)(
	_Out_ D3D10_MeshAnimation * _meshAnimation,
	_In_ std::list<aiNode *> & _list,
	_In_ wchar_t(&_buffer)[256],
	_In_ unsigned int _index = 0,
	_In_ unsigned int _parentID = 0);

//--------------------------------------------------------------------------------------
// D3D10_Factory impl
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Factory, void, destroy)(
	_X_ void)
{
	g_D3D10Texture_Container.clear();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Factory, get_Texture)(
	_Out_ ID3D10ShaderResourceView ** _texture,
	_In_ const wchar_t * _directory,
	_In_ const D3DX10_IMAGE_INFO ** _info)
{
	// 중복 검사
	auto iter = g_D3D10Texture_Container.find(_directory);

	if (iter != g_D3D10Texture_Container.end())
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
		auto & element = g_D3D10Texture_Container[_directory];
		ZeroMemory(&element.info, sizeof(D3DX10_IMAGE_INFO));

		// 데이터가 없는 경우
		HRESULT hr;
		IF_FAILED(hr = D3DX10CreateShaderResourceViewFromFile(
			framework::g_Framework_Device.d3d10Device,
			_directory,
			nullptr,
			nullptr,
			&element.texture,
			nullptr))
		{
			g_D3D10Texture_Container.erase(_directory);

			if (hr != E_INVALIDARG)
			{
				return create_Texture(_texture, _directory);
			}

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
CLASS_IMPL_FUNC_T(D3D10_Factory, const D3DX10_IMAGE_INFO *, get_Texture_info)(
	_In_ const wchar_t * _directory)
{
	// 중복 검사
	auto iter = g_D3D10Texture_Container.find(_directory);

	if (iter == g_D3D10Texture_Container.end())
	{
		return nullptr;
	}

	return &iter->second.info;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Factory, create_Texture)(
	_Out_ ID3D10ShaderResourceView ** _texture,
	_In_ const wchar_t * _directory)
{
	DirectX::Image image;

	HRESULT hr;
	IF_FAILED(hr = DirectX::LoadImageFromFile(
		framework::g_Framework_Device.d3d10Device, _directory, &image))
	{
		return hr;
	}

	D3D10_TEXTURE2D_DESC desc;
	desc.Width = image.getWidth();;
	desc.Height = image.getHeight();
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = image.getFormat();;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D10_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;

	D3D10_SUBRESOURCE_DATA data;
	data.pSysMem = image.getData();
	data.SysMemPitch = image.getRowPitch();
	data.SysMemSlicePitch = image.getSize();

	AutoRelease<ID3D10Texture2D> texture;
	IF_SUCCEEDED(framework::g_Framework_Device.d3d10Device->CreateTexture2D(&desc, &data, &texture))
	{
		D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MipLevels = desc.MipLevels;
		srvDesc.TextureCube.MostDetailedMip = 0;

		return framework::g_Framework_Device.d3d10Device->CreateShaderResourceView(texture, &srvDesc, _texture);
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Factory, create_SkyBoxTexture)(
	_Out_ ID3D10ShaderResourceView ** _texture,
	_In_ unsigned int _width,
	_In_ unsigned int _height,
	_In_ const wchar_t * _front,
	_In_ const wchar_t * _back,
	_In_ const wchar_t * _left,
	_In_ const wchar_t * _right,
	_In_ const wchar_t * _top,
	_In_ const wchar_t * _bottom)
{
	const unsigned int maxSize = _width * _height;
	const unsigned int maxByteSize = maxSize * 4;

	std::vector<char> dataBuffers[6];
	D3D10_SUBRESOURCE_DATA datas[6];

	const wchar_t * path[] = {
		_right, _left, _top,
		_bottom, _front, _back };

	for (int index = 0; index < 6; ++index)
	{
		std::vector<char> & vector = dataBuffers[index];

		vector.resize(maxByteSize, 0xF);
		datas[index].pSysMem = &vector[0];
		datas[index].SysMemPitch = _width * 4;
		datas[index].SysMemSlicePitch = maxByteSize;

		DirectX::Image image;
		IF_SUCCEEDED(DirectX::LoadImageFromFile(
			framework::g_Framework_Device.d3d10Device, path[index], &image, maxSize))
		{
			const unsigned int minSize = min(image.getSize(), maxByteSize);
			memcpy(&vector[0], image.getData(), minSize);
		}
	}

	D3D10_TEXTURE2D_DESC desc;
	desc.Width = _width;
	desc.Height = _height;
	desc.MipLevels = 1;
	desc.ArraySize = 6;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D10_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;

	AutoRelease<ID3D10Texture2D> texture;
	IF_SUCCEEDED(framework::g_Framework_Device.d3d10Device->CreateTexture2D(&desc, datas, &texture))
	{
		D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MipLevels = desc.MipLevels;
		srvDesc.TextureCube.MostDetailedMip = 0;

		return framework::g_Framework_Device.d3d10Device->CreateShaderResourceView(texture, &srvDesc, _texture);
	}
	else
	{
		return E_FAIL;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Factory, create_MeshSkyBox)(
	_Out_ D3D10_Mesh & _mesh,
	_In_ float _size)
{
	_mesh.clear();

	HRESULT hr;
	IF_FAILED(hr = _mesh.setup0(1, 1))
	{
		return hr;
	}

	IF_FAILED(hr = _mesh.setup1_Mesh(0, 1, 1))
	{
		return hr;
	}

	// Build box
	D3D10_SkyBoxFormat vBox[] = {
		D3DXVECTOR3(-_size, -_size, -_size),
		D3DXVECTOR3(_size, -_size, -_size),
		D3DXVECTOR3(_size, _size, -_size),
		D3DXVECTOR3(-_size, _size, -_size),

		D3DXVECTOR3(-_size, -_size, _size),
		D3DXVECTOR3(_size, -_size, _size),
		D3DXVECTOR3(_size, _size, _size),
		D3DXVECTOR3(-_size, _size, _size) };

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
		sizeof(D3D10_SkyBoxFormat),
		0,
		sizeof(vBox) / sizeof(D3D10_SkyBoxFormat)))
	{
		return hr;
	}

	unsigned short iBox[] = {
		//front
		0, 2, 3,
		0, 1, 2,
		//right
		1, 6, 2,
		1, 5, 6,
		//back
		5, 7, 6,
		5, 4, 7,
		//left
		4, 3, 7,
		4, 0, 3,
		//top
		3, 6, 7,
		3, 2, 6,
		//bottom
		1, 4, 5,
		1, 0, 4 };

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

	IF_FAILED(hr = _mesh.setup2_RenderDesc(
		0, 0, 0, 0, 36, 0, 0,
		D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
	{
		return hr;
	}

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Factory, create_MeshFromFile)(
	_Out_ D3D10_Mesh & _mesh,
	_In_ const wchar_t * _filePath,
	_In_ const wchar_t * _fileName,
	_Out_ D3D10_MeshAnimation * _meshAnimation)
{
	_mesh.clear();
	if (_meshAnimation)
	{
		_meshAnimation->clear();
	}

	wchar_t wtoa[256];
	ZeroMemory(wtoa, sizeof(wtoa));

	if (_filePath)
	{
		wcscat_s<256>(wtoa, _filePath);
	}

	const unsigned int nameOffset = wcslen(wtoa);
	const unsigned int bufferSize = 256 - nameOffset;
	wcscat_s(&wtoa[nameOffset], bufferSize, _fileName);

	_mesh.userSet_MeshPath(wtoa);
	
	char atow[256];
	wcstombs_s<256>(nullptr, atow, wtoa, sizeof(atow));
	const aiScene * scene = g_importer.ReadFile(
		atow,
		aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_FlipUVs |
		aiProcess_SortByPType |
		aiProcess_LimitBoneWeights);

	IF_INVALID(scene)
	{
		return E_INVALIDARG;
	}

	HRESULT hr;

	if (_meshAnimation)
	{
		wchar_t buffer[256];

		std::list<aiNode *> bones;
		compute_CountOfBonesFromAiNode(scene->mRootNode, bones);

		IF_SUCCEEDED(_meshAnimation->setup0(
			bones.size(),
			scene->mNumAnimations))
		{
			build_MeshAnimationFromAiNode(
				_meshAnimation,
				bones,
				buffer,
				0);
		}

		for (unsigned int aindex = 0; aindex < scene->mNumAnimations; ++aindex)
		{
			const aiAnimation & animation = *scene->mAnimations[aindex];

			mbstowcs_s<256>(nullptr, buffer, animation.mName.C_Str(), sizeof(buffer));
			_meshAnimation->setup1_Animation(
				aindex, buffer,
				animation.mNumChannels,
				animation.mTicksPerSecond,
				animation.mDuration);

			for (unsigned int cindex = 0; cindex < animation.mNumChannels; ++cindex)
			{
				const aiNodeAnim & anim = *animation.mChannels[cindex];

				D3D10MY_FRAME_HINT hint;
				switch (anim.mPostState)
				{
				case aiAnimBehaviour_DEFAULT:
					hint = FRAME_HINT_DEFAULT;
					break;
				case aiAnimBehaviour_CONSTANT:
					hint = FRAME_HINT_CONSTANT;
					break;
				case aiAnimBehaviour_LINEAR:
					hint = FRAME_HINT_LINEAR;
					break;
				case aiAnimBehaviour_REPEAT:
				case _aiAnimBehaviour_Force32Bit:
					hint = FRAME_HINT_REPEAT;
				};

				unsigned int keySize = min(anim.mNumPositionKeys, anim.mNumRotationKeys);
				keySize = min(keySize, anim.mNumScalingKeys);

				mbstowcs_s<256>(nullptr, buffer, anim.mNodeName.C_Str(), sizeof(buffer));
				_meshAnimation->setup2_AnimationBoneKeyFrame(
					aindex, cindex,
					_meshAnimation->find_BoneIDFromName(buffer),
					keySize,
					(D3DXVECTOR3 *)anim.mPositionKeys,
					(D3DXVECTOR3 *)anim.mRotationKeys,
					(D3DXVECTOR3 *)anim.mScalingKeys,
					hint);
			}
		}
	}

	IF_FAILED(hr = _mesh.setup0(
		scene->mNumMaterials ? scene->mNumMaterials : 1,
		scene->mNumMeshes))
	{
		return hr;
	}

	for (unsigned int index = 0; index < scene->mNumMaterials; ++index)
	{
		const aiMaterial & matl = *scene->mMaterials[index];
		aiString aiPath;

		const unsigned int ndt = matl.GetTextureCount(aiTextureType_DIFFUSE);
		if (ndt)
		{
			// 2개 이상의 디퓨즈 텍스처를 받아들일 수 없음
			if (AI_SUCCESS == matl.GetTexture(aiTextureType_DIFFUSE, 0, &aiPath))
			{
				mbstowcs_s(nullptr, &wtoa[nameOffset], bufferSize, aiPath.C_Str(), bufferSize);
				_mesh.setup1_Texture(index, 0, wtoa);
			}
		}

		const unsigned int nnt = matl.GetTextureCount(aiTextureType_NORMALS);
		if (nnt)
		{
			// 2개 이상의 노말 텍스처를 받아들일 수 없음
			if (AI_SUCCESS == matl.GetTexture(aiTextureType_NORMALS, 0, &aiPath))
			{
				mbstowcs_s(nullptr, &wtoa[nameOffset], bufferSize, aiPath.C_Str(), bufferSize);
				_mesh.setup1_Texture(index, 1, wtoa);
			}
		}

		const unsigned int nst = matl.GetTextureCount(aiTextureType_SPECULAR);
		if (nnt)
		{
			// 2개 이상의 스펙큘러 텍스처를 받아들일 수 없음
			if (AI_SUCCESS == matl.GetTexture(aiTextureType_SPECULAR, 0, &aiPath))
			{
				mbstowcs_s(nullptr, &wtoa[nameOffset], bufferSize, aiPath.C_Str(), bufferSize);
				_mesh.setup1_Texture(index, 2, wtoa);
			}
		}

		aiColor4D material;

		if (AI_FAILURE == aiGetMaterialColor(&matl, AI_MATKEY_COLOR_DIFFUSE, &material))
		{
			material = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
		}

		_mesh.setup1_Material(
			index, 0, { material.r, material.g, material.b, material.a });

		if (AI_FAILURE == aiGetMaterialColor(&matl, AI_MATKEY_COLOR_AMBIENT, &material))
		{
			material = aiColor4D(0.2f, 0.2f, 0.2f, 1.f);
		}

		_mesh.setup1_Material(
			index, 1, { material.r, material.g, material.b, material.a });

		if (AI_FAILURE == aiGetMaterialColor(&matl, AI_MATKEY_COLOR_SPECULAR, &material))
		{
			material = aiColor4D(0.f, 0.f, 0.f, 0.f);
		}

		_mesh.setup1_Material(
			index, 2, { material.r, material.g, material.b, material.a });

		if (AI_FAILURE == aiGetMaterialColor(&matl, AI_MATKEY_COLOR_EMISSIVE, &material))
		{
			material = aiColor4D(0.f, 0.f, 0.f, 0.f);
		}

		_mesh.setup1_Material(
			index, 3, { material.r, material.g, material.b, material.a });

		unsigned int max;

		float shininess, strength;
		if (AI_SUCCESS == aiGetMaterialFloatArray(&matl, AI_MATKEY_SHININESS, &shininess, &(max = 1)))
		{
			if (AI_SUCCESS == aiGetMaterialFloatArray(&matl, AI_MATKEY_SHININESS_STRENGTH, &strength, &(max = 1)))
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
		const aiMesh & mesh = *scene->mMeshes[index];
		IF_FAILED(_mesh.setup1_Mesh(index, 1, 1))
		{
			return hr;
		}

		D3D10_PRIMITIVE_TOPOLOGY topology;
		unsigned int faceSize = 0;

		switch (mesh.mPrimitiveTypes)
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

		std::vector<unsigned int> indices(mesh.mNumFaces * faceSize);
		unsigned int offsetOrSumOfindices = 0;

		unsigned int * indicesBuffer = &indices[0];
		for (unsigned int findex = 0; findex < mesh.mNumFaces; ++findex)
		{
			aiFace & face = mesh.mFaces[findex];

			if (face.mNumIndices != faceSize)
			{
				continue;
			}

			memcpy(
				&indicesBuffer[offsetOrSumOfindices],
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

		std::vector<D3D10_SkinnedFormat> vbuffer(mesh.mNumVertices);
		ZeroMemory(&vbuffer[0], mesh.mNumVertices * sizeof(D3D10_SkinnedFormat));

		D3D10_SkinnedFormat * formatBuffer = &vbuffer[0];
		for (unsigned int vindex = 0; vindex < mesh.mNumVertices; ++vindex)
		{
			D3D10_SkinnedFormat & format = formatBuffer[vindex];
			memcpy(&format.pos, &mesh.mVertices[vindex], sizeof(D3DXVECTOR3));
			memcpy(&format.norm, &mesh.mNormals[vindex], sizeof(D3DXVECTOR3));
			memcpy(&format.tex, &mesh.mTextureCoords[0][vindex], sizeof(D3DXVECTOR2));
			memcpy(&format.color, &mesh.mColors[0][vindex], sizeof(D3DXVECTOR4));
		}

		if (_meshAnimation)
		{
			for (unsigned int bindex = 0; bindex < mesh.mNumBones; ++bindex)
			{
				const aiBone & bone = *mesh.mBones[bindex];

				unsigned int nodeBindex = 0;

				mbstowcs_s<256>(nullptr, wtoa, bone.mName.C_Str(), sizeof(wtoa));
				nodeBindex = _meshAnimation->find_BoneIDFromName(wtoa);

				_meshAnimation->userSet_BoneMatrix(
					nodeBindex, (float *)&bone.mOffsetMatrix);

				for (unsigned int windex = 0; windex < bone.mNumWeights; ++windex)
				{
					const aiVertexWeight & wight = bone.mWeights[windex];
					D3D10_SkinnedFormat & format = formatBuffer[wight.mVertexId];

					for (unsigned int i = 0; i < 4; ++i)
					{
						if (format.bindexs[i])
						{
							continue;
						}
						else
						{
							format.bindexs[i] = nodeBindex;
							format.bweight[i] = wight.mWeight;
							break;
						}
					}
				}
			}
		}

		// Vertex Buffer
		D3D10_BUFFER_DESC vBufferDesc;
		vBufferDesc.ByteWidth = mesh.mNumVertices * sizeof(D3D10_SkinnedFormat);
		vBufferDesc.Usage = D3D10_USAGE_DEFAULT;
		vBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		vBufferDesc.CPUAccessFlags = 0;
		vBufferDesc.MiscFlags = 0;

		IF_FAILED(hr = _mesh.setup2_Vertexbuffer(
			index, 0,
			vBufferDesc,
			&vbuffer[0],
			sizeof(D3D10_SkinnedFormat),
			0, mesh.mNumVertices))
		{
			return hr;
		}

		_mesh.setup2_RenderDesc(
			index, 0, mesh.mMaterialIndex,
			0, offsetOrSumOfindices, 0, 1, topology);
	}

	g_importer.FreeScene();

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Grobal impl function
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, compute_CountOfBonesFromAiNode)(
	_In_ aiNode * _node,
	_In_ std::list<aiNode *> & _list)
{
	IF_INVALID(_node)
	{
		return;
	}

	_list.push_back(_node);

	for (unsigned int index = 0; index < _node->mNumChildren; ++index)
	{
		compute_CountOfBonesFromAiNode(_node->mChildren[index], _list);
	}

	return;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(unsigned int, build_MeshAnimationFromAiNode)(
	_Out_ D3D10_MeshAnimation * _meshAnimation,
	_In_ std::list<aiNode *> & _list,
	_In_ wchar_t(&_buffer)[256],
	_In_ unsigned int _index,
	_In_ unsigned int _parentID)
{
	const aiNode & node = *_list.front();
	_list.pop_front();

	unsigned int length = 0;
	for (unsigned int cindex = 0; cindex < node.mNumChildren; ++cindex)
	{
		length += build_MeshAnimationFromAiNode(
			_meshAnimation, _list, _buffer,
			_meshAnimation->get_NumOfBones() - _list.size(),
			_index);
	}

	mbstowcs_s<256>(nullptr, _buffer, node.mName.C_Str(), sizeof(_buffer));
	if (FAILED(_meshAnimation->setup1_BoneNode(
		_index,
		_parentID,
		length,
		_buffer,
		(float*)&node.mTransformation)))
	{
		return 0;
	}

	return length ? length + 1 : 1;
}

//--------------------------------------------------------------------------------------
// open variable 
//--------------------------------------------------------------------------------------

// 설명 : 
D3D10_Factory hsdk::direct3d::g_D3D10_Factory;