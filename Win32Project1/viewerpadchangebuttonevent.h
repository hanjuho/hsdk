#pragma once



#include "common.h"



namespace compoevent
{

	// 설명 : 
	DECL_CLASS(ViewerPadChangeButtonEvent) :
		public i::frame::MouseableAdapter
	{
	public:

		// 설명 : 
		CLASS_DECL_CONSTRUCTOR(ViewerPadChangeButtonEvent)(
			_In_ frame::MultiContainer<2> * _centerpad,
			_In_ frame::MultiContainer<2> * _southpad,
			_In_ unsigned int _number);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, onClick_Up)(
			_In_ i::frame::MOUSE_BUTTON _button,
			_In_ int _x,
			_In_ int _y);

	private:

		// 설명 : 
		frame::MultiContainer<2> * const my_Centerpad;

		// 설명 : 
		frame::MultiContainer<2> * const my_Southpad;

		// 설명 : 
		const unsigned int my_Number;

	};

}