#include "entrybuttoneventfactory.h"
#include "gotogamebuttonevent.h"
#include "modelchangebuttonevent.h"
#include "viewerpadchangebuttonevent.h"



using namespace entryloop;


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryButtonEventFactory, i::frame::MouseableAdapter *, create_GoToGameButtonEvent)(
	_X_ void)
{
	return new compoevent::GoToGameButtonEvent();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryButtonEventFactory, i::frame::MouseableAdapter *, create_ModelChangeButtonEvent)(
	_In_ compo::ModelViewerCompo * _mvcompo,
	_In_ unsigned int _index)
{
	return new compoevent::ModelChangeButtonEvent(_mvcompo, _index);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(EntryButtonEventFactory, i::frame::MouseableAdapter *, create_ViewerPadChangeButtonEvent)(
	_In_ frame::MultiContainer<2> * _centerpad,
	_In_ frame::MultiContainer<2> * _southpad,
	_In_ unsigned int _index)
{
	return new compoevent::ViewerPadChangeButtonEvent(_centerpad, _southpad, _index);
}