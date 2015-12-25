#include "gomain.h"



using namespace go;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(GameEngine, GameEngine)(void)
{
	HRESULT hr;

	// 어디에 쓰이지는 잘 모르겠음
	IF_FAILED(hr = my_PhysicsEngine.setup0_World(
		WORLD_SIZE,
		WORLD_SIZE,
		WORLD_SIZE,
		go::callback_CollisionResult))
	{
		throw hr;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(GameEngine, GameEngine)(void)
{
	reset();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, setup0_Terrain)(
	_In_ int _cellX,
	_In_ int _cellY,
	_In_ const float * _heightBuffer,
	_In_ unsigned int _size)
{
	HRESULT hr = HSDK_FAIL;

	_cellX = min(WORLD_SIZE, bullet::compute_Find2(_cellX));
	_cellY = min(WORLD_SIZE, bullet::compute_Find2(_cellY));

	// 터레인 설정
	direct3d::terrain::build_Terrain(
		my_Terrain, _cellX, _cellY, _cellX, _cellY);

	// 생성
	IF_FAILED(direct3d::g_D3D10_Factory.build_MeshTerran(
		&my_TerrainMesh, my_Terrain, _heightBuffer))
	{
		return hr;
	}

	// 터레인 물리 적용
	IF_FAILED(hr = my_PhysicsEngine.setup1_Terrain(
		_cellX,
		_cellY,
		_heightBuffer,
		_size))
	{
		return hr;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, usetSet_Terrain)(
	_In_ const wchar_t * _texture)
{
	HRESULT hr;

	//
	ID3D10ShaderResourceView * view;
	IF_SUCCEEDED(hr = direct3d::g_D3D10_Factory.get_Texture(&view, _texture))
	{
		my_TerrainMesh.materials[0].diffuseRV =
			AutoRelease<ID3D10ShaderResourceView>(view);
	}

	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, usetSet_Sky)(
	_In_ unsigned int _width,
	_In_ unsigned int _height,
	_In_ const wchar_t * _front,
	_In_ const wchar_t * _back,
	_In_ const wchar_t * _left,
	_In_ const wchar_t * _right,
	_In_ const wchar_t * _top,
	_In_ const wchar_t * _bottom)
{
	HRESULT hr;

	// 스카이 박스
	IF_FAILED(hr = direct3d::g_D3D10_Factory.build_MeshSkyBox(
		&my_SkyMesh, WORLD_SIZE / 2))
	{
		return hr;
	}

	// 스카이 박스 텍스처
	ID3D10ShaderResourceView * view;
	IF_FAILED(hr = direct3d::g_D3D10_Factory.create_SkyBoxTexture(
		&view,
		_width,
		_height,
		_front,
		_back,
		_left,
		_right,
		_top,
		_bottom))
	{
		return hr;
	}

	my_SkyMesh.materials[0].diffuseRV =
		AutoRelease<ID3D10ShaderResourceView>(view);

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, create_User)(
	_Out_ PTR_USER * _user,
	_In_ const REF_KEY _key,
	_In_ void * _context)
{
	HRESULT hr;

	PTR_DATA data;
	IF_FAILED(hr = get_Data(&data, _key))
	{
		return hr;
	}

	unsigned int id;
	IF_FAILED(hr = create_ID(&id, _key))
	{
		return hr;
	}

	return create_User(_user, id, data, _context);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, create_User)(
	_Out_ PTR_USER * _user,
	_In_ unsigned int _id,
	_In_ const PTR_DATA _data,
	_In_ void * _context)
{
	PTR_USER newUser = new User(_id);

	newUser->set_Datatalbe(create_DataTable(_data, (const float *)(_context)));
	newUser->set_Controller(create_Controller(_data));
	newUser->set_ActionBase(create_ActionBase(_data));
	newUser->set_Renderer(create_Renderer(_data));

	(*_user) = newUser;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, create_ID)(
	_Out_ unsigned int * _id,
	_In_ const REF_KEY _key)
{
	GOE_DataMap::const_iterator iter = my_DataMap.find(_key);
	if (iter == my_DataMap.end())
	{
		return HSDK_FAIL;
	}

	unsigned int id = _key << 16;
	id = ADD_FLAG(id, iter->second->count);

	(*_id) = id;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, get_Data)(
	_Out_ PTR_DATA * _data,
	_In_ const REF_KEY _key)
{
	PTR_DATA & data = my_DataMap[_key];
	IF_INVALID(data)
	{
		switch (_key)
		{
		case 0:

			data = new GOE_Data();
			data->cbsize = sizeof(GOE_Data);
			data->count = 0;

			data->dataTable_Key = 0;
			data->actionBase_Key = 0;
			data->controller_key = 0;
			data->renderer_Key = 0;

			btVector3 vector(5.0f, 5.0f, 5.0f);
			data->shape = new btBoxShape(vector);

			break;
		}

		IF_INVALID(data)
		{
			my_DataMap.erase(_key);
			return HSDK_FAIL;
		}
	}

	(*_data) = data;

	return 0;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, push)(
	/* [set] */ PTR_USER _user)
{
	my_UserMap[_user->get_ID() >> 16].push_back(_user);

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, void, pop)(
	_In_ PTR_USER _user)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, void, pop)(
	_In_ unsigned int _id)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, void, clear)(
	_In_ const REF_KEY _key)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, void, clear_All)(
	_X_ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, move)(
	_In_ const REF_KEY _from,
	_In_ unsigned int _id,
	_In_ const REF_KEY _to,
	_Out_ PTR_USER * _user)
{
	return HSDK_NOTIMPL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, move_all)(
	_In_ const REF_KEY _from,
	_In_ const REF_KEY _to,
	_In_ unsigned int _size,
	_Out_ PTR_USER * _uerbuffer)
{
	return HSDK_NOTIMPL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, void, sort)(
	_X_ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, unsigned int, size)(
	_In_ const REF_KEY _key)
{
	return 0;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, unsigned int, size_All)(
	_X_ void)
{
	return 0;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, find_Explicit)(
	_Out_ PTR_USER * _user,
	_In_ unsigned int _id)const
{
	return HSDK_NOTIMPL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, find_Explicit_All)(
	_Out_ PTR_USER * _userbuffer,
	_In_ const REF_KEY _key,
	_In_ unsigned int _size)const
{
	return HSDK_NOTIMPL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, find_implicit)(
	_Out_ PTR_USER * _user,
	_In_ const REF_KEY _key,
	_In_ const LPVOID _data,
	_In_ unsigned int _format,
	_In_ unsigned int _size)const
{
	return HSDK_NOTIMPL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(GameEngine, find_implicit_All)(
	_Out_ PTR_USER * _userbuffer,
	_In_ const LPVOID _data,
	_In_ unsigned int _format,
	_In_ unsigned int _size)const
{
	return HSDK_NOTIMPL;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, GOE::PTR_FACTORY, get_Factory)(
	_X_ void)
{
	return this;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, GOE::PTR_CONTAINER, get_Container)(
	_X_ void)
{
	return this;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, GOE::PTR_FINDER, get_Finder)(
	_X_ void)
{
	return this;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, void, update)(
	_In_ float _time)
{
	my_PhysicsEngine.update(_time);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, void, render)(
	_X_ void)
{
	direct3d::g_D3D10_Renderer.set_MatrixWorldViewProj(&direct3d::g_D3D10_ViewProjMatrix);
	direct3d::g_D3D10_Renderer.set_ScalarVSFlag(0);
	direct3d::g_D3D10_Renderer.set_ScalarPSFlag(direct3d::PS_TEXTURE_0);
	direct3d::g_D3D10_Renderer.render_SkyBox(my_SkyMesh);
	direct3d::g_D3D10_Renderer.render_Mesh(my_TerrainMesh);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, void, reset)(
	_X_ void)
{
	clear_All();

	my_PhysicsEngine.reset();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, game::DataTable *, create_DataTable)(
	_In_ const PTR_DATA _data,
	_In_ const D3DXMATRIX & _matrix)
{
	btTransform form;
	form.setIdentity();

	if (_matrix)
	{
		memcpy(&form, &_matrix, 16 * sizeof(float));
	}

	btRigidBody * body = nullptr;
	if (SUCCEEDED(my_PhysicsEngine.create(&body, form, _data->shape, 1.0f)))
	{
		game::DataTable * newData = new GameDataTable();
		newData->set_Body(body);

		return newData;
	}
	else
	{
		return nullptr;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, game::Controller *, create_Controller)(
	_In_ const PTR_DATA _data)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, game::ActionBase *, create_ActionBase)(
	_In_ const PTR_DATA _data)
{
	return nullptr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GameEngine, game::Renderer *, create_Renderer)(
	_In_ const PTR_DATA _data)
{
	return nullptr;
}