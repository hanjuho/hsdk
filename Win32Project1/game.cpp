#include "game.h"
#include "entry.h"
#include "bullet_engine.h"


// (256 + 1) * (16 + 1)
#define TERRAINHEIGHTBUFFER 4369
#define TERRAINWIDTH 256
#define TERRAINDEPTH 16
#define BOXSIZE 1.5f


//--------------------------------------------------------------------------------------
// Grobal Variable
//--------------------------------------------------------------------------------------

// 설명 : 
frame::Container g_GUI_Game;

// 설명 :
FMOD::Sound * g_Sound_Background1;
FMOD::Channel * g_Sound_Controller1;

// 설명 : 
direct3d::D3D10_Terrain g_Terrain;
direct3d::D3D10_Mesh g_TerrainMesh;
direct3d::D3D10_Mesh g_SkyMesh;

// 설명 :
direct3d::D3D10_Mesh g_BoxMesh;

// 설명 : 
direct3d::D3D10_Mesh mesh;
direct3d::D3D10_Animation anim;

struct BodyBody
{

	btRigidBody * body;

	direct3d::D3D10_Animation_Recorder pos;

};

// 설명 : 
bullet::Bullet_Engine g_Engine(1.0f);
std::list<BodyBody> g_Bodys;
btRigidBody * g_Controller;

// 설명 : 
framework::Framework_Camera g_Camera;

//--------------------------------------------------------------------------------------
class GoEntryButtonEvent :
	public i::frame::MouseableAdapter
{
public:

	//
	INTERFACE_DECL_FUNC_T(void, onClick_Up)(
		_In_ i::frame::MOUSE_BUTTON _button,
		_In_ int _x,
		_In_ int _y)
	{
		entry::initialize_Entry(nullptr);
	}
};


