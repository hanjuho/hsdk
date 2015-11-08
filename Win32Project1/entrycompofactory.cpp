#include "entrycompofactory.h"



using namespace entry;


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, frame::MultiContainer<2> *, create_CenterPad)(
	_X_ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, frame::MultiContainer<2> *, create_SouthPad)(
	_X_ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryCompoFactory, frame::Container *, create_EasePad)(
	_X_ void)
{

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