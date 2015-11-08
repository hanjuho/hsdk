#pragma once



#include "common.h"



namespace compoevent
{

	// ���� : 
	DECL_CLASS(ViewerPadChangeButtonEvent) :
		public i::frame::MouseableAdapter
	{
	public:

		// ���� : 
		CLASS_DECL_CONSTRUCTOR(ViewerPadChangeButtonEvent)(
			_In_ frame::MultiContainer<2> * _centerpad,
			_In_ frame::MultiContainer<2> * _southpad,
			_In_ unsigned int _number);

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, onClick_Up)(
			_In_ i::frame::MOUSE_BUTTON _button,
			_In_ int _x,
			_In_ int _y);

	private:

		// ���� : 
		frame::MultiContainer<2> * const my_Centerpad;

		// ���� : 
		frame::MultiContainer<2> * const my_Southpad;

		// ���� : 
		const unsigned int my_Number;

	};

}