#include "game.h"
#include "entry.h"
#include "gomain.h"



// (256 + 1) * (16 + 1)
#define TERRAINHEIGHTBUFFER 4369
#define TERRAINWIDTH 256
#define TERRAINDEPTH 16
#define BOXSIZE 1.5f



//--------------------------------------------------------------------------------------
// Grobal Variable
//--------------------------------------------------------------------------------------

// 설명 : 
common::GUI_Target g_GUI_Game;

// 설명 : 
go::GameEngine g_GameEngine;

// 설명 :
FMOD::Sound * g_Sound_Background1;
FMOD::Channel * g_Sound_Controller1;

// 설명 : 
D3DXVECTOR3 g_vTarget;
D3DXVECTOR3 g_vPos;
D3DXVECTOR3 g_vUp;

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
		entryloop::initialize_Entry(nullptr);
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
		D3DXMATRIX r1, r2;
		D3DXVECTOR3 cross;

		D3DXVec3Normalize(&cross, &g_vPos);
		D3DXMatrixRotationAxis(&r1, D3DXVec3Cross(&cross, &cross, &g_vUp), D3DXToDegree(_y) * 0.001f);
		D3DXMatrixRotationY(&r2, D3DXToDegree(_x) * 0.001f);

		D3DXVec3TransformNormal(&g_vPos, &g_vPos, &(r1 * r2));
	}

	// 설명 : mouse의 wheel을 조작하면 발생하는 event.
	INTERFACE_DECL_FUNC_T(void, onWheel)(
		_In_ int _x,
		_In_ int _y,
		_In_ int _w)
	{
		D3DXVECTOR3 normal;
		D3DXVec3Normalize(&normal, &g_vPos);
		g_vPos += (normal * float(_w / 40));
	}

};

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, gameloop::OnFrameMove)(
	_In_ double _fTime,
	_In_ float _fElapsedTime,
	_Inout_ void * _userContext)
{
	sound::g_FMOD_SoundDevice.play();
	g_GameEngine.update(_fElapsedTime);
	g_GUI_Game.update();
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, gameloop::OnD3D10FrameRender)(
	_In_ ID3D10Device * _d3dDevice,
	_In_ double _fTime,
	_In_ float _fElapsedTime,
	_Inout_ void * _userContext)
{
	D3DXMatrixLookAtLH(&direct3d::g_D3D10_ViewMatrix, &(g_vTarget + g_vPos), &g_vTarget, &g_vUp);
	direct3d::g_D3D10_ViewProjMatrix = direct3d::g_D3D10_ViewMatrix * direct3d::g_D3D10_ProjMatrix;

	// Clear the render target and depth stencil
	float ClearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	ID3D10RenderTargetView * pRTV = framework::g_Framework_Device.d3d10RTV;
	_d3dDevice->ClearRenderTargetView(pRTV, ClearColor);
	ID3D10DepthStencilView * pDSV = framework::g_Framework_Device.d3d10DSV;
	_d3dDevice->ClearDepthStencilView(pDSV, D3D10_CLEAR_DEPTH, 1.0, 0);
	g_GameEngine.render();
	g_GUI_Game.render();
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(LRESULT, gameloop::OnMsgProc)(
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
		g_GUI_Game.redraw();
	}

	return 0;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, gameloop::OnMouse)(
	_In_ const short * _buttonsDown,
	_In_ unsigned int _buttonCount,
	_In_ int _mouseWheelDelta,
	_In_ int _xPos,
	_In_ int _yPos,
	_Inout_ void * _userContext)
{
	common::OnMouse_GUI(
		_buttonsDown,
		_buttonCount,
		_mouseWheelDelta,
		_xPos, _yPos,
		_userContext);
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, gameloop::OnKeyboard)(
	_In_ unsigned char _nKey,
	_In_ short _bKeyDown,
	_In_ short _bAltDown,
	_Inout_ void * _userContext)
{

}

//--------------------------------------------------------------------------------------
IMPL_FUNC(gameloop::OnD3D10CreateDevice)(
	_In_ ID3D10Device * _d3dDevice,
	_In_ const DXGI_SURFACE_DESC & _backBufferSurfaceDesc,
	_Inout_ void * _userContext)
{
	HRESULT hr = E_FAIL;
	IF_SUCCEEDED(hr = common::initialize_Common(&g_GUI_Game))
	{
		g_vTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_vPos = D3DXVECTOR3(0.0f, 20.0f, 100.0f);
		g_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		D3DXMatrixLookAtLH(&direct3d::g_D3D10_ViewMatrix,
			&(g_vTarget + g_vPos), &g_vTarget, &g_vUp);

		float heightbuffer[TERRAINHEIGHTBUFFER];
		IF_FAILED(hr = direct3d::terrain::load_RawFromFile(heightbuffer, L"terrain/terrain0.raw", TERRAINHEIGHTBUFFER, 25))
		{
			return hr;
		}

		// 터레인
		IF_FAILED(hr = g_GameEngine.setup0_Terrain(TERRAINWIDTH, TERRAINDEPTH, heightbuffer, TERRAINHEIGHTBUFFER))
		{
			return hr;
		}

		// 터레인
		IF_FAILED(hr = g_GameEngine.usetSet_Terrain(L"image/terrain/terrain.png"))
		{
			return hr;
		}

		// 스카이
		IF_FAILED(hr = g_GameEngine.usetSet_Sky(512, 512,
			L"image/skybox/jajalien1_front.jpg",
			L"image/skybox/jajalien1_back.jpg",
			L"image/skybox/jajalien1_left.jpg",
			L"image/skybox/jajalien1_right.jpg",
			L"image/skybox/jajalien1_top.jpg",
			L""))
		{
			return hr;
		}

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
DECL_FUNC_T(void, gameloop::OnD3D10DestroyDevice)(
	_Inout_ void * _userContext)
{
	common::destroy_Common();

	// bullet::g_BulletEngine.reset();
	/*direct3d::mesh::meshClear(g_TerrainMesh);
	direct3d::mesh::meshClear(g_SkyMesh);*/

	g_GUI_Game.reset();
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(gameloop::initialize_Game)(
	_In_ void * _context)
{
	if (framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc)
	{
		framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc(
			framework::g_Framework_Callbacks.d3d10DeviceDestroyedFuncUserContext);
	}

	framework::g_Framework_Callbacks.windowMsgFunc = gameloop::OnMsgProc;
	framework::g_Framework_Callbacks.mouseFunc = gameloop::OnMouse;
	framework::g_Framework_Callbacks.keyboardFunc = gameloop::OnKeyboard;
	framework::g_Framework_Callbacks.frameMoveFunc = gameloop::OnFrameMove;
	framework::g_Framework_Callbacks.d3d10DeviceCreatedFunc = gameloop::OnD3D10CreateDevice;
	framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc = gameloop::OnD3D10DestroyDevice;
	framework::g_Framework_Callbacks.d3d10FrameRenderFunc = gameloop::OnD3D10FrameRender;

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
IMPL_FUNC(gameloop::build_GameLayout)(
	_In_ frame::Container * _container,
	_In_ float _width,
	_In_ float _height)
{
	_container->clear_Component();

	frame::Component * button = new frame::ButtonCompo();
	button->set_Visible(true);
	button->set_X(0.0f);
	button->set_Y(0.0f);
	button->set_Mouseable(new GoEntryButtonEvent());
	button->graphics()->set_image(L"image/layout/button.png");

	// 버튼 컨테이너
	frame::RenderTargetContainer<frame::Container> * skillContainer =
		new frame::RenderTargetContainer<frame::Container>();

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