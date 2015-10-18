#include <hsdk/win/frame/direct3d/d3d10_master.h>
#include <hash_map>
#include <string>



using namespace hsdk;
using namespace direct3d;


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------

DECL_STRUCT(D3D10MY_TEXTURE)
{

	// 설명 : 
	D3DX10_IMAGE_LOAD_INFO info;

	// 설명 : 
	AutoRelease<ID3D10ShaderResourceView> texture;

};

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------

// 설명 : 
std::hash_map<std::wstring, D3D10MY_TEXTURE> g_Manager_Texture_Container;

//--------------------------------------------------------------------------------------
// D3D10_Master impl
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Master, void, destroy_Master)(
	/* [x] */ void)
{
	g_Manager_Texture_Container.clear();

	destroy();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Master, get_Texture)(
	/* [w] */ ID3D10ShaderResourceView ** _texture,
	/* [r] */ const wchar_t * _directory,
	/* [r] */ D3DX10_IMAGE_LOAD_INFO * _info)
{
	// 중복 검사
	auto iter = g_Manager_Texture_Container.find(_directory);

	if (iter != g_Manager_Texture_Container.end())
	{
		// 이미 있는 경우
		(*_texture) = iter->second.texture;

		if (_info)
		{
			(*_info) = iter->second.info;
		}
	}
	else
	{
		auto element = g_Manager_Texture_Container[_directory];

		// 데이터가 없는 경우
		HRESULT hr;
		IF_FAILED(hr = D3DX10CreateShaderResourceViewFromFile(
			get_Device()->d3d10Device,
			_directory,
			&element.info,
			NULL,
			&element.texture,
			NULL))
		{
			g_Manager_Texture_Container.erase(_directory);

			return hr;
		}

		// added one count from AutoRelease
		(*_texture) = element.texture;

		if (_info)
		{
			(*_info) = element.info;
		}

	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Master, const D3DX10_IMAGE_LOAD_INFO *, get_TextureLoadinfo)(
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
	_mesh.destroy();

	HRESULT hr;
	IF_FAILED(hr = _mesh.setup0(
		get_Device()->d3d10Device, 6, 1))
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
	/* [r] */ const wchar_t * _szFileName,
	/* [r] */ bool _createAdjacencyIndices)
{
	HRESULT hr = S_OK;

	wchar_t meshPath[MAX_PATH];

	// Open the file
	HANDLE hFile = CreateFile(
		meshPath,
		FILE_READ_DATA,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return Direct3DERR_MEDIANOTFOUND;
	}

	// Change the path to just the directory
	wchar_t * lastBSlash = wcsrchr(meshPath, L'\\');
	if (lastBSlash)
	{
		(*(lastBSlash + 1)) = L'\0';
	}
	else
	{
		(*meshPath) = L'\0';
	}

	// Get the file size
	LARGE_INTEGER fileSize;
	GetFileSizeEx(hFile, &fileSize);

	unsigned int cBytes = fileSize.LowPart;

	// Allocate memory
	std::vector<unsigned char> meshData(cBytes);

	// Read in the file
	DWORD dwBytesRead;
	IF_FALSE(ReadFile(
		hFile,
		&meshData[0],
		cBytes,
		&dwBytesRead,
		nullptr))
	{
		hr = E_FAIL;
	}

	CloseHandle(hFile);

	IF_SUCCEEDED(hr)
	{
		if (FAILED(hr = create_MeshFromMemory(
			_mesh,
			meshPath,
			&meshData[0],
			meshData.size(),
			_createAdjacencyIndices)))
		{
			_mesh.destroy();
		}
		else
		{
			_mesh.userSet_MeshPath(meshPath);
		}
	}

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Master, create_MeshFromMemory)(
	/* [w] */ D3D10_Mesh & _mesh,
	/* [r] */ const wchar_t * _resourcePath,
	/* [r] */ unsigned char * _data,
	/* [r] */ unsigned int _dataBytes,
	/* [r] */ bool _createAdjacencyIndices)
{
	_mesh.destroy();

	D3DXVECTOR3 lower;
	D3DXVECTOR3 upper;

	// Pointer fixup
	D3D10MESH_DESC * refMeshDescs =
		(D3D10MESH_DESC *)_data;

	D3D10MESH_VERTEX_BUFFER_DESC * refVertexBufferArray =
		(D3D10MESH_VERTEX_BUFFER_DESC *)(_data + refMeshDescs->vertexStreamHeadersOffset);

	D3D10MESH_INDEX_BUFFER_DESC * refindexBufferArray =
		(D3D10MESH_INDEX_BUFFER_DESC *)(_data + refMeshDescs->indexStreamHeadersOffset);

	D3D10MESH_MESH * refMeshArray =
		(D3D10MESH_MESH*)(_data + refMeshDescs->meshDataOffset);

	D3D10MESH_SUBSET * refSubsetArray =
		(D3D10MESH_SUBSET*)(_data + refMeshDescs->subsetDataOffset);

	D3D10MESH_FRAME * refFrameArray =
		(D3D10MESH_FRAME*)(_data + refMeshDescs->frameDataOffset);

	D3D10MESH_MATERIAL * refMaterialArray =
		(D3D10MESH_MATERIAL*)(_data + refMeshDescs->materialDataOffset);

	// Setup subsets
	const unsigned int numMeshes =
		refMeshDescs->numMeshes;

	for (unsigned int i = 0; i < numMeshes; ++i)
	{
		refMeshArray[i].subsets =
			(unsigned int *)(_data + refMeshArray[i].subsetOffset);

		refMeshArray[i].frame_influences =
			(unsigned int *)(_data + refMeshArray[i].frame_influenceOffset);
	}

	// error condition
	if (refMeshDescs->version != SDKMESH_FILE_VERSION)
	{
		return E_NOINTERFACE;
	}

	// Setup buffer data pointer
	unsigned char * refBufferData =
		_data + refMeshDescs->headerSize + refMeshDescs->nonBufferDataSize;

	// Get the start of the buffer data
	unsigned long long BufferDataStart =
		refMeshDescs->headerSize + refMeshDescs->nonBufferDataSize;

	HRESULT hr = E_FAIL;

	IF_FAILED(hr = _mesh.setup0(
		get_Device()->d3d10Device,
		refMeshDescs->numMaterials,
		refMeshDescs->numMeshes))
	{
		return hr;
	}

	// Create Materials
	for (unsigned int m = 0; m < refMeshDescs->numMaterials; ++m)
	{
		D3D10MESH_MATERIAL & refMaterial = refMaterialArray[m];

		// Load Materials
		_mesh.setup1_Material(
			m, 0, refMaterial.diffuse);

		_mesh.setup1_Material(
			m, 1, refMaterial.ambient);

		_mesh.setup1_Material(
			m, 2, refMaterial.specular);

		_mesh.setup1_Material(
			m, 3, refMaterial.emissive);

		// Load Textures
		_mesh.setup1_Texture(
			refMaterial.szDiffuseTexture, m, 0);

		_mesh.setup1_Texture(
			refMaterial.szNormalTexture, m, 1);

		_mesh.setup1_Texture(
			refMaterial.szSpecularTexture, m, 2);
	}

	for (unsigned int m = 0; m < refMeshDescs->numMeshes; ++m)
	{
		D3D10MESH_MESH & refMesh = refMeshArray[m];

		IF_SUCCEEDED(_mesh.setup1_Mesh(
			m, refMesh.numSubsets, refMesh.numVertexBuffers))
		{
			for (unsigned int s = 0; s < refMesh.numSubsets; ++s)
			{
				D3D10MESH_SUBSET & subset =
					refSubsetArray[refMesh.subsets[s]];

				IF_FAILED(hr = _mesh.setup2_RenderDesc(
					m, s,
					subset.material_id,
					subset.indexStart,
					subset.indexCount,
					subset.vertexStart,
					subset.vertexCount,
					convert_PrimitiveType((D3D10MESH_PRIMITIVE_TYPE)subset.primitiveType)))
				{
					return hr;
				}
			}

			// Create VBs
			for (unsigned int v = 0; v < refMesh.numVertexBuffers; ++v)
			{
				D3D10MESH_VERTEX_BUFFER_DESC & desc =
					refVertexBufferArray[refMesh.vertexBuffers[v]];

				//Vertex Buffer
				D3D10_BUFFER_DESC bufferDesc;
				bufferDesc.ByteWidth = (UINT)(desc.sizeBytes);
				bufferDesc.Usage = D3D10_USAGE_DEFAULT;
				bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
				bufferDesc.CPUAccessFlags = 0;
				bufferDesc.MiscFlags = 0;

				IF_FAILED(_mesh.setup2_Vertexbuffer(
					m, v,
					bufferDesc,
					(refBufferData + (desc.dataOffset - BufferDataStart)),
					desc.strideBytes,
					0,
					desc.numVertices))
				{
					return hr;
				}
			}

			// Create IBs
			D3D10MESH_INDEX_BUFFER_DESC & desc =
				refindexBufferArray[refMesh.indexBuffer];

			// Index Buffer
			D3D10_BUFFER_DESC bufferDesc;
			bufferDesc.ByteWidth = (UINT)(desc.sizeBytes);
			bufferDesc.Usage = D3D10_USAGE_DEFAULT;
			bufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;

			IF_FAILED(hr = _mesh.setup2_indexbuffer(
				m, bufferDesc,
				(refBufferData + (desc.dataOffset - BufferDataStart)),
				desc.indexType == IT_16BIT ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT,
				desc.numindices))
			{
				return hr;
			}
		}
	}

	// Create Adjacency Indices
	if (_createAdjacencyIndices)
	{
		/*
		create_Adjacency_indices(
		_d3d10Device,
		0.001f,
		refBufferData - BufferDataStart);*/
	}

	hr = S_OK;

	D3D10MESH_MESH * currentMesh =
		&refMeshArray[0];

	int tris = 0;
	for (unsigned int meshi = 0; meshi < numMeshes; ++meshi)
	{
		lower.x = FLT_MAX; lower.y = FLT_MAX; lower.z = FLT_MAX;
		upper.x = -FLT_MAX; upper.y = -FLT_MAX; upper.z = -FLT_MAX;

		currentMesh = (D3D10MESH_MESH *)&refMeshArray[meshi];

		INT indsize;
		if (refindexBufferArray[currentMesh->indexBuffer].indexType == IT_16BIT)
		{
			indsize = 2;
		}
		else
		{
			indsize = 4;
		}

		const unsigned int numSubsets =
			currentMesh->numSubsets;

		for (unsigned int subseti = 0; subseti < numSubsets; ++subseti)
		{
			D3D10MESH_SUBSET * pSubset = &refSubsetArray[currentMesh->subsets[subseti]];

			const unsigned int indexCount = (unsigned int)pSubset->indexCount;
			const unsigned int indexStart = (unsigned int)pSubset->indexStart;

			unsigned int * ind =
				(unsigned int *)(refBufferData + (refindexBufferArray[currentMesh->indexBuffer].dataOffset - BufferDataStart));

			float * verts =
				(float *)(refBufferData + (refVertexBufferArray[currentMesh->vertexBuffers[0]].dataOffset - BufferDataStart));

			unsigned int stride =
				(unsigned int)refVertexBufferArray[currentMesh->vertexBuffers[0]].strideBytes;

			stride /= 4;
			for (unsigned int vertind = indexStart; vertind < indexStart + indexCount; ++vertind)
			{
				unsigned int current_ind = 0;
				if (indsize == 2)
				{
					unsigned int ind_div2 = vertind / 2;
					current_ind = ind[ind_div2];

					if (vertind % 2 == 0)
					{
						current_ind = current_ind << 16;
						current_ind = current_ind >> 16;
					}
					else
					{
						current_ind = current_ind >> 16;
					}
				}
				else
				{
					current_ind = ind[vertind];
				}

				tris++;
				D3DXVECTOR3 * pt =
					(D3DXVECTOR3 *)&(verts[stride * current_ind]);

				if (pt->x < lower.x)
				{
					lower.x = pt->x;
				}

				if (pt->y < lower.y)
				{
					lower.y = pt->y;
				}

				if (pt->z < lower.z)
				{
					lower.z = pt->z;
				}

				if (pt->x > upper.x)
				{
					upper.x = pt->x;
				}

				if (pt->y > upper.y)
				{
					upper.y = pt->y;
				}

				if (pt->z > upper.z)
				{
					upper.z = pt->z;
				}
			}
		}

		D3DXVECTOR3 half = upper - lower;
		half *= 0.5f;

		_mesh.userSet_MeshBoundingBox(
			meshi,
			lower + half,
			half);
	}

	return hr;
}