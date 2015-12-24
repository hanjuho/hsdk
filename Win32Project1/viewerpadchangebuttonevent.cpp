#include "viewerpadchangebuttonevent.h"
#include "entrycompofactory.h"



using namespace compoevent;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(ViewerPadChangeButtonEvent, ViewerPadChangeButtonEvent)(
	_In_ frame::MultiContainer<2> * _centerpad,
	_In_ frame::MultiContainer<2> * _southpad,
	_In_ unsigned int _number) :
	my_Centerpad(_centerpad), my_Southpad(_southpad), my_Number(_number)
{
	if (nullptr == _centerpad || nullptr == _southpad)
	{
		throw E_INVALIDARG;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ViewerPadChangeButtonEvent, void, onClick_Up)(
	_In_ i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	if (my_Centerpad->get_CurrentBuffer() == my_Number &&
		my_Southpad->get_CurrentBuffer() == my_Number)
	{
		return;
	}

	my_Centerpad->select_Buffer(my_Number);
	my_Southpad->select_Buffer(my_Number);

	entryloop::EntryCompoFactory factory;

	switch (my_Number)
	{
	case 0:
	{
			  // 센터 레이아웃
			  frame::layout::BorderLayout * borderLayout_Center = new frame::layout::BorderLayout();
			  factory.autoset_CenterBorderLayout(borderLayout_Center);

			  // 남쪽 레이아웃
			  frame::layout::GridLayout * gridLayout_South = new frame::layout::GridLayout(3, 1);
			  factory.autoset_SouthGridLayout0(gridLayout_South);

			  my_Centerpad->set_Layout(borderLayout_Center);
			  my_Southpad->set_Layout(gridLayout_South);
	}
		break;

	case 1:
	{
			  // 센터 레이아웃
			  frame::layout::GridLayout * gridLayout_Center = new frame::layout::GridLayout(10, 6);
			  factory.autoset_CenterGridLayout(gridLayout_Center);

			  // 남쪽 레이아웃
			  frame::layout::GridLayout * gridLayout_South = new frame::layout::GridLayout(4, 1);
			  factory.autoset_SouthGridLayout1(gridLayout_South);

			  my_Centerpad->set_Layout(gridLayout_Center);
			  my_Southpad->set_Layout(gridLayout_South);
	}
		break;

	}

	my_Centerpad->reform();
	my_Centerpad->redraw();

	my_Southpad->reform();
	my_Southpad->redraw();
}