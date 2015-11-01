#include "entry.h"
#include "game.h"
#include "modelviewcompo.h"
#include <hsdk/win/frame/layout/borderlayout.h>
#include <hsdk/win/frame/layout/flowlayout.h>


class ToGameButtonEvent :
	public common::MouseableAdapter
{
public:

	INTERFACE_DECL_FUNC_T(void, onClick_Up)(
		_In_ i::frame::MOUSE_BUTTON _button,
		_In_ int _x,
		_In_ int _y)
	{
		game::initialize();
	}

};

//--------------------------------------------------------------------------------------
// Grobal Variable
//--------------------------------------------------------------------------------------

// 설명 : 
frame::Container g_GUI_Entry;

// 설명 : 
frame::inputEventHelper g_GUIHelper_Entry(&g_GUI_Entry);

// 설명 :
FMOD::Sound * g_Sound_Background0;
FMOD::Channel * g_Sound_Controller0;

// 설명 :
int mouse_X = 0, mouse_Y = 0;

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, entry::initialize)(
	_X_ void)
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
			framework::g_Framework_Callbacks.d3d10DeviceCreatedFuncUserContext))
		{
			MessageBox(framework::g_Framework_Window.hwnd, L"초기화에 실패했습니다.\n코드를 확인해주세요", L"Error", MB_OK);
			framework::g_Framework.shutdown();
		}
	}
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
	int dx = _xPos - mouse_X;
	int dy = _yPos - mouse_Y;
	bool moved = false;

	g_GUIHelper_Entry.onMove(dx, dy);

	if (0 < dx || 0 < dy)
	{
		moved = true;
	}

	if (_buttonsDown[FRAMEWORK_LEFTBUTTON] == 1)
	{
		g_GUIHelper_Entry.onClick_Down(i::frame::LBUTTON, _xPos, _yPos);

		if (moved)
		{
			g_GUIHelper_Entry.onDrag(i::frame::LBUTTON, _xPos - mouse_X, _yPos - mouse_Y);
		}
	}
	else if (_buttonsDown[FRAMEWORK_LEFTBUTTON] == 2)
	{
		g_GUIHelper_Entry.onClick_Up(i::frame::LBUTTON, _xPos, _yPos);
	}

	if (_buttonsDown[FRAMEWORK_MIDDLEBUTTON])
	{
		g_GUIHelper_Entry.onClick_Down(i::frame::WBUTTON, _xPos, _yPos);
	}
	else if (_buttonsDown[FRAMEWORK_MIDDLEBUTTON] == 2)
	{
		g_GUIHelper_Entry.onClick_Up(i::frame::LBUTTON, _xPos, _yPos);
	}

	if (_buttonsDown[FRAMEWORK_RIGHTBUTTON])
	{
		g_GUIHelper_Entry.onClick_Down(i::frame::RBUTTON, _xPos, _yPos);
	}
	else if (_buttonsDown[FRAMEWORK_RIGHTBUTTON] == 2)
	{
		g_GUIHelper_Entry.onClick_Up(i::frame::LBUTTON, _xPos, _yPos);
	}

	mouse_X = _xPos;
	mouse_Y = _yPos;
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
IMPL_FUNC_T(void, entry::OnFrameMove)(
	_In_ double _fTime,
	_In_ float _fElapsedTime,
	_Inout_ void * _userContext)
{
	sound::g_FMOD_SoundDevice.play();
	g_GUI_Entry.update();
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(entry::OnD3D10CreateDevice)(
	_In_ ID3D10Device * _d3dDevice,
	_In_ const DXGI_SURFACE_DESC & _backBufferSurfaceDesc,
	_Inout_ void * _userContext)
{
	HRESULT hr = E_FAIL;

	IF_SUCCEEDED(hr = common::initialize_Common())
	{
		float w = (float)_backBufferSurfaceDesc.Width;
		float h = (float)_backBufferSurfaceDesc.Height;

		// 메인
		{
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
			g_GUI_Entry.set_Visible(true);
			g_GUI_Entry.set_X(0.0f);
			g_GUI_Entry.set_Y(0.0f);
			g_GUI_Entry.set_W(w);
			g_GUI_Entry.set_H(h);
			g_GUI_Entry.set_Layout(borderLayout);
			g_GUI_Entry.graphics()->set_image(L"image/background/entry.png");
		}

		// 상태 정보
		{
			// 상태 정보 레이아웃
			frame::layout::BorderLayout * borderLayout = new frame::layout::BorderLayout();

			borderLayout->set_Space(
				hsdk::i::frame::SPACE_LEFT, 0.05f);
			borderLayout->set_Space(
				hsdk::i::frame::SPACE_TOP, 0.07f);
			borderLayout->set_Space(
				hsdk::i::frame::SPACE_RIGHT, 0.05f);
			borderLayout->set_Space(
				hsdk::i::frame::SPACE_BOTTOM, 0.07f);

			borderLayout->set_HGap(0.03f);
			borderLayout->set_VGap(0.03f);

			// 상태 정보 컨테이너
			frame::RenderTargetContainer * stateLayout = new frame::RenderTargetContainer();

			stateLayout->set_Visible(true);
			stateLayout->set_X(0.0f);
			stateLayout->set_Y(0.0f);
			stateLayout->set_W(w);
			stateLayout->set_H(h);
			stateLayout->set_Layout(borderLayout);
			stateLayout->graphics()->set_image(L"image/layout/notepad.png");

			// 버튼 컨테이너 구성 요소
			try
			{
				// 모델 목록
				const wchar_t * modelnames[3] = {
					L"Arthas.X",
					L"DeathwingHuman.X",
					L"tiny_4anim.X" };

				// 모델 뷰어
				gamecompo::ModelViewCompo * modelView = new gamecompo::ModelViewCompo(
					frame::PARENT_RELATION_RELATIVE,
					L"model/", modelnames, ARRAYSIZE(modelnames));
				modelView->set_Visible(true);
				modelView->graphics()->set_Background({ 0.0f, 1.0f, 0.0f, 1.0f });

				// 추가
				stateLayout->add_Component(modelView, i::frame::COMPOSITION_CENTER);
			}
			catch (HRESULT hr)
			{
				return hr;
			}

			{
				// text 창
				frame::Component * notepad = new frame::Component(
					frame::PARENT_RELATION_RELATIVE);

				notepad->set_Visible(true);
				notepad->graphics()->set_image(L"image/layout/buttonpad.png");

				// 추가
				stateLayout->add_Component(notepad, i::frame::COMPOSITION_WEST);
			}

			// 레이아웃 추가
			g_GUI_Entry.add_Component(stateLayout, i::frame::COMPOSITION_CENTER);
		}

		// 주 버튼
		{
			// 주 버튼 레이아웃
			frame::layout::FlowLayout * flowlayout = new frame::layout::FlowLayout(frame::layout::FLOW_VERTICAL);

			// 레이아웃 설정
			flowlayout->set_Space(
				hsdk::i::frame::SPACE_LEFT, 0.15f);
			flowlayout->set_Space(
				hsdk::i::frame::SPACE_TOP, 0.2f);
			flowlayout->set_Space(
				hsdk::i::frame::SPACE_RIGHT, 0.0f);
			flowlayout->set_Space(
				hsdk::i::frame::SPACE_BOTTOM, 0.0f);

			flowlayout->set_HGap(0.2f);
			flowlayout->set_VGap(0.1f);

			flowlayout->set_EachWidthAbs(256.0f);
			flowlayout->set_EachHeightAbs(96.0f);

			// 버튼 컨테이너
			frame::RenderTargetContainer * buttonContainer = new frame::RenderTargetContainer();

			buttonContainer->set_Visible(true);
			buttonContainer->set_W(w);
			buttonContainer->set_H(h);
			buttonContainer->set_Layout(flowlayout);
			buttonContainer->graphics()->set_image(L"image/layout/buttonpad.png");

			{
				// 구성 요소
				frame::ButtonCompo * buttons[4];

				// 
				buttons[0] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
				buttons[0]->graphics()->set_image(L"image/layout/button.png");

				// 
				buttons[1] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
				buttons[1]->graphics()->set_image(L"image/layout/button.png");
				buttons[1]->set_Mouseable(new ToGameButtonEvent());

				// 
				buttons[2] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
				buttons[2]->graphics()->set_image(L"image/layout/button.png");

				// 
				buttons[3] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
				buttons[3]->graphics()->set_image(L"image/layout/button.png");

				for (unsigned int index = 0; index < 4; ++index)
				{
					buttons[index]->set_Visible(true);
					buttonContainer->add_Component(buttons[index]);
				}
			}

			// 레이아웃 추가
			g_GUI_Entry.add_Component(buttonContainer, i::frame::COMPOSITION_EAST);

		}

		// 보조 버튼
		{
			// 보조 버튼 레이아웃
			frame::layout::FlowLayout * flowlayout = new frame::layout::FlowLayout(frame::layout::FLOW_HORIZON);

			// 보조 버튼 컨테이너
			frame::RenderTargetContainer * subButtonContainer = new frame::RenderTargetContainer();

			subButtonContainer->set_Visible(true);
			subButtonContainer->set_W(w);
			subButtonContainer->set_H(h);
			subButtonContainer->set_Layout(flowlayout);
			subButtonContainer->graphics()->set_image(L"image/layout/subbuttonpad.png");

			// 구성 요소

			// 레이아웃 추가
			g_GUI_Entry.add_Component(subButtonContainer, i::frame::COMPOSITION_SOUTH);
		}

		// 이후에 WM_SIZE에서 호출됨
		// g_GUI_Entry.reform();

		// sound
		IF_SUCCEEDED(hr = sound::g_FMOD_SoundDevice.load_WaveFile(
			&g_Sound_Background0,
			&g_Sound_Controller0,
			L"sound/entry_background.wav"))
		{
			FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
			FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

			g_Sound_Controller0->setMode(FMOD_LOOP_NORMAL);
			g_Sound_Controller0->set3DAttributes(&pos, &vel);
		}
	}

	return hr;
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
DECL_FUNC_T(void, entry::OnD3D10DestroyDevice)(
	_Inout_ void * _userContext)
{
	common::destroy_Common();

	g_GUI_Entry.clear();
}