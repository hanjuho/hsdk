#include "entry.h"
#include "game.h"
#include "modelviewcompo.h"
#include <hsdk/win/frame/layout/borderlayout.h>
#include <hsdk/win/frame/layout/flowlayout.h>


//--------------------------------------------------------------------------------------
// Grobal Variable
//--------------------------------------------------------------------------------------

// 설명 : 
frame::Container g_GUI_Entry;

// 설명 :
FMOD::Sound * g_Sound_Background0;
FMOD::Channel * g_Sound_Controller0;

//--------------------------------------------------------------------------------------
class GoGameButtonEvent :
	public i::frame::MouseableAdapter
{
public:

	INTERFACE_DECL_FUNC_T(void, onClick_Up)(
		_In_ i::frame::MOUSE_BUTTON _button,
		_In_ int _x,
		_In_ int _y)
	{
		game::initialize_Game(nullptr);
	}

};

//--------------------------------------------------------------------------------------
class ModelViewChangeButtonEvent :
	public i::frame::MouseableAdapter
{
public:

	CLASS_DECL_CONSTRUCTOR(ModelViewChangeButtonEvent)(
		_In_ gamecompo::ModelViewCompo * _mvcompo,
		_In_ unsigned int _viewNumber) :
		my_MVCompo(_mvcompo), my_ViewNumber(_viewNumber), my_AnimNumber(0)
	{

	}

	INTERFACE_DECL_FUNC_T(void, onClick_Up)(
		_In_ i::frame::MOUSE_BUTTON _button,
		_In_ int _x,
		_In_ int _y)
	{
		my_MVCompo->select_Model(my_ViewNumber, my_AnimNumber++);
	}

private:

	// 설명 : 
	gamecompo::ModelViewCompo * my_MVCompo;

	// 설명 : 
	const unsigned int my_ViewNumber;

	// 설명 : 
	unsigned int my_AnimNumber;

};

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, entry::OnFrameMove)(
	_In_ double _fTime,
	_In_ float _fElapsedTime,
	_Inout_ void * _userContext)
{
	sound::g_FMOD_SoundDevice.play();

	g_GUI_Entry.update();
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, entry::OnD3D10FrameRender)(
	_In_ ID3D10Device * _d3dDevice,
	_In_ double _fTime,
	_In_ float _fElapsedTime,
	_Inout_ void * _userContext)
{
	// Clear the render target and depth stencil
	float ClearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	ID3D10RenderTargetView * pRTV = framework::g_Framework_Device.d3d10RTV;
	_d3dDevice->ClearRenderTargetView(pRTV, ClearColor);
	ID3D10DepthStencilView * pDSV = framework::g_Framework_Device.d3d10DSV;
	_d3dDevice->ClearDepthStencilView(pDSV, D3D10_CLEAR_DEPTH, 1.0, 0);

	g_GUI_Entry.render();
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(LRESULT, entry::OnMsgProc)(
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
		g_GUI_Entry.set_W((float)LOWORD(_lParam));
		g_GUI_Entry.set_H((float)HIWORD(_lParam));
		g_GUI_Entry.reform();
	}

	return 0;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, entry::OnMouse)(
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
IMPL_FUNC_T(void, entry::OnKeyboard)(
	_In_ unsigned char _nKey,
	_In_ short _bKeyDown,
	_In_ short _bAltDown,
	_Inout_ void * _userContext)
{

}

//--------------------------------------------------------------------------------------
IMPL_FUNC(entry::OnD3D10CreateDevice)(
	_In_ ID3D10Device * _d3dDevice,
	_In_ const DXGI_SURFACE_DESC & _backBufferSurfaceDesc,
	_Inout_ void * _userContext)
{
	HRESULT hr = E_FAIL;
	IF_SUCCEEDED(hr = common::initialize_Common(&g_GUI_Entry))
	{
		// layout
		IF_FAILED(hr = build_EntryLayout(&g_GUI_Entry,
			(float)_backBufferSurfaceDesc.Width,
			(float)_backBufferSurfaceDesc.Height))
		{
			return hr;
		}

		// sound
		IF_FAILED(hr = sound::g_FMOD_SoundDevice.load_WaveFile(
			&g_Sound_Background0,
			&g_Sound_Controller0,
			L"sound/entry_background.wav"))
		{
			return hr;
		}

		FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

		g_Sound_Controller0->setMode(FMOD_LOOP_NORMAL);
		g_Sound_Controller0->set3DAttributes(&pos, &vel);
	}

	return hr;
}

//--------------------------------------------------------------------------------------
DECL_FUNC_T(void, entry::OnD3D10DestroyDevice)(
	_Inout_ void * _userContext)
{
	common::destroy_Common();

	g_GUI_Entry.clear();
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(entry::initialize_Entry)(
	_In_ void * _context)
{
	if (framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc)
	{
		framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc(
			framework::g_Framework_Callbacks.d3d10DeviceDestroyedFuncUserContext);
	}

	framework::g_Framework_Callbacks.windowMsgFunc = entry::OnMsgProc;
	framework::g_Framework_Callbacks.mouseFunc = entry::OnMouse;
	framework::g_Framework_Callbacks.keyboardFunc = entry::OnKeyboard;
	framework::g_Framework_Callbacks.frameMoveFunc = entry::OnFrameMove;

	framework::g_Framework_Callbacks.d3d10DeviceCreatedFunc = entry::OnD3D10CreateDevice;
	framework::g_Framework_Callbacks.d3d10DeviceDestroyedFunc = entry::OnD3D10DestroyDevice;
	framework::g_Framework_Callbacks.d3d10FrameRenderFunc = entry::OnD3D10FrameRender;

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
IMPL_FUNC(entry::build_EntryLayout)(
	_In_ frame::Container * _container,
	_In_ float _width,
	_In_ float _height)
{
	_container->clear();

	// 상태 정보 레이아웃
	frame::layout::BorderLayout * borderLayout_0 = new frame::layout::BorderLayout();

	borderLayout_0->set_Space(
		hsdk::i::frame::SPACE_LEFT, 0.05f);
	borderLayout_0->set_Space(
		hsdk::i::frame::SPACE_TOP, 0.14f);
	borderLayout_0->set_Space(
		hsdk::i::frame::SPACE_RIGHT, 0.05f);
	borderLayout_0->set_Space(
		hsdk::i::frame::SPACE_BOTTOM, 0.07f);

	borderLayout_0->set_HGap(0.03f);
	borderLayout_0->set_VGap(0.03f);

	// 상태 정보 컨테이너
	frame::RenderTargetContainer * stateLayout = new frame::RenderTargetContainer();

	stateLayout->set_Visible(true);
	stateLayout->set_X(0.0f);
	stateLayout->set_Y(0.0f);
	stateLayout->set_W(_width);
	stateLayout->set_H(_height);
	stateLayout->set_Layout(borderLayout_0);
	stateLayout->graphics()->set_image(L"image/layout/notepad.png");

	// 모델 뷰어
	gamecompo::ModelViewCompo * modelView = nullptr;
	{
		// 1
		try
		{
			// 모델 목록
			const wchar_t * modelnames[3] = {
				L"Arthas.X",
				L"DeathwingHuman.X",
				L"Deathwing.X" };

			modelView = new gamecompo::ModelViewCompo(
				frame::PARENT_RELATION_RELATIVE,
				L"model/", modelnames, ARRAYSIZE(modelnames));

			modelView->set_X(0.0f);
			modelView->set_Y(0.0f);
			modelView->set_Visible(true);
			modelView->graphics()->set_Background({ 0.0f, 0.0f, 0.0f, 1.0f });

			// 추가
			stateLayout->add_Component(modelView, i::frame::COMPOSITION_CENTER);
		}
		catch (...)
		{

		}

		// 2
		{
			// text layout
			frame::Component * notepad = new frame::Component(
				frame::PARENT_RELATION_RELATIVE);

			notepad->set_X(0.0f);
			notepad->set_Y(0.0f);
			notepad->set_Visible(true);
			notepad->graphics()->set_image(L"image/layout/buttonpad.png");

			// 추가
			stateLayout->add_Component(notepad, i::frame::COMPOSITION_WEST);
		}
	}

	// 주 버튼 레이아웃
	frame::layout::FlowLayout * flowlayout_0 = new frame::layout::FlowLayout(frame::layout::FLOW_VERTICAL);

	flowlayout_0->set_Space(
		hsdk::i::frame::SPACE_LEFT, 0.15f);
	flowlayout_0->set_Space(
		hsdk::i::frame::SPACE_TOP, 0.1f);
	flowlayout_0->set_Space(
		hsdk::i::frame::SPACE_RIGHT, 0.0f);
	flowlayout_0->set_Space(
		hsdk::i::frame::SPACE_BOTTOM, 0.0f);

	flowlayout_0->set_HGap(0.2f);
	flowlayout_0->set_VGap(0.1f);

	flowlayout_0->set_EachWidthAbs(256.0f);
	flowlayout_0->set_EachHeightAbs(96.0f);

	// 버튼 컨테이너
	frame::RenderTargetContainer * buttonContainer = new frame::RenderTargetContainer();

	buttonContainer->set_Visible(true);
	buttonContainer->set_X(0.0f);
	buttonContainer->set_Y(0.0f);
	buttonContainer->set_W(_width);
	buttonContainer->set_H(_height);
	buttonContainer->set_Layout(flowlayout_0);
	buttonContainer->graphics()->set_image(L"image/layout/buttonpad.png");

	{
		// 구성 요소
		frame::ButtonCompo * buttons[4];

		// 1
		buttons[0] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
		buttons[0]->graphics()->set_image(L"image/layout/button.png");

		// 2
		buttons[1] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
		buttons[1]->graphics()->set_image(L"image/layout/button.png");
		buttons[1]->set_Mouseable(new GoGameButtonEvent());

		// 3
		buttons[2] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
		buttons[2]->graphics()->set_image(L"image/layout/button.png");

		// 4
		buttons[3] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
		buttons[3]->graphics()->set_image(L"image/layout/button.png");

		for (unsigned int index = 0; index < ARRAYSIZE(buttons); ++index)
		{
			buttons[index]->set_X(0.0f);
			buttons[index]->set_Y(0.0f);
			buttons[index]->set_Visible(true);
			buttonContainer->add_Component(buttons[index]);
		}
	}

	// 보조 버튼 레이아웃
	frame::layout::FlowLayout * flowlayout = new frame::layout::FlowLayout(frame::layout::FLOW_HORIZON);

	flowlayout->set_Space(
		hsdk::i::frame::SPACE_LEFT, 0.2f);
	flowlayout->set_Space(
		hsdk::i::frame::SPACE_TOP, 0.3f);
	flowlayout->set_Space(
		hsdk::i::frame::SPACE_RIGHT, 0.0f);
	flowlayout->set_Space(
		hsdk::i::frame::SPACE_BOTTOM, 0.0f);

	flowlayout->set_HGap(0.2f);
	flowlayout->set_VGap(0.1f);

	flowlayout->set_EachWidthAbs(64.0f);
	flowlayout->set_EachHeightAbs(48.0f);

	// 보조 버튼 컨테이너
	frame::RenderTargetContainer * subButtonContainer = new frame::RenderTargetContainer();

	subButtonContainer->set_Visible(true);
	subButtonContainer->set_X(0.0f);
	subButtonContainer->set_Y(0.0f);
	subButtonContainer->set_W(_width);
	subButtonContainer->set_H(_height);
	subButtonContainer->set_Layout(flowlayout);
	subButtonContainer->graphics()->set_image(L"image/layout/subbuttonpad.png");

	{
		// 구성 요소
		frame::ButtonCompo * buttons[3];

		// 1
		buttons[0] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
		buttons[0]->set_Mouseable(new ModelViewChangeButtonEvent(modelView, 0));
		buttons[0]->graphics()->set_image(L"image/layout/button.png");

		// 2
		buttons[1] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
		buttons[1]->set_Mouseable(new ModelViewChangeButtonEvent(modelView, 1));
		buttons[1]->graphics()->set_image(L"image/layout/button.png");

		// 3
		buttons[2] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
		buttons[2]->set_Mouseable(new ModelViewChangeButtonEvent(modelView, 2));
		buttons[2]->graphics()->set_image(L"image/layout/button.png");

		for (unsigned int index = 0; index < ARRAYSIZE(buttons); ++index)
		{
			buttons[index]->set_X(0.0f);
			buttons[index]->set_Y(0.0f);
			buttons[index]->set_Visible(true);
			subButtonContainer->add_Component(buttons[index]);
		}
	}

	// 메인 레이아웃
	frame::layout::BorderLayout * borderLayout = new frame::layout::BorderLayout();

	borderLayout->set_Space(
		hsdk::i::frame::SPACE_LEFT, 0.05f);
	borderLayout->set_Space(
		hsdk::i::frame::SPACE_RIGHT, 0.05f);
	borderLayout->set_Space(
		hsdk::i::frame::SPACE_TOP, 0.05f);
	borderLayout->set_Space(
		hsdk::i::frame::SPACE_BOTTOM, 0.05f);

	borderLayout->set_HGap(0.03f);
	borderLayout->set_VGap(0.03f);

	// 메인 컨테이너
	_container->set_Visible(true);
	_container->set_X(0.0f);
	_container->set_Y(0.0f);
	_container->set_W(_width);
	_container->set_H(_height);
	_container->set_Layout(borderLayout);
	_container->graphics()->set_image(L"image/background/entry.png");

	// 레이아웃 추가
	_container->add_Component(stateLayout, i::frame::COMPOSITION_CENTER);

	// 레이아웃 추가
	_container->add_Component(buttonContainer, i::frame::COMPOSITION_EAST);

	// 레이아웃 추가
	_container->add_Component(subButtonContainer, i::frame::COMPOSITION_SOUTH);

	//
	_container->reform();

	return S_OK;
}