#include <hsdk/win/frame/direct3d/d3d10_manager.h>



using namespace hsdk;
using namespace direct3d;


//--------------------------------------------------------------------------------------
// Grobal function declare
//--------------------------------------------------------------------------------------

/*
설명 : Tries to find the location of a SDK media file cchDest is the size in WCHARs of strDestPath.
Be careful not to pass in sizeof(strDest) on UNICODE builds.
*/
template<unsigned int T> DECL_FUNC(find_Media_FileCch)(
	/* [w] */ wchar_t(&_strDestPath)[T],
	/* [r] */ int _cchDest,
	/* [r] */ const wchar_t * _strFilename);

/*
설명: Search parent directories starting at strStartAt, and appending strLeafName at each parent directory.
It stops at the root directory.
*/
template<unsigned int T> DECL_FUNC_T(bool, find_Media_SearchParentDirs)(
	/* [w] */ wchar_t(&_strDestPath)[T],
	/* [r] */ int _cchSearch,
	/* [r] */ wchar_t * _strStartAt,
	/* [r] */ wchar_t * _strLeafName);

//--------------------------------------------------------------------------------------
// D3D10_Manager impl
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Manager, create_MeshFromFile)(
	/* [w] */ D3D10_Mesh & _mesh,
	/* [r] */ const wchar_t * _szFileName,
	/* [r] */ bool _createAdjacencyIndices)
{
	HRESULT hr = S_OK;

	wchar_t meshPath[MAX_PATH];

	// Find the path for the file
	IF_FAILED(hr = find_Media_FileCch(
		meshPath,
		sizeof(meshPath) / sizeof(wchar_t),
		_szFileName))
	{
		return hr;
	}

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
CLASS_IMPL_FUNC(D3D10_Manager, create_MeshFromMemory)(
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

	IF_FAILED(hr = _mesh.setup(
		get_D3D10_Device(),
		refMeshDescs->numMaterials,
		refMeshDescs->numMeshes,
		refMeshDescs->numFrames))
	{
		return hr;
	}


	// Create Materials

	for (unsigned int m = 0; m < refMeshDescs->numMaterials; ++m)
	{
		D3D10MESH_MATERIAL & refMaterial = refMaterialArray[m];

		// Load Materials

		_mesh.setup_Material(
			m, 0, refMaterial.diffuse);

		_mesh.setup_Material(
			m, 1, refMaterial.ambient);

		_mesh.setup_Material(
			m, 2, refMaterial.specular);

		_mesh.setup_Material(
			m, 3, refMaterial.emissive);

		// Load Textures

		_mesh.setup_Texture(
			refMaterial.szDiffuseTexture, m, 0);

		_mesh.setup_Texture(
			refMaterial.szNormalTexture, m, 1);

		_mesh.setup_Texture(
			refMaterial.szSpecularTexture, m, 2);
	}

	for (unsigned int m = 0; m < refMeshDescs->numMeshes; ++m)
	{
		D3D10MESH_MESH & refMesh = refMeshArray[m];

		IF_SUCCEEDED(_mesh.setup_Mesh(
			m, refMesh.numSubsets, refMesh.numVertexBuffers))
		{
			for (unsigned int s = 0; s < refMesh.numSubsets; ++s)
			{
				D3D10MESH_SUBSET & subset =
					refSubsetArray[refMesh.subsets[s]];

				IF_FAILED(hr = _mesh.setup_RenderDesc(
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

				IF_FAILED(_mesh.setup_Vertexbuffer(
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

			IF_FAILED(hr = _mesh.setup_indexbuffer(
				m, bufferDesc,
				(refBufferData + (desc.dataOffset - BufferDataStart)),
				desc.indexType == IT_16BIT ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT),
				desc.numindices)
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
			//&m_pSubsetArray[ currentMesh->pSubsets[subset] ];
			D3D10MESH_SUBSET * pSubset = &refSubsetArray[currentMesh->subsets[subseti]];

			const unsigned int indexCount = (unsigned int)pSubset->indexCount;
			const unsigned int indexStart = (unsigned int)pSubset->indexStart;

			/*if( bAdjacent )
			{
			IndexCount *= 2;
			IndexStart *= 2;
			}*/

			//BYTE * pIndices = NULL;
			//m_ppIndices[i]

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

				//BYTE** m_ppVertices;
				//BYTE** m_ppIndices;
			}

			//pd3dDeviceContext->DrawIndexed( IndexCount, IndexStart, VertexStart );
		}

		D3DXVECTOR3 half = upper - lower;
		half *= 0.5f;

		_mesh.userSet_MeshBoundingBox(
			meshi,
			lower + half,
			half);
	}
	// Update 

Error:

	return hr;
}

//--------------------------------------------------------------------------------------
// Grobal function impl
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
template<unsigned int T> IMPL_FUNC(find_MediaFileCch)(
	/* [w] */ wchar_t(&_strDestPath)[T],
	/* [r] */ int _cchDest,
	/* [r] */ const wchar_t * _strFilename)
{
	bool bFound;
	wchar_t strSearchFor[MAX_PATH];

	if (nullptr == _strFilename || _strFilename[0] == 0 || nullptr == _strDestPath || _cchDest < 10)
	{
		return E_INVALIDARG;
	}

	// Get the exe name, and exe path
	wchar_t strExePath[MAX_PATH] = { 0 };
	wchar_t strExeName[MAX_PATH] = { 0 };
	wchar_t * strLastSlash = nullptr;

	GetModuleFileName(nullptr, strExePath, MAX_PATH);

	strExePath[MAX_PATH - 1] = 0;
	strLastSlash = wcsrchr(strExePath, TEXT('\\'));
	if (strLastSlash)
	{
		wcscpy_s(strExeName, MAX_PATH, &strLastSlash[1]);

		// Chop the exe name from the exe path
		*strLastSlash = 0;

		// Chop the .exe from the exe name
		strLastSlash = wcsrchr(strExeName, TEXT('.'));
		if (strLastSlash)
			*strLastSlash = 0;
	}

	// Typical directories:
	//      .\
																    //      ..\
																    //      ..\..\
																    //      %EXE_DIR%\
																    //      %EXE_DIR%\..\
																    //      %EXE_DIR%\..\..\
																    //      %EXE_DIR%\..\%EXE_NAME%
	//      %EXE_DIR%\..\..\%EXE_NAME%

	// Typical directory search
	bFound = find_Media_SearchParentDirs(
		_strDestPath,
		_cchDest,
		_strFilename,
		strExePath,
		strExeName);

	if (bFound)
		return S_OK;

	// Typical directory search again, but also look in a subdir called "\media\" 
	swprintf_s(strSearchFor, MAX_PATH, L"media\\%s", _strFilename);
	bFound = find_Media_SearchParentDirs(
		strDestPath,
		cchDest,
		strSearchFor,
		strExePath,
		strExeName);

	if (bFound)
		return S_OK;

	WCHAR strLeafName[MAX_PATH] = { 0 };

	// Search all parent directories starting at .\ and using strFilename as the leaf name
	wcscpy_s(strLeafName, MAX_PATH, _strFilename);
	bFound = find_Media_SearchParentDirs(
		strDestPath,
		cchDest,
		L".",
		strLeafName);

	if (bFound)
		return S_OK;

	// Search all parent directories starting at the exe's dir and using strFilename as the leaf name
	bFound = find_Media_SearchParentDirs(strDestPath, cchDest, strExePath, strLeafName);
	if (bFound)
		return S_OK;

	// Search all parent directories starting at .\ and using "media\strFilename" as the leaf name
	swprintf_s(strLeafName, MAX_PATH, L"media\\%s", _strFilename);
	bFound = find_Media_SearchParentDirs(
		strDestPath,
		cchDest,
		L".",
		strLeafName);

	if (bFound)
		return S_OK;

	// Search all parent directories starting at the exe's dir and using "media\strFilename" as the leaf name
	bFound = find_Media_SearchParentDirs(strDestPath, cchDest, strExePath, strLeafName);
	if (bFound)
		return S_OK;

	// On failure, return the file as the path but also return an error code
	wcscpy_s(strDestPath, cchDest, _strFilename);

	return Direct3DERR_MEDIANOTFOUND;
}

//--------------------------------------------------------------------------------------
template<unsigned int T> IMPL_FUNC_T(bool, find_Media_SearchParentDirs)(
	/* [w] */ wchar_t(&_strDestPath)[T],
	/* [r] */ int _cchSearch,
	/* [r] */ const wchar_t * _strStartAt,
	/* [r] */ const wchar_t * _strLeafName)
{
	wchar_t strFullPath[MAX_PATH] = { 0 };
	wchar_t strFullFileName[MAX_PATH] = { 0 };
	wchar_t strSearch[MAX_PATH] = { 0 };
	wchar_t* strFilePart = NULL;

	GetFullPathName(_strStartAt, MAX_PATH, strFullPath, &strFilePart);
	if (strFilePart == NULL)
		return false;

	while (strFilePart != NULL && *strFilePart != '\0')
	{
		swprintf_s(strFullFileName, MAX_PATH, L"%s\\%s", strFullPath, _strLeafName);
		if (GetFileAttributes(strFullFileName) != 0xFFFFFFFF)
		{
			wcscpy_s(_strSearchPath, _cchSearch, &strFullFileName[0]);
			return true;
		}

		swprintf_s(strSearch, MAX_PATH, L"%s\\..", strFullPath);
		GetFullPathName(strSearch, MAX_PATH, strFullPath, &strFilePart);
	}

	return false;
}