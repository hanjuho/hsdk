#include "game.h"
#include "entry.h"

class ToEntryButtonEvent :
	public common::MouseableAdapter
{
public:

	INTERFACE_DECL_FUNC_T(void, onClick_Up)(
		_In_ i::frame::MOUSE_BUTTON _button,
		_In_ int _x,
		_In_ int _y)
	{
		entry::initialize();
	}
};

//--------------------------------------------------------------------------------------
// Grobal Variable
//--------------------------------------------------------------------------------------

// 설명 : 
frame::Container g_GUI_Background1;

// 설명 : 
frame::inputEventHelper g_GUI_Helper1(&g_GUI_Background1);

// 설명 :
FMOD::Sound * g_Sound_Background1;
FMOD::Channel * g_Sound_Controller1;

// 설명 : 
direct3d::D3D10_Mesh g_Mesh;
direct3d::D3D10_Animation g_MeshAnimation;
direct3d::D3D10_Animation_Recorder g_MeshPos;

// 설명 : 
D3DXMATRIX g_World_0 = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f };

framework::Framework_Camera g_Camera;
int x = 0, y = 0; float length = 5;

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, game::initialize)(
	_X_ void)
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
			framework::g_Framework_Callbacks.d3d10DeviceCreatedFuncUserContext))
		{
			MessageBox(framework::g_Framework_Window.hwnd, L"초기화에 실패했습니다.\n코드를 확인해주세요", L"Error", MB_OK);
			framework::g_Framework.shutdown();
		}
	}
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
		g_GUI_Background1.set_W((float)LOWORD(_lParam));
		g_GUI_Background1.set_H((float)HIWORD(_lParam));
		g_GUI_Background1.reform();
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
	length += (float)_mouseWheelDelta / (float)120.0f;

	if (_buttonsDown[FRAMEWORK_LEFTBUTTON] == 1)
	{
		g_Camera.rotate_YAxis(D3DXToDegree(_xPos - x) * 0.001f);
		g_Camera.rotate_XAxis(D3DXToDegree(_yPos - y) * 0.001f);


		const float * dir = g_Camera.get_ZDir();
		D3DXVECTOR3 position(
			-(dir[0] * length),
			0.5f - (dir[1] * length),
			-(dir[2] * length));
		g_Camera.set_Position(position);


		g_GUI_Helper1.onClick_Down(i::frame::LBUTTON, _xPos, _yPos);
	}
	else if (_buttonsDown[FRAMEWORK_LEFTBUTTON] == 2)
	{
		g_GUI_Helper1.onClick_Up(i::frame::LBUTTON, _xPos, _yPos);
	}

	if (_buttonsDown[FRAMEWORK_MIDDLEBUTTON])
	{
		g_GUI_Helper1.onClick_Down(i::frame::WBUTTON, _xPos, _yPos);
	}
	else if (_buttonsDown[FRAMEWORK_MIDDLEBUTTON] == 2)
	{
		g_GUI_Helper1.onClick_Up(i::frame::LBUTTON, _xPos, _yPos);
	}

	if (_buttonsDown[FRAMEWORK_RIGHTBUTTON])
	{
		g_GUI_Helper1.onClick_Down(i::frame::RBUTTON, _xPos, _yPos);
		direct3d::animation::reset_Pos(g_MeshPos, g_MeshAnimation);
	}
	else if (_buttonsDown[FRAMEWORK_RIGHTBUTTON] == 2)
	{
		g_GUI_Helper1.onClick_Up(i::frame::RBUTTON, _xPos, _yPos);
	}

	x = _xPos;
	y = _yPos;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, game::OnKeyboard)(
	_In_ unsigned char _nKey,
	_In_ short _bKeyDown,
	_In_ short _bAltDown,
	_Inout_ void * _userContext)
{
	if (_bKeyDown)
	{

		for (unsigned int index = 0; index < 10; ++index)
		{
			if (_nKey == (char)(49 + index))
			{
				if (index < g_MeshAnimation.animations.size())
				{
					direct3d::animation::trans_Pos(
						g_MeshPos,
						g_MeshAnimation,
						index);
				}
			}
		}

	}
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, game::OnFrameMove)(
	_In_ double _fTime,
	_In_ float _fElapsedTime,
	_Inout_ void * _userContext)
{
	sound::g_FMOD_SoundDevice.play();
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(game::OnD3D10CreateDevice)(
	_In_ ID3D10Device * _d3dDevice,
	_In_ const DXGI_SURFACE_DESC & _backBufferSurfaceDesc,
	_Inout_ void * _userContext)
{
	HRESULT hr = E_FAIL;
	IF_SUCCEEDED(hr = common::initialize_Common())
	{
		g_Camera.set_Position(D3DXVECTOR3(0.0f, 0.0f, -5.0f));

		// gui
		g_GUI_Background1.graphics()->set_image(L"image/layout/layout_0.png");
		g_GUI_Background1.set_X((float)_backBufferSurfaceDesc.Width / 2 - 512.0f);
		g_GUI_Background1.set_Y((float)_backBufferSurfaceDesc.Height - 168.0f);
		g_GUI_Background1.set_W((float)1024.0f);
		g_GUI_Background1.set_H((float)168.0f);
		g_GUI_Background1.reform();
		g_GUI_Background1.set_Visible(true);

		frame::Component * button = new frame::ButtonCompo(10, 10, 312, 48);
		button->set_Mouseable(new ToEntryButtonEvent());
		button->graphics()->set_image(L"image/layout/button.png");
		button->set_Visible(true);

		g_GUI_Background1.add_Component(button);

		// sound
		IF_SUCCEEDED(hr = sound::g_FMOD_SoundDevice.load_WaveFile(
			&g_Sound_Background1,
			&g_Sound_Controller1,
			L"sound/game_background.wav"))
		{
			FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
			FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

			g_Sound_Controller1->setMode(FMOD_LOOP_NORMAL);
			g_Sound_Controller1->set3DAttributes(&pos, &vel);
		}

		IF_FAILED(hr = direct3d::g_D3D10_Factory.create_MeshFromFile(
			g_Mesh, L"model/human/", L"Deathwing.X",
			&g_MeshAnimation))
		{
			return hr;
		}

		IF_FAILED(hr = direct3d::animation::create_Pos(
			g_MeshPos,
			g_MeshAnimation,
			0, 0.0f))
		{
			return hr;
		}
	}

	return hr;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, game::OnD3D10FrameRender)(
	_In_ ID3D10Device * _d3dDevice,
	_In_ double _fTime,
	_In_ float _fElapsedTime,
	_Inout_ void * _userContext)
{
	g_Camera.compute_ViewMatrix(direct3d::g_D3D10_ViewMatrix);

	// Clear the render target and depth stencil
	float ClearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	ID3D10RenderTargetView * pRTV = framework::g_Framework_Device.d3d10RTV;
	_d3dDevice->ClearRenderTargetView(pRTV, ClearColor);
	ID3D10DepthStencilView * pDSV = framework::g_Framework_Device.d3d10DSV;
	_d3dDevice->ClearDepthStencilView(pDSV, D3D10_CLEAR_DEPTH, 1.0, 0);
	
	g_MeshPos.time += _fElapsedTime;
	direct3d::animation::animate_Pos(
		g_MeshPos,
		g_MeshAnimation);

	D3DXMATRIX matrix;
	direct3d::g_D3D10_Renderer.set_MatrixWorld(D3DXMatrixIdentity(&matrix));
	direct3d::g_D3D10_Renderer.set_MatrixWorldViewProj(D3DXMatrixMultiply(&matrix, &direct3d::g_D3D10_ViewMatrix, &direct3d::g_D3D10_ProjMatrix));
	
	direct3d::g_D3D10_Renderer.set_ScalarVSFlag(0);

	static float time = 0.0f;
	time += _fElapsedTime;
	if (1.0f < time)
	{
		time -= 1.0;
	}

	direct3d::g_D3D10_Renderer.set_ScalarVSTime(0.5f * time);
	direct3d::g_D3D10_Renderer.set_ScalarPSFlag(direct3d::PS_TEXTURE_0);

	direct3d::g_D3D10_Renderer.render_Skinned(
		g_Mesh,
		g_MeshAnimation,
		g_MeshPos);

	g_GUI_Background1.render();
}

//--------------------------------------------------------------------------------------
DECL_FUNC_T(void, game::OnD3D10DestroyDevice)(
	_Inout_ void * _userContext)
{
	common::destroy_Common();

	g_GUI_Background1.clear();

	direct3d::mesh::meshClear(g_Mesh);
	direct3d::animation::animationClear(g_MeshAnimation);
	direct3d::animation::animationRecordClear(g_MeshPos);
}