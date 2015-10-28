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

// ���� : 
DECL_STRUCT(D3D10MY_TEXTURE)
{

	// ���� : 
	D3DX10_IMAGE_INFO info;

	// ���� : 
	hsdk::AutoRelease<ID3D10ShaderResourceView> texture;

};

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------

// Create an instance of the Importer class
Assimp::Importer g_importer;

// ���� : 
std::hash_map<std::wstring, D3D10MY_TEXTURE> g_D3D10Texture_Container;

//--------------------------------------------------------------------------------------
// Grobal decl function
//--------------------------------------------------------------------------------------

// ���� :
DECL_FUNC_T(void, sort_BonesFromAiNode)(
	_In_ aiNode * _node,
	_In_ std::list<aiNode *> & _list);

// ���� :
DECL_FUNC_T(unsigned int, build_BonesFromAiNode)(
	_Out_ std::vector<D3D10MY_BONE> & _bones,
	_In_ std::list<aiNode *> & _list,
	_In_ wchar_t(&_buffer)[256],
	_In_ unsigned int _index = 0,
	_In_ unsigned int _parent = 0);

//--------------------------------------------------------------------------------------
// D3D10_Factory impl
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Factory, void, clear)(
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
	// �ߺ� �˻�
	auto iter = g_D3D10Texture_Container.find(_directory);

	if (iter != g_D3D10Texture_Container.end())
	{
		// �̹� �ִ� ���
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

		// �����Ͱ� ���� ���
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
	// �ߺ� �˻�
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
	_mesh.materials.resize(1);
	_mesh.meshs.resize(1);

	D3D10MY_MESH & refmesh =
		_mesh.meshs[0];

	HRESULT hr = E_FAIL;

	{
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

		D3D10_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vBox[0];

		D3D10MY_VERTEXBUFFER & refvb =
			refmesh.vetexbuffer;

		IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateBuffer(
			&vBufferDesc,
			&vinitData,
			&refvb.vertexbuffer))
		{
			return hr;
		}

		refvb.numOfVertices = sizeof(vBox) / sizeof(D3D10_SkyBoxFormat);
		refvb.vertexbuffers_Strides = sizeof(D3D10_SkyBoxFormat);
		refvb.vertexbuffers_Offsets = 0;
	}

	{
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

		D3D10_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &iBox[0];

		D3D10MY_INDEXBUFFER & refib =
			refmesh.indexbuffer;

		IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateBuffer(
			&iBufferDesc,
			&iinitData,
			&refib.indexbuffer))
		{
			return hr;
		}

		refib.indexType = DXGI_FORMAT_R16_UINT;
		refib.numOfindices = sizeof(iBox) / sizeof(unsigned short);
	}

	{
		refmesh.subsets.resize(1);
		D3D10MY_RENDER_DESC & refdesc =
			refmesh.subsets[0];

		refdesc.material_id = 0;
		refdesc.indexCount = 36;
		refdesc.indexStart = 0;
		refdesc.vertexbufferCount = 0;
		refdesc.vertexbufferStart = 0;
		refdesc.primitiveType = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Factory, create_MeshFromFile)(
	_Out_ D3D10_Mesh & _mesh,
	_In_ const wchar_t * _filePath,
	_In_ const wchar_t * _fileName,
	_Out_ D3D10_Animation * _animation)
{
	wchar_t wtoa[256];
	ZeroMemory(wtoa, sizeof(wtoa));

	if (_filePath)
	{
		wcscat_s<256>(wtoa, _filePath);
	}

	const unsigned int nameOffset = wcslen(wtoa);
	const unsigned int bufferSize = 256 - nameOffset;
	wcscat_s(&wtoa[nameOffset], bufferSize, _fileName);

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

	{
		_mesh.meshPath = wtoa;
	}

	HRESULT hr;

	if (_animation)
	{
		std::list<aiNode *> bones;
		sort_BonesFromAiNode(scene->mRootNode, bones);

		_animation->bones.resize(bones.size());
		_animation->animations.resize(scene->mNumAnimations);

		{
			wchar_t buffer[256];
			build_BonesFromAiNode(
				_animation->bones,
				bones, buffer, 0);

			auto begin = _animation->bones.begin();
			auto end = _animation->bones.end();

			while (begin != end)
			{
				_animation->bonePath[begin->name] = begin->id;
				++begin;
			}
		}

		{
			wchar_t buffer[256];
			for (unsigned int aindex = 0; aindex < scene->mNumAnimations; ++aindex)
			{
				const aiAnimation & animation = *scene->mAnimations[aindex];
				D3D10MY_ANIMATION & refanimation = _animation->animations[aindex];
				{
					refanimation.animID = aindex;

					mbstowcs_s<256>(nullptr, buffer, animation.mName.C_Str(), sizeof(buffer));
					refanimation.name = buffer;

					refanimation.boneKeyFrames.resize(animation.mNumChannels);
					refanimation.tickPerSecond = animation.mTicksPerSecond;
					refanimation.duration = animation.mDuration;
				}

				for (unsigned int cindex = 0; cindex < animation.mNumChannels; ++cindex)
				{
					const aiNodeAnim & anim = *animation.mChannels[cindex];
					D3D10MY_BONE_KEYFRAME & keyFrame = refanimation.boneKeyFrames[cindex];
					{
						mbstowcs_s<256>(nullptr, buffer, anim.mNodeName.C_Str(), sizeof(buffer));
						keyFrame.boneID = _animation->bonePath[buffer];

						keyFrame.postFrameMoveHint = anim.mPostState;
						keyFrame.preFrameMoveHint = anim.mPreState;

						keyFrame.positionKeyFrame = std::vector<aiVectorKey>(
							&anim.mPositionKeys[0],
							&anim.mPositionKeys[anim.mNumPositionKeys]);

						keyFrame.rotationKeyFrame = std::vector<aiQuatKey>(
							&anim.mRotationKeys[0],
							&anim.mRotationKeys[anim.mNumRotationKeys]);

						keyFrame.scaleKeyFrame = std::vector<aiVectorKey>(
							&anim.mScalingKeys[0],
							&anim.mScalingKeys[anim.mNumScalingKeys]);
					}
				}
			}

			{
				auto begin = _animation->animations.begin();
				auto end = _animation->animations.end();

				while (begin != end)
				{
					_animation->animationPath[begin->name] = begin->animID;
					++begin;
				}
			}
		}
	}

	{
		_mesh.materials.resize(scene->mNumMaterials ? scene->mNumMaterials : 1);
		_mesh.meshs.resize(scene->mNumMeshes);
	}

	for (unsigned int mindex = 0; mindex < scene->mNumMaterials; ++mindex)
	{
		const aiMaterial & matl = *scene->mMaterials[mindex];
		D3D10MY_MATERIAL & refmaterial = _mesh.materials[mindex];
		{
			aiString aiPath;
			const unsigned int ndt = matl.GetTextureCount(aiTextureType_DIFFUSE);
			if (ndt)
			{
				// 2�� �̻��� ��ǻ�� �ؽ�ó�� �޾Ƶ��� �� ����
				if (AI_SUCCESS == matl.GetTexture(aiTextureType_DIFFUSE, 0, &aiPath))
				{
					mbstowcs_s(nullptr, &wtoa[nameOffset], bufferSize, aiPath.C_Str(), bufferSize);
					D3DX10CreateShaderResourceViewFromFile(
						framework::g_Framework_Device.d3d10Device,
						wtoa, nullptr, nullptr, &refmaterial.diffuseRV, nullptr);
				}
			}

			const unsigned int nnt = matl.GetTextureCount(aiTextureType_NORMALS);
			if (nnt)
			{
				// 2�� �̻��� �븻 �ؽ�ó�� �޾Ƶ��� �� ����
				if (AI_SUCCESS == matl.GetTexture(aiTextureType_NORMALS, 0, &aiPath))
				{
					mbstowcs_s(nullptr, &wtoa[nameOffset], bufferSize, aiPath.C_Str(), bufferSize);
					D3DX10CreateShaderResourceViewFromFile(
						framework::g_Framework_Device.d3d10Device,
						wtoa, nullptr, nullptr, &refmaterial.normalRV, nullptr);
				}
			}

			const unsigned int nst = matl.GetTextureCount(aiTextureType_SPECULAR);
			if (nnt)
			{
				// 2�� �̻��� ����ŧ�� �ؽ�ó�� �޾Ƶ��� �� ����
				if (AI_SUCCESS == matl.GetTexture(aiTextureType_SPECULAR, 0, &aiPath))
				{
					mbstowcs_s(nullptr, &wtoa[nameOffset], bufferSize, aiPath.C_Str(), bufferSize);
					D3DX10CreateShaderResourceViewFromFile(
						framework::g_Framework_Device.d3d10Device,
						wtoa, nullptr, nullptr, &refmaterial.specularRV, nullptr);
				}
			}
		}

		{
			aiColor4D material;
			if (AI_FAILURE == aiGetMaterialColor(&matl, AI_MATKEY_COLOR_DIFFUSE, &material))
			{
				material = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
			}

			refmaterial.diffuse = { material.r, material.g, material.b, material.a };

			if (AI_FAILURE == aiGetMaterialColor(&matl, AI_MATKEY_COLOR_AMBIENT, &material))
			{
				material = aiColor4D(0.2f, 0.2f, 0.2f, 1.f);
			}

			refmaterial.ambient = { material.r, material.g, material.b, material.a };

			if (AI_FAILURE == aiGetMaterialColor(&matl, AI_MATKEY_COLOR_SPECULAR, &material))
			{
				material = aiColor4D(0.f, 0.f, 0.f, 0.f);
			}

			refmaterial.specular = { material.r, material.g, material.b, material.a };

			if (AI_FAILURE == aiGetMaterialColor(&matl, AI_MATKEY_COLOR_EMISSIVE, &material))
			{
				material = aiColor4D(0.f, 0.f, 0.f, 0.f);
			}

			refmaterial.emissive = { material.r, material.g, material.b, material.a };

		}

		{
			unsigned int max; float shininess, strength;

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

			refmaterial.shininess = shininess;
		}
	}

	if (scene->mNumMaterials == 0)
	{
		_mesh.materials[0].diffuse = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
		_mesh.materials[0].ambient = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.f);
		_mesh.materials[0].specular = D3DXVECTOR4(0.f, 0.f, 0.f, 0.f);
		_mesh.materials[0].emissive = D3DXVECTOR4(0.f, 0.f, 0.f, 0.f);
		_mesh.materials[0].shininess = 0.0f;
	}

	for (unsigned int index = 0; index < scene->mNumMeshes; ++index)
	{
		const aiMesh & mesh = *scene->mMeshes[index];
		D3D10MY_MESH & refmesh = _mesh.meshs[index];

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

		unsigned int offsetOrSumOfindices = 0;
		{
			std::vector<unsigned int> indices(mesh.mNumFaces * faceSize);
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

			D3D10_SUBRESOURCE_DATA iinitData;
			iinitData.pSysMem = &indices[0];

			D3D10MY_INDEXBUFFER & refib =
				refmesh.indexbuffer;

			IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateBuffer(
				&iBufferDesc,
				&iinitData,
				&refib.indexbuffer))
			{
				return hr;
			}

			refib.indexType = DXGI_FORMAT_R32_UINT;
			refib.numOfindices = offsetOrSumOfindices;
		}

		{
			std::vector<D3D10_SkinnedFormat> vbuffer(mesh.mNumVertices);
			D3D10_SkinnedFormat * formatBuffer = &vbuffer[0];

			{
				ZeroMemory(formatBuffer, mesh.mNumVertices * sizeof(D3D10_SkinnedFormat));
			}

			for (unsigned int vindex = 0; vindex < mesh.mNumVertices; ++vindex)
			{
				D3D10_SkinnedFormat & format = formatBuffer[vindex];
				memcpy(&format.pos, &mesh.mVertices[vindex], sizeof(D3DXVECTOR3));

				if (mesh.mNormals)
				{
					memcpy(&format.norm, &mesh.mNormals[vindex], sizeof(D3DXVECTOR3));
				}

				if (mesh.mTextureCoords[0])
				{
					memcpy(&format.tex, &mesh.mTextureCoords[0][vindex], sizeof(D3DXVECTOR2));
				}

				if (mesh.mColors[0])
				{
					memcpy(&format.color, &mesh.mColors[0][vindex], sizeof(D3DXVECTOR4));
				}
			}

			if (_animation)
			{
				refmesh.boneNode.resize(mesh.mNumBones);
				for (unsigned int bindex = 0; bindex < mesh.mNumBones; ++bindex)
				{
					const aiBone & bone = *mesh.mBones[bindex];

					unsigned int nodeBindex = 0;
					{
						mbstowcs_s<256>(nullptr, wtoa, bone.mName.C_Str(), sizeof(wtoa));
						nodeBindex = _animation->bonePath[wtoa];

						refmesh.boneNode[bindex] = nodeBindex;

						// �ߺ��� �־ �����ϰ� �ִ´�.
						aiMatrix4x4 m = bone.mOffsetMatrix;

						_animation->bones[nodeBindex].mOwn = D3DXMATRIX(
							m.a1, m.a2, m.a3, m.a4,
							m.b1, m.b2, m.b3, m.b4,
							m.c1, m.c2, m.c3, m.c4,
							m.d1, m.d2, m.d3, m.d4);
					}

					for (unsigned int windex = 0; windex < bone.mNumWeights; ++windex)
					{
						const aiVertexWeight & wight = bone.mWeights[windex];
						D3D10_SkinnedFormat & format = formatBuffer[wight.mVertexId];

						for (unsigned int i = 0; i < 4; ++i)
						{
							if (format.bweight[i])
							{
								continue;
							}
							else
							{
								// bones[ refmesh.boneNode[format.bindexs] ]
								format.bindexs[i] = bindex;
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

			D3D10_SUBRESOURCE_DATA vinitData;
			vinitData.pSysMem = formatBuffer;

			D3D10MY_VERTEXBUFFER & refvb =
				refmesh.vetexbuffer;

			IF_FAILED(hr = framework::g_Framework_Device.d3d10Device->CreateBuffer(
				&vBufferDesc,
				&vinitData,
				&refvb.vertexbuffer))
			{
				return hr;
			}

			refvb.numOfVertices = mesh.mNumVertices;
			refvb.vertexbuffers_Strides = sizeof(D3D10_SkinnedFormat);
			refvb.vertexbuffers_Offsets = 0;
		}

		{
			refmesh.subsets.resize(1);
			D3D10MY_RENDER_DESC & refdesc =
				refmesh.subsets[0];

			refdesc.material_id = mesh.mMaterialIndex;
			refdesc.indexCount = offsetOrSumOfindices;
			refdesc.indexStart = 0;
			refdesc.vertexbufferCount = 0;
			refdesc.vertexbufferStart = 0;
			refdesc.primitiveType = topology;
		}
	}

	g_importer.FreeScene();

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Grobal impl function
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, sort_BonesFromAiNode)(
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
		sort_BonesFromAiNode(_node->mChildren[index], _list);
	}

	return;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(unsigned int, build_BonesFromAiNode)(
	_Out_ std::vector<D3D10MY_BONE> & _bones,
	_In_ std::list<aiNode *> & _list,
	_In_ wchar_t(&_buffer)[256],
	_In_ unsigned int _index,
	_In_ unsigned int _parent)
{
	const aiNode & node = *_list.front();
	_list.pop_front();

	unsigned int length = 0;
	for (unsigned int cindex = 0; cindex < node.mNumChildren; ++cindex)
	{
		length += build_BonesFromAiNode(
			_bones, _list, _buffer,
			_bones.size() - _list.size(),
			_index);
	}

	D3D10MY_BONE & refbone = _bones[_index];
	refbone.id = _index;
	refbone.parent = _parent;
	refbone.length = length;

	aiMatrix4x4 m = node.mTransformation;
	m.Transpose();

	refbone.mRelation = D3DXMATRIX(
		m.a1, m.a2, m.a3, m.a4,
		m.b1, m.b2, m.b3, m.b4,
		m.c1, m.c2, m.c3, m.c4,
		m.d1, m.d2, m.d3, m.d4);

	mbstowcs_s<256>(nullptr, _buffer, node.mName.C_Str(), sizeof(_buffer));
	refbone.name = _buffer;

	return length ? length + 1 : 1;
}

//--------------------------------------------------------------------------------------
// open variable 
//--------------------------------------------------------------------------------------

// ���� : 
D3D10_Factory hsdk::direct3d::g_D3D10_Factory;