#include <hsdk/win/frame/direct3d/d3d10_mesh.h>



using namespace hsdk;
using namespace direct3d;


//--------------------------------------------------------------------------------------
// D3D10_Mesh impl
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(D3D10_Mesh, D3D10_Mesh)(
	/* [r] */ CALLBACK_D3D10_CREATE_TEXTURE_FROMFILE _callback_Create_Texture_FromFile,
	/* [r] */ CALLBACK_D3D10_CREATE_VERTEXBUFFER _callback_Create_VertexBuffer,
	/* [r] */ CALLBACK_D3D10_CREATE_INDEXBUFFER _callback_Create_indexBuffer,
	/* [r] */ void * _userContext)
	: my_refCallback_Create_Texture_FromFile(_callback_Create_Texture_FromFile),
	my_refCallback_Create_VertexBuffer(_callback_Create_VertexBuffer),
	my_refCallback_Create_indexBuffer(_callback_Create_indexBuffer),
	my_refUserContext(_userContext),
	my_refD3D10Device(nullptr),
	my_MeshPath(L""),
	my_LoadLock(false)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(D3D10_Mesh, D3D10_Mesh)(void)
{
	destroy();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Mesh, void, destroy)(
	/* [x] */ void)
{
	my_MeshPath = L"";
	my_Meshs.clear();
	my_MeshPath.clear();
	my_LoadLock = false;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Mesh, setup)(
	/* [r] */ ID3D10Device * _device,
	/* [r] */ unsigned int _numOfMaterials,
	/* [r] */ unsigned int _numOfMeshs)
{
	if (my_Meshs.size())
	{
		return E_ACCESSDENIED;
	}

	my_refD3D10Device = _device;
	my_Meshs.resize(_numOfMeshs);
	my_Materials.resize(_numOfMaterials);

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Mesh, setup_Texture)(
	/* [r] */ const wchar_t * _path,
	/* [r] */ unsigned int _indexOfMaterial,
	/* [r] */ unsigned int _attribute)
{
	IF_FALSE(_indexOfMaterial < my_Materials.size())
	{
		return E_ACCESSDENIED;
	}

	D3D10MY_MATERIAL & material =
		my_Materials[_indexOfMaterial];

	if (my_refCallback_Create_Texture_FromFile)
	{
		switch (_attribute)
		{
		case 0:
			return my_refCallback_Create_Texture_FromFile(
				&material.diffuseRV,
				my_refD3D10Device,
				_path,
				my_refUserContext);
		case 1:

			return my_refCallback_Create_Texture_FromFile(
				&material.normalRV,
				my_refD3D10Device,
				_path,
				my_refUserContext);
		case 2:

			return my_refCallback_Create_Texture_FromFile(
				&material.specularRV,
				my_refD3D10Device,
				_path,
				my_refUserContext);
		default:
			return E_NOTIMPL;
		};
	}

	return E_FAIL;
}


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Mesh, setup_Material)(
	/* [r] */ unsigned int _indexOfMaterial,
	/* [r] */ unsigned int _attribute,
	/* [r] */ const D3DXVECTOR4 & _value)
{

	IF_FALSE(_indexOfMaterial < my_Materials.size())
	{
		return E_ACCESSDENIED;
	}

	D3D10MY_MATERIAL & material =
		my_Materials[_indexOfMaterial];

	switch (_attribute)
	{
	case 0:
		material.diffuse = _value;
		break;

	case 1:
		material.ambient = _value;
		break;

	case 2:
		material.specular = _value;
		break;

	case 3:
		material.emissive = _value;
		break;

	default:
		return E_NOTIMPL;
	};

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Mesh, setup_Mesh)(
	/* [r] */ unsigned int _indexOfMesh,
	/* [r] */ unsigned int _numOfRenderDescs,
	/* [r] */ unsigned int _numOfVertexBuffers)
{
	IF_FALSE(_indexOfMesh < my_Meshs.size())
	{
		return E_ACCESSDENIED;
	}

	D3D10MY_MESH & mesh =
		my_Meshs[_indexOfMesh];

	mesh.render_Descs.resize(_numOfRenderDescs);
	mesh.vertexbuffers_Strides.resize(_numOfVertexBuffers);
	mesh.vertexbuffers_Offsets.resize(_numOfVertexBuffers);
	mesh.vertexbuffers.resize(_numOfVertexBuffers);

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Mesh, setup_RenderDesc)(
	/* [r] */ unsigned int _indexOfMesh,
	/* [r] */ unsigned int _indexOfRenderDesc,
	/* [r] */ unsigned int _material_id,
	/* [r] */ unsigned int _indexStart,
	/* [r] */ unsigned int _indexCount,
	/* [r] */ unsigned int _vertexbufferStart,
	/* [r] */ unsigned int _vertexbufferCount,
	/* [r] */ D3D10_PRIMITIVE_TOPOLOGY _primitiveType)
{
	IF_FALSE(_indexOfMesh < my_Meshs.size())
	{
		return E_ACCESSDENIED;
	}

	IF_FALSE(_indexOfRenderDesc < my_Meshs[_indexOfMesh].render_Descs.size())
	{
		return E_ACCESSDENIED;
	}

	D3D10MY_RENDER_DESC & render_Desc =
		my_Meshs[_indexOfMesh].render_Descs[_indexOfRenderDesc];

	render_Desc.material_id = _material_id;
	render_Desc.indexStart = _indexStart;
	render_Desc.indexCount = _indexCount;
	render_Desc.vertexbufferStart = _vertexbufferStart;
	render_Desc.vertexbufferCount = _vertexbufferCount;
	render_Desc.primitiveType = _primitiveType;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Mesh, setup_Vertexbuffer)(
	/* [r] */ unsigned int _indexOfMesh,
	/* [r] */ unsigned int _indexOfBuffer,
	/* [r] */ const D3D10_BUFFER_DESC & _desc,
	/* [r] */ const void * _vertices,
	/* [r] */ unsigned int _stride,
	/* [r] */ unsigned int _offset,
	/* [r] */ unsigned int _numOfVertices)
{
	IF_FALSE(_indexOfMesh < my_Meshs.size())
	{
		return E_ACCESSDENIED;
	}

	IF_FALSE(_indexOfBuffer < my_Meshs[_indexOfMesh].vertexbuffers.size())
	{
		return E_ACCESSDENIED;
	}

	D3D10MY_MESH & mesh =
		my_Meshs[_indexOfMesh];

	AutoRelease<ID3D10Buffer> & vb =
		mesh.vertexbuffers[_indexOfBuffer];

	HRESULT hr = E_FAIL;

	if (my_refCallback_Create_VertexBuffer)
	{
		hr = my_refCallback_Create_VertexBuffer(
			&vb,
			my_refD3D10Device,
			_desc,
			_vertices,
			my_refUserContext);
	}
	else
	{
		D3D10_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = _vertices;

		hr = my_refD3D10Device->CreateBuffer(
			&_desc,
			&InitData,
			&vb);
	}

	IF_SUCCEEDED(hr)
	{
		mesh.numOfVertices = _numOfVertices;
		mesh.vertexbuffers_Strides[_indexOfBuffer] = _stride;
		mesh.vertexbuffers_Offsets[_indexOfBuffer] = _offset;
	}

	return hr;

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Mesh, setup_indexbuffer)(
	/* [r] */ unsigned int _indexOfMesh,
	/* [r] */ const D3D10_BUFFER_DESC & _desc,
	/* [r] */ const void * _indices,
	/* [r] */ DXGI_FORMAT _format,
	/* [r] */ unsigned int _numOfindices)
{
	IF_FALSE(_indexOfMesh < my_Meshs.size())
	{
		return E_ACCESSDENIED;
	}

	D3D10MY_INDEXBUFFER & ib =
		my_Meshs[_indexOfMesh].indexbuffer;

	HRESULT hr = E_FAIL;

	if (my_refCallback_Create_indexBuffer)
	{
		hr = my_refCallback_Create_indexBuffer(
			&ib.indexbuffer,
			my_refD3D10Device,
			_desc,
			_indices,
			my_refUserContext);
	}
	else
	{
		D3D10_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = _indices;

		hr = my_refD3D10Device->CreateBuffer(
			&_desc,
			&InitData,
			&ib.indexbuffer);
	}

	IF_SUCCEEDED(hr)
	{
		ib.indexType = _format;
		ib.numOfindices = _numOfindices;
	}

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Mesh, userSet_MeshPath)(
	/* [r] */ const wchar_t * _path)
{
	if (wcscmp(my_MeshPath.c_str(), L"") != 0)
	{
		return E_ACCESSDENIED;
	}

	my_MeshPath = _path;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Mesh, userSet_MeshBoundingBox)(
	/* [r] */ unsigned int _indexOfMesh,
	/* [r] */ const D3DXVECTOR3 & _center,
	/* [r] */ const D3DXVECTOR3 & _size)
{
	IF_FALSE(_indexOfMesh < my_Meshs.size())
	{
		return E_ACCESSDENIED;
	}

	D3D10MY_MESH & mesh =
		my_Meshs[_indexOfMesh];

	mesh.boundingBoxCenter = _center;
	mesh.boundingBoxSize = _size;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Mesh, userSet_MaterialName)(
	/* [r] */ unsigned int _indexOfMaterial,
	/* [r] */ const wchar_t * _name)
{
	IF_FALSE(_indexOfMaterial < my_Materials.size())
	{
		return E_ACCESSDENIED;
	}

	my_Materials[_indexOfMaterial].name = _name;

	return S_OK;
}