//--------------------------------------------------------------------------------------
class ScreenMouseEvent :
	public i::frame::MouseableAdapter
{
public:

	//
	INTERFACE_DECL_FUNC_T(void, onClick_Up)(
		_In_ i::frame::MOUSE_BUTTON _button,
		_In_ int _x,
		_In_ int _y)
	{

	}

	// 설명 : mouse의 버튼을 누른 채 커서를 이동하면 발생하는 event.
	INTERFACE_DECL_FUNC_T(void, onDrag)(
		_In_ i::frame::MOUSE_BUTTON _button,
		_In_ int _x,
		_In_ int _y)
	{
		g_Camera.rotate_YAxis(D3DXToDegree(_x) * 0.001f, true);
		g_Camera.rotate_XAxis(D3DXToDegree(_y) * 0.001f, true);

		g_Camera.compute_ViewMatrix(direct3d::g_D3D10_ViewMatrix);
	}

	// 설명 : mouse의 wheel을 조작하면 발생하는 event.
	INTERFACE_DECL_FUNC_T(void, onWheel)(
		_In_ int _x,
		_In_ int _y,
		_In_ int _w)
	{
		g_Camera.set_Hinge(g_Camera.get_Hinge() + (_w / 120.f));
	}

};

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, game::OnFrameMove)(
	_In_ double _fTime,
	_In_ float _fElapsedTime,
	_Inout_ void * _userContext)
{
	sound::g_FMOD_SoundDevice.play();

	g_GUI_Game.update();

	g_Engine.update(_fElapsedTime);

	if (g_Bodys.empty())
	{
		g_Controller = nullptr;
	}
	else
	{
		g_Controller = 
			g_Bodys.front().body;
	}
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, game::OnD3D10FrameRender)(
	_In_ ID3D10Device * _d3dDevice,
	_In_ double _fTime,
	_In_ float _fElapsedTime,
	_Inout_ void * _userContext)
{
	g_Camera.compute_ViewMatrix(direct3d::g_D3D10_ViewMatrix);
	direct3d::g_D3D10_ViewProjMatrix =
		direct3d::g_D3D10_ViewMatrix * direct3d::g_D3D10_ProjMatrix;

	// Clear the render target and depth stencil
	float ClearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	ID3D10RenderTargetView * pRTV = framework::g_Framework_Device.d3d10RTV;
	_d3dDevice->ClearRenderTargetView(pRTV, ClearColor);
	ID3D10DepthStencilView * pDSV = framework::g_Framework_Device.d3d10DSV;
	_d3dDevice->ClearDepthStencilView(pDSV, D3D10_CLEAR_DEPTH, 1.0, 0);

	direct3d::g_D3D10_Renderer.set_MatrixWorldViewProj(&direct3d::g_D3D10_ViewProjMatrix);
	direct3d::g_D3D10_Renderer.set_ScalarVSFlag(0);
	direct3d::g_D3D10_Renderer.set_ScalarPSFlag(direct3d::PS_TEXTURE_0);
	direct3d::g_D3D10_Renderer.render_SkyBox(g_SkyMesh);
	direct3d::g_D3D10_Renderer.render_Mesh(g_TerrainMesh);

	auto begin = g_Bodys.begin();
	auto end = g_Bodys.end();

	direct3d::g_D3D10_Renderer.set_ScalarVSFlag(0);
	direct3d::g_D3D10_Renderer.set_ScalarPSFlag(direct3d::PS_TEXTURE_0);
	
	while (begin != end)
	{
		BodyBody & body = (*begin);

		btDefaultMotionState * dms = (
			btDefaultMotionState *)body.body->getMotionState();

		D3DXMATRIX t;
		dms->m_graphicsWorldTrans.getOpenGLMatrix((btScalar *)&t);
		t = t * direct3d::g_D3D10_ViewProjMatrix;

		direct3d::g_D3D10_Renderer.set_MatrixWorldViewProj(&t);
		body.pos.time += _fElapsedTime;
		direct3d::animation::animate_Pos(body.pos, anim);
		direct3d::g_D3D10_Renderer.render_Skinned(mesh, anim, body.pos);

		++begin;
	}
	
	g_GUI_Game.render();
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(LRESULT, game::OnMsgProc)(
	_Out_opt_ BOOL * _bNoFurtherProcessing,
	_In_ HWND _hWnd,
	_In_ UINT _uMsg,
	_In_ WPARAM _wParam,
	_In_ LPARAM _lParam,
	_Inout_ void * _userContext)
{
	switch (_uMsg)
	{
	case WM_SIZE:
		g_GUI_Game.set_W((float)LOWORD(_lParam));
		g_GUI_Game.set_H((float)HIWORD(_lParam));
		g_GUI_Game.reform();
	}

	return 0;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, game::OnMouse)(
	_In_ const short * _buttonsDown,
	_In_ unsigned int _buttonCount,
	_In_ int _mouseWheelDelta,
	_In_ int _xPos,
	_In_ int _yPos,
	_Inout_ void * _userContext)
{
	common::OnMouse(
		_buttonsDown,
		_buttonCount,
		_mouseWheelDelta,
		_xPos, _yPos,
		_userContext);
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, game::OnKeyboard)(
	_In_ unsigned char _nKey,
	_In_ short _bKeyDown,
	_In_ short _bAltDown,
	_Inout_ void * _userContext)
{
	if (_nKey == 'Q')
	{
		btTransform form;
		form.setIdentity();
		form.setOrigin(btVector3(0.0f, 10.0f, 0.0f));

		btRigidBody * body = g_Engine.add(form, new btBoxShape(btVector3(
			BOXSIZE, BOXSIZE, BOXSIZE)), 10.0f);		

		if (body)
		{
			g_Bodys.push_back({ body });
			direct3d::animation::build_Pos(g_Bodys.back().pos, anim, 0, 0.0f);
		}
	}

	if (g_Controller)
	{
		btDefaultMotionState * dms =
			(btDefaultMotionState *)g_Controller->getMotionState();
		
		float time = framework::g_Framework_TimeStream.get_ElapsedTime();
		if (_nKey == VK_UP)
		{
			g_Controller->applyCentralImpulse(
			{ 0.0f, 0.0f, 5.0f });
		}

		if (_nKey == VK_DOWN)
		{
			g_Controller->applyCentralImpulse(
			{ 0.0f, 0.0f, -5.0f });
		}

		if (_nKey == VK_LEFT)
		{
			g_Controller->applyCentralImpulse(
			{ -5.0f, 0.0f, 0.0f });
		}

		if (_nKey == VK_RIGHT)
		{
			g_Controller->applyCentralImpulse(
			{ 5.0f, 0.0f, 0.0f });
		}

		if (_nKey == VK_SPACE)
		{
			g_Controller->applyCentralImpulse(
			{ 0.0f, 5.0f, 0.0f });
		}

		dms->m_graphicsWorldTrans.setIdentity();
		g_Controller->activate(true);
	}
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(game::OnD3D10CreateDevice)(
	_In_ ID3D10Device * _d3dDevice,
	_In_ const DXGI_SURFACE_DESC & _backBufferSurfaceDesc,
	_Inout_ void * _userContext)
{
	HRESULT hr = E_FAIL;
	IF_SUCCEEDED(hr = common::initialize_Common(&g_GUI_Game))
	{
		// model
		direct3d::g_D3D10_Factory.build_MeshFromFile(
			mesh, L"model/", L"Deathwing.X", &anim);

		g_Camera.set_Position(D3DXVECTOR3(0.0f, 20.0f, -50.0f));
		g_Camera.set_Hinge(50.0f);
		//g_Camera.set_Target(D3DXVECTOR3(0.0f, 10.0f, 0.0f));

		// skybox
		{
			//
			ID3D10ShaderResourceView * view;
			direct3d::g_D3D10_Factory.create_SkyBoxTexture(&view, 512, 512,
				L"image/skybox/jajalien1_front.jpg",
				L"image/skybox/jajalien1_back.jpg",
				L"image/skybox/jajalien1_left.jpg",
				L"image/skybox/jajalien1_right.jpg",
				L"image/skybox/jajalien1_top.jpg",
				L"");

			direct3d::g_D3D10_Factory.build_MeshSkyBox(g_SkyMesh, 100.0f);

			g_SkyMesh.materials[0].diffuseRV =
				AutoRelease<ID3D10ShaderResourceView>(view);
		}

		// height buffer
		float heightbuffer[TERRAINHEIGHTBUFFER];
		direct3d::terrain::load_RawFromFile(
			heightbuffer, L"terrain/terrain0.raw", TERRAINHEIGHTBUFFER, 25);

		// terrain
		{
			// pixel size 1.0f
			direct3d::terrain::build_Terrain(
				g_Terrain, TERRAINWIDTH, TERRAINDEPTH, TERRAINWIDTH, TERRAINDEPTH);

			//
			direct3d::g_D3D10_Factory.build_MeshTerran(
				g_TerrainMesh, g_Terrain, heightbuffer);

			direct3d::g_D3D10_Factory.build_MeshBox(
				g_BoxMesh, { 1.0f, 0.0f, 0.0f, 1.0f }, BOXSIZE);

			//
			ID3D10ShaderResourceView * view;
			IF_SUCCEEDED(direct3d::g_D3D10_Factory.get_Texture(
				&view, L"image/terrain/terrain.png"))
			{
				g_TerrainMesh.materials[0].diffuseRV =
					AutoRelease<ID3D10ShaderResourceView>(view);
			}
		}

		// 물리 엔진
		g_Engine.setup0_World(300, 300, 300);
		g_Engine.setup1_Terrain(TERRAINWIDTH, TERRAINDEPTH, heightbuffer, TERRAINHEIGHTBUFFER);

		// layout
		IF_FAILED(hr = build_GameLayout(&g_GUI_Game,
			(float)_backBufferSurfaceDesc.Width,
			(float)_backBufferSurfaceDesc.Height))
		{
			return hr;
		}

		// sound
		IF_FAILED(hr = sound::g_FMOD_SoundDevice.load_WaveFile(
			&g_Sound_Background1,
			&g_Sound_Controller1,
			L"sound/game_background.wav"))
		{
			return hr;
		}

		FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

		g_Sound_Controller1->setMode(FMOD_LOOP_NORMAL);
		g_Sound_Controller1->set3DAttributes(&pos, &vel);
	}

	return hr;
}

//--------------------------------------------------------------------------------------
DECL_FUNC_T(void, game::OnD3D10DestroyDevice)(
	_Inout_ void * _userContext)
{
	common::destroy_Common();

	direct3d::mesh::meshClear(g_TerrainMesh);
	direct3d::mesh::meshClear(g_SkyMesh);
	direct3d::mesh::meshClear(g_BoxMesh);
	direct3d::mesh::meshClear(mesh);

	g_Bodys.clear();
	g_Engine.reset();

	g_GUI_Game.clear();
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(game::initialize_Game)(
	_In_ void * _context)
{
	if (framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc)
	{
		framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc(
			framework::g_Framework_Callbacks.d3d10DeviceDestroyedFuncUserContext);
	}

	framework::g_Framework_Callbacks.windowMsgFunc = game::OnMsgProc;
	framework::g_Framework_Callbacks.mouseFunc = game::OnMouse;
	framework::g_Framework_Callbacks.keyboardFunc = game::OnKeyboard;
	framework::g_Framework_Callbacks.frameMoveFunc = game::OnFrameMove;

	framework::g_Framework_Callbacks.d3d10DeviceCreatedFunc = game::OnD3D10CreateDevice;
	framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc = game::OnD3D10DestroyDevice;
	framework::g_Framework_Callbacks.d3d10FrameRenderFunc = game::OnD3D10FrameRender;

	if (framework::g_Framework_Callbacks.d3d10DeviceCreatedFunc)
	{
		IF_FAILED(framework::g_Framework_Callbacks.d3d10DeviceCreatedFunc(
			framework::g_Framework_Device.d3d10Device,
			framework::g_Framework_Device.dxgiBackBuffer_Desc,
			_context))
		{
			MessageBox(framework::g_Framework_Window.hwnd, L"초기화에 실패했습니다.\n코드를 확인해주세요", L"Error", MB_OK);
			return E_FAIL;
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(game::build_GameLayout)(
	_In_ frame::Container * _container,
	_In_ float _width,
	_In_ float _height)
{
	_container->clear();

	frame::Component * button = new frame::ButtonCompo();
	button->set_Visible(true);
	button->set_X(0.0f);
	button->set_Y(0.0f);
	button->set_Mouseable(new GoEntryButtonEvent());
	button->graphics()->set_image(L"image/layout/button.png");

	// 버튼 컨테이너
	frame::RenderTargetContainer * skillContainer = new frame::RenderTargetContainer();

	skillContainer->set_Visible(true);
	skillContainer->set_X(0.0f);
	skillContainer->set_Y(0.0f);
	skillContainer->set_W(_width);
	skillContainer->set_H(_height);
	skillContainer->graphics()->set_image(L"image/layout/layout_0.png");

	// 메인 레이아웃
	frame::layout::BorderLayout * borderLayout = new frame::layout::BorderLayout();

	borderLayout->set_Space(
		hsdk::i::frame::SPACE_LEFT, 0.0f);
	borderLayout->set_Space(
		hsdk::i::frame::SPACE_TOP, 0.7f);
	borderLayout->set_Space(
		hsdk::i::frame::SPACE_RIGHT, 0.0f);
	borderLayout->set_Space(
		hsdk::i::frame::SPACE_BOTTOM, 0.02f);

	borderLayout->set_HGap(0.07f);
	borderLayout->set_VGap(0.07f);

	// 메인 컨테이너
	_container->set_Visible(true);
	_container->set_X(0.0f);
	_container->set_Y(0.0f);
	_container->set_W(_width);
	_container->set_H(_height);
	_container->set_Layout(borderLayout);
	_container->set_Mouseable(new ScreenMouseEvent());
	_container->graphics()->set_Background({ 0 });

	//
	_container->add_Component(skillContainer, i::frame::COMPOSITION_CENTER);

	//
	_container->add_Component(button, i::frame::COMPOSITION_WEST);

	//
	_container->reform();

	return S_OK;
}