#include "modelchangebuttonevent.h"



using namespace compoevent;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(ModelChangeButtonEvent, ModelChangeButtonEvent)(
	_In_ compo::ModelViewerCompo * _mvcompo,
	_In_ unsigned int _number) :
	my_MVCompo(_mvcompo), my_ModelNumber(_number), my_AnimNumber(0)
{
	if (nullptr == _mvcompo)
	{
		throw E_INVALIDARG;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelChangeButtonEvent, void, onClick_Up)(
	_In_ i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	if (my_MVCompo->get_NumModel() != my_ModelNumber)
	{
		my_AnimNumber = 0;
	}

	my_MVCompo->select_Model(my_ModelNumber, my_AnimNumber++);
}