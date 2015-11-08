#pragma once



#include "common.h"
#include "modelviewercompo.h"



namespace compoevent
{

	// 설명 : 
	DECL_CLASS(ModelChangeButtonEvent) :
		public i::frame::MouseableAdapter
	{
	public:

		// 설명 : 
		CLASS_DECL_CONSTRUCTOR(ModelChangeButtonEvent)(
			_In_ compo::ModelViewerCompo * _mvcompo,
			_In_ unsigned int _index);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, onClick_Up)(
			_In_ i::frame::MOUSE_BUTTON _button,
			_In_ int _x,
			_In_ int _y);

	private:

		// 설명 : 
		compo::ModelViewerCompo * const my_MVCompo;
		
		// 설명 : 
		const unsigned int my_ModelNumber;

		// 설명 : 
		unsigned int my_AnimNumber;

	};

}