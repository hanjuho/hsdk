#include "entrycompofactory.h"
#include "entrybuttoneventfactory.h"



using namespace entry;


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, void, build_EntryGUI)(
	_Out_ frame::Container * _container,
	_In_ float _width,
	_In_ float _height)
{
	// �� ��� ������Ʈ ����
	compo::ModelViewerCompo * modelViewer = create_ModelViewerCompo();

	// �߾� �����̳� ����
	frame::RenderTargetContainer<frame::MultiContainer<2>> * centerCompo =
		new frame::RenderTargetContainer<frame::MultiContainer<2>>();

	// ���� �����̳� ����
	frame::RenderTargetContainer<frame::MultiContainer<2>> * southCompo =
		new frame::RenderTargetContainer<frame::MultiContainer<2>>();

	// ���� �����̳� ����
	frame::RenderTargetContainer<frame::Container> * eastCompo =
		new frame::RenderTargetContainer<frame::Container>();

	// ���̾ƿ� ����
	frame::layout::BorderLayout * borderLayout = new frame::layout::BorderLayout();
	autoset_MainBorderLayout(borderLayout);

	// �����̳� ����
	_container->set_Visible(true);
	_container->set_W(_width);
	_container->set_H(_height);
	_container->set_Layout(borderLayout);
	_container->graphics()->set_image(L"image/background/entry.png");

	// ������Ʈ �߰�
	_container->add_Component(centerCompo, i::frame::COMPOSITION_CENTER);
	_container->add_Component(eastCompo, i::frame::COMPOSITION_EAST);
	_container->add_Component(southCompo, i::frame::COMPOSITION_SOUTH);

	// ������Ʈ ����
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

	// �ι�° ���۸� ���� ����
	_container->select_Buffer(1);

	// 100���� ������Ʈ�� �߰�
	for (unsigned int index = 0; index < 54; ++index)
	{
		// ������Ʈ ����
		frame::Component * skill_icon = new frame::Component(frame::PARENT_RELATION_RELATIVE);

		// ������ Ŭ��
		float clip[4] = {
			64.0f * (float)(index % 10),
			64.0f * (float)(index / 10),
			64.0f, 64.0f };

		// ������Ʈ ����
		skill_icon->set_Visible(true);
		skill_icon->graphics()->set_image(L"image/icon/skill.png");
		skill_icon->graphics()->set_imageDetail(clip);

		// ������Ʈ �߰�
		_container->add_Component(skill_icon);
	}

	// ù��° ���۸� ���߿� ����
	_container->select_Buffer(0);

	// �����̳� ���̾ƿ� ����
	frame::layout::BorderLayout * borderLayout = new frame::layout::BorderLayout();
	autoset_CenterBorderLayout(borderLayout);
	_container->set_Layout(borderLayout);

	// ���� ���� ������Ʈ ����
	frame::Component * notepad = new frame::Component(
		frame::PARENT_RELATION_RELATIVE);

	notepad->set_Visible(true);
	notepad->graphics()->set_image(L"image/layout/buttonpad.png");

	// ������Ʈ �߰�
	_container->add_Component(_mvcompo, i::frame::COMPOSITION_CENTER);
	_container->add_Component(notepad, i::frame::COMPOSITION_WEST);

	// �����̳� ����
	_container->set_Visible(true);
	_container->graphics()->set_image(L"image/layout/notepad.png");
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, void, build_SouthPad)(
	_Out_ frame::MultiContainer<2> * _container,
	_In_ compo::ModelViewerCompo * _mvcompo)
{
	_container->reset();

	// �ι�° ���۸� ���� ����
	_container->select_Buffer(1);

	// 4���� ������Ʈ�� �߰�
	for (unsigned int index = 0; index < 4; ++index)
	{
		// ������Ʈ ����
		frame::Component * skill_icon = new frame::Component(
			frame::PARENT_RELATION_RELATIVE);

		// ������Ʈ �߰�
		skill_icon->set_Visible(true);
		skill_icon->graphics()->set_Background({ 0.0f, 0.0f, 0.0f, 1.0f });
		_container->add_Component(skill_icon);
	}

	// ù��° ���۸� ���߿� ����
	_container->select_Buffer(0);

	// �����̳� ���̾ƿ� ����
	frame::layout::GridLayout * gridLayout = new frame::layout::GridLayout(3, 1);
	autoset_SouthGridLayout0(gridLayout);
	_container->set_Layout(gridLayout);

	// 3���� ������Ʈ�� �߰�
	EntryButtonEventFactory factory;
	for (unsigned int index = 0; index < 3; ++index)
	{
		// ������Ʈ ����
		frame::ButtonCompo * button = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);

		// ������Ʈ ����
		button->set_Visible(true);
		button->set_Mouseable(factory.create_ModelChangeButtonEvent(_mvcompo, index));
		button->graphics()->set_image(L"image/layout/button.png");

		// ������Ʈ �߰�
		_container->add_Component(button);
	}

	// �����̳� ����
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

	// ������Ʈ ����
	frame::ButtonCompo * buttons[4];
	EntryButtonEventFactory factory;

	// 1 ����
	buttons[0] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
	buttons[0]->graphics()->set_image(L"image/layout/button.png");
	buttons[0]->set_Mouseable(factory.create_ViewerPadChangeButtonEvent(_centerpad, _southpad, 0));

	// 2 ����
	buttons[1] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
	buttons[1]->graphics()->set_image(L"image/layout/button.png");
	buttons[1]->set_Mouseable(factory.create_ViewerPadChangeButtonEvent(_centerpad, _southpad, 1));

	// 3 ����
	buttons[2] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
	buttons[2]->graphics()->set_image(L"image/layout/button.png");
	buttons[2]->set_Mouseable(factory.create_GoToGameButtonEvent());

	// 4 ����
	buttons[3] = new frame::ButtonCompo(frame::PARENT_RELATION_RELATIVE);
	buttons[3]->graphics()->set_image(L"image/layout/button.png");

	// ���̾ƿ� ����
	frame::layout::GridLayout * gridlayout = new frame::layout::GridLayout(1, 4);
	autoset_EastGridLayout(gridlayout);
	_container->set_Layout(gridlayout);

	// 4���� ������Ʈ �߰�
	for (unsigned int index = 0; index < ARRAYSIZE(buttons); ++index)
	{
		// ������Ʈ ����
		buttons[index]->set_Visible(true);
		_container->add_Component(buttons[index]);
	}

	// �����̳� ����
	_container->set_Visible(true);
	_container->graphics()->set_image(L"image/layout/buttonpad.png");
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, compo::ModelViewerCompo *, create_ModelViewerCompo)(
	_X_ void)
{
	// �� ���
	const wchar_t * modelnames[3] = {
		L"Arthas.X",
		L"DeathwingHuman.X",
		L"Deathwing.X" };

	// �� ���
	compo::ModelViewerCompo * modelView = new compo::ModelViewerCompo(
		frame::PARENT_RELATION_RELATIVE,
		L"model/", modelnames, ARRAYSIZE(modelnames));

	// ����
	modelView->set_Visible(true);
	modelView->graphics()->set_Background({ 0.0f, 0.0f, 0.0f, 1.0f });

	// ��ȯ
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