//--------------------------------------------------------------------------------------
//Helpers (general)
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_Mesh, loadLock)(
	/* [x] */ void)
{
	return InterlockedCompareExchange(&my_LoadLock, TRUE, FALSE) == TRUE ? E_ACCESSDENIED : S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Mesh, long, is_Load)(
	/* [x] */ void)const
{
	return my_LoadLock;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Mesh, const wchar_t *, get_MeshPath)(
	/* [x] */ void)const
{
	return my_MeshPath.c_str();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Mesh, unsigned int, get_NumMeshes)(
	/* [x] */ void)const
{
	return my_Meshs.size();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Mesh, unsigned int, get_NumMaterials)(
	/* [x] */ void)const
{
	return my_Materials.size();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Mesh, const D3D10MY_MATERIAL &, get_Material)(
	/* [r] */ unsigned int _indexOfMaterial)const
{
	return my_Materials[_indexOfMaterial];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Mesh, const D3D10MY_MESH &, get_Mesh)(
	/* [r] */ unsigned int _indexOfMesh)const
{
	return my_Meshs[_indexOfMesh];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Mesh, D3DXVECTOR3, get_Mesh_BBoxCenter)(
	/* [r] */ unsigned int _indexOfMesh)const
{
	return my_Meshs[_indexOfMesh].boundingBoxCenter;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_Mesh, D3DXVECTOR3, get_Mesh_BBoxSize)(
	/* [r] */ unsigned int _indexOfMesh)const
{
	return my_Meshs[_indexOfMesh].boundingBoxSize;
}