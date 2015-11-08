#pragma once



#include "common.h"
#include "modelviewercompo.h"



namespace compoevent
{

	// ���� : 
	DECL_CLASS(ModelChangeButtonEvent) :
		public i::frame::MouseableAdapter
	{
	public:

		// ���� : 
		CLASS_DECL_CONSTRUCTOR(ModelChangeButtonEvent)(
			_In_ compo::ModelViewerCompo * _mvcompo,
			_In_ unsigned int _index);

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, onClick_Up)(
			_In_ i::frame::MOUSE_BUTTON _button,
			_In_ int _x,
			_In_ int _y);

	private:

		// ���� : 
		compo::ModelViewerCompo * const my_MVCompo;
		
		// ���� : 
		const unsigned int my_ModelNumber;

		// ���� : 
		unsigned int my_AnimNumber;

	};

}