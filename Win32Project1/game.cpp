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
		entry::initialize_Entry(nullptr);
	}
};

//--------------------------------------------------------------------------------------
// Grobal Variable
//--------------------------------------------------------------------------------------

// 설명 : 
frame::Container g_GUI_Game;

// 설명 : 
frame::inputEventHelper g_GUIHelper_Game(&g_GUI_Game);

// 설명 :
FMOD::Sound * g_Sound_Background1;
FMOD::Channel * g_Sound_Controller1;

// 설명 : 
D3DXMATRIX g_World_0 = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f };

framework::Framework_Camera g_Camera;
int x = 0, y = 0; float length = 5;

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, game::OnFrameMove)(
	_In_ double _fTime,
	_In_ float _fElapsedTime,
	_Inout_ void * _userContext)
{
	sound::g_FMOD_SoundDevice.play();

	g_GUI_Game.update();
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


		g_GUIHelper_Game.onClick_Down(i::frame::LBUTTON, _xPos, _yPos);
	}
	else if (_buttonsDown[FRAMEWORK_LEFTBUTTON] == 2)
	{
		g_GUIHelper_Game.onClick_Up(i::frame::LBUTTON, _xPos, _yPos);
	}

	if (_buttonsDown[FRAMEWORK_MIDDLEBUTTON])
	{
		g_GUIHelper_Game.onClick_Down(i::frame::WBUTTON, _xPos, _yPos);
	}
	else if (_buttonsDown[FRAMEWORK_MIDDLEBUTTON] == 2)
	{
		g_GUIHelper_Game.onClick_Up(i::frame::LBUTTON, _xPos, _yPos);
	}

	if (_buttonsDown[FRAMEWORK_RIGHTBUTTON])
	{
		g_GUIHelper_Game.onClick_Down(i::frame::RBUTTON, _xPos, _yPos);
	}
	else if (_buttonsDown[FRAMEWORK_RIGHTBUTTON] == 2)
	{
		g_GUIHelper_Game.onClick_Up(i::frame::RBUTTON, _xPos, _yPos);
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
		// layout
		IF_FAILED(hr = build_GameLayout(&g_GUI_Game,
			_backBufferSurfaceDesc.Width,
			_backBufferSurfaceDesc.Height))
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
	button->set_Mouseable(new ToEntryButtonEvent());
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
	
	//
	_container->add_Component(skillContainer, i::frame::COMPOSITION_CENTER);

	//
	_container->add_Component(button, i::frame::COMPOSITION_WEST);

	//
	_container->reform();

	return S_OK;
}