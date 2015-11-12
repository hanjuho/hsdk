#include "entrycompofactory.h"
#include "entrybuttoneventfactory.h"



using namespace entry;


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, void, build_EntryGUI)(
	_Out_ frame::Container * _container,
	_In_ float _width,
	_In_ float _height)
{
	// 모델 뷰어 컴포넌트 생성
	compo::ModelViewerCompo * modelViewer = create_ModelViewerCompo();

	// 중앙 컨테이너 생성
	frame::RenderTargetContainer<frame::MultiContainer<2>> * centerCompo =
		new frame::RenderTargetContainer<frame::MultiContainer<2>>();

	// 남쪽 컨테이너 생성
	frame::RenderTargetContainer<frame::MultiContainer<2>> * southCompo =
		new frame::RenderTargetContainer<frame::MultiContainer<2>>();

	// 동쪽 컨테이너 생성
	frame::RenderTargetContainer<frame::Container> * eastCompo =
		new frame::RenderTargetContainer<frame::Container>();

	// 레이아웃 설정
	frame::layout::BorderLayout * borderLayout = new frame::layout::BorderLayout();
	autoset_MainBorderLayout(borderLayout);

	// 컨테이널 설정
	_container->set_Visible(true);
	_container->set_W(_width);
	_container->set_H(_height);
	_container->set_Layout(borderLayout);
	_container->graphics()->set_image(L"image/background/entry.png");

	// 컴포넌트 추가
	_container->add_Component(centerCompo, i::frame::COMPOSITION_CENTER);
	_container->add_Component(eastCompo, i::frame::COMPOSITION_EAST);
	_container->add_Component(southCompo, i::frame::COMPOSITION_SOUTH);

	// 컴포넌트 설정
	build_CenterPad(centerCompo, modelViewer);
	build_SouthPad(southCompo, modelViewer);
	build_EastPad(eastCompo, centerCompo, southCompo);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, void, build_CenterPad)(
	_Out_ frame::MultiContainer<2> * _container,
	_In_ compo::ModelViewerCompo * _mvcompo)
{
	_container->reset();

	// 두번째 버퍼를 먼저 선택
	_container->select_Buffer(1);

	// 100개의 컴포넌트를 추가
	for (unsigned int index = 0; index < 54; ++index)
	{
		// 컴포넌트 생성
		frame::Component * skill_icon = new frame::Component(frame::PARENT_RELATION_RELATIVE);

		// 아이콘 클립
		float clip[4] = {
			64.0f * (float)(index % 10),
			64.0f * (float)(index / 10),
			64.0f, 64.0f };

		// 컴포넌트 설정
		skill_icon->set_Visible(true);
		skill_icon->graphics()->set_image(L"image/icon/skill.png");
		skill_icon->graphics()->set_imageDetail(clip);

		// 컴포넌트 추가
		_container->add_Component(skill_icon);
	}

	// 첫번째 버퍼를 나중에 선택
	_container->select_Buffer(0);

	// 컨테이너 레이아웃 설정
	frame::layout::BorderLayout * borderLayout = new frame::layout::BorderLayout();
	autoset_CenterBorderLayout(borderLayout);
	_container->set_Layout(borderLayout);

	// 상태 설정 컴포넌트 생성
	frame::Component * notepad = new frame::Component(
		frame::PARENT_RELATION_RELATIVE);

	notepad->set_Visible(true);
	notepad->graphics()->set_image(L"image/layout/buttonpad.png");

	// 컴포넌트 추가
	_container->add_Component(_mvcompo, i::frame::COMPOSITION_CENTER);
	_container->add_Component(notepad, i::frame::COMPOSITION_WEST);

	// 컨테이너 설정
	_container->set_Visible(true);
	_container->graphics()->set_image(L"image/layout/notepad.png");
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, void, build_SouthPad)(
	_Out_ frame::MultiContainer<2> * _container,
	_In_ compo::ModelViewerCompo * _mvcompo)
{
	_container->reset();

	// 두번째 버퍼를 먼저 선택
	_container->select_Buffer(1);

	// 4개의 컴포넌트를 추가
	for (unsigned int index = 0; index < 4; ++index)
	{
		// 컴포넌트 생성
		frame::Component * skill_icon = new frame::Component(
			frame::PARENT_RELATION_RELATIVE);

		// 컴포넌트 추가
		skill_icon->set_Visible(true);
		skill_icon->graphics()->set_Background({ 0.0f, 0.0f, 0.0f, 1.0f });
		_container->add_Component(skill_icon);
	}

	// 첫번째 버퍼를 나중에 선택
	_container->select_Buffer(0);

	// 컨테이너 레이아웃 설정
	frame::layout::GridLayout * gridLayout = new frame::layout::GridLayout(3, 1);
	autoset_SouthGridLayout0(gridLayout);
	_container->set_Layout(gridLayout);

	// 3개의 컴포넌트를 추가
	EntryButtonEventFactory factory;
	for (unsigned int index = 0; index < 3; ++index)
	{
		// 컴포넌트 생성
		frame::ButtonCompo * button = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);

		// 컴포넌트 설정
		button->set_Visible(true);
		button->set_Mouseable(factory.create_ModelChangeButtonEvent(_mvcompo, index));
		button->graphics()->set_image(L"image/layout/button.png");

		// 컴포넌트 추가
		_container->add_Component(button);
	}

	// 컨테이너 설정
	_container->set_Visible(true);
	_container->graphics()->set_image(L"image/layout/subbuttonpad.png");
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, void, build_EastPad)(
	_Out_ frame::Container * _container,
	_In_ frame::MultiContainer<2> * _centerpad,
	_In_ frame::MultiContainer<2> * _southpad)
{
	_container->reset();

	// 컴포넌트 생성
	frame::ButtonCompo * buttons[4];
	EntryButtonEventFactory factory;

	// 1 생성
	buttons[0] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
	buttons[0]->graphics()->set_image(L"image/layout/button.png");
	buttons[0]->set_Mouseable(factory.create_ViewerPadChangeButtonEvent(_centerpad, _southpad, 0));

	// 2 생성
	buttons[1] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
	buttons[1]->graphics()->set_image(L"image/layout/button.png");
	buttons[1]->set_Mouseable(factory.create_ViewerPadChangeButtonEvent(_centerpad, _southpad, 1));

	// 3 생성
	buttons[2] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
	buttons[2]->graphics()->set_image(L"image/layout/button.png");
	buttons[2]->set_Mouseable(factory.create_GoToGameButtonEvent());

	// 4 생성
	buttons[3] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
	buttons[3]->graphics()->set_image(L"image/layout/button.png");

	// 레이아웃 설정
	frame::layout::GridLayout * gridlayout = new frame::layout::GridLayout(1, 4);
	autoset_EastGridLayout(gridlayout);
	_container->set_Layout(gridlayout);

	// 4개의 컴포넌트 추가
	for (unsigned int index = 0; index < ARRAYSIZE(buttons); ++index)
	{
		// 컴포넌트 설정
		buttons[index]->set_Visible(true);
		_container->add_Component(buttons[index]);
	}

	// 컨테이너 설정
	_container->set_Visible(true);
	_container->graphics()->set_image(L"image/layout/buttonpad.png");
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, compo::ModelViewerCompo *, create_ModelViewerCompo)(
	_X_ void)
{
	// 모델 목록
	const wchar_t * modelnames[3] = {
		L"Arthas.X",
		L"DeathwingHuman.X",
		L"Deathwing.X" };

	// 모델 뷰어
	compo::ModelViewerCompo * modelView = new compo::ModelViewerCompo(
		frame::PARENT_RELATION_RELATIVE,
		L"model/", modelnames, ARRAYSIZE(modelnames));

	// 설정
	modelView->set_Visible(true);
	modelView->graphics()->set_Background({ 0.0f, 0.0f, 0.0f, 1.0f });

	// 반환
	return modelView;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, void, autoset_MainBorderLayout)(
	_Out_ i::frame::i_Layout * _layout)
{
	_layout->set_Space(
		hsdk::i::frame::SPACE_LEFT, 0.05f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_RIGHT, 0.05f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_TOP, 0.05f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_BOTTOM, 0.05f);

	_layout->set_HGap(0.03f);
	_layout->set_VGap(0.03f);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, void, autoset_CenterBorderLayout)(
	_Out_ i::frame::i_Layout * _layout)
{
	_layout->set_Space(
		hsdk::i::frame::SPACE_LEFT, 0.05f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_TOP, 0.14f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_RIGHT, 0.05f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_BOTTOM, 0.07f);

	_layout->set_HGap(0.03f);
	_layout->set_VGap(0.03f);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, void, autoset_CenterGridLayout)(
	_Out_ i::frame::i_Layout * _layout)
{
	_layout->set_Space(
		hsdk::i::frame::SPACE_LEFT, 0.05f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_TOP, 0.14f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_RIGHT, 0.05f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_BOTTOM, 0.07f);

	_layout->set_HGap(0.03f);
	_layout->set_VGap(0.03f);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, void, autoset_SouthGridLayout0)(
	_Out_ i::frame::i_Layout * _layout)
{
	_layout->set_Space(
		hsdk::i::frame::SPACE_LEFT, 0.2f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_TOP, 0.3f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_RIGHT, 0.2f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_BOTTOM, 0.3f);

	_layout->set_HGap(0.2f);
	_layout->set_VGap(0.1f);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, void, autoset_SouthGridLayout1)(
	_Out_ i::frame::i_Layout * _layout)
{
	_layout->set_Space(
		hsdk::i::frame::SPACE_LEFT, 0.2f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_TOP, 0.3f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_RIGHT, 0.2f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_BOTTOM, 0.3f);

	_layout->set_HGap(0.1f);
	_layout->set_VGap(0.1f);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, void, autoset_EastGridLayout)(
	_Out_ i::frame::i_Layout * _layout)
{
	_layout->set_Space(
		hsdk::i::frame::SPACE_LEFT, 0.15f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_TOP, 0.1f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_RIGHT, 0.15f);
	_layout->set_Space(
		hsdk::i::frame::SPACE_BOTTOM, 0.1f);

	_layout->set_HGap(0.2f);
	_layout->set_VGap(0.1f);
}
