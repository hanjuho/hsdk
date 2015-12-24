#pragma once



#include "common.h"
#include "modelviewercompo.h"



namespace entryloop
{

	// 설명 : 
	DECL_CLASS(EntryButtonEventFactory)
	{
	public:
		
		// 설명 : Container의 구성요소는 내부에서 결정됨.
		CLASS_DECL_FUNC_T(i::frame::MouseableAdapter *, create_GoToGameButtonEvent)(
			_X_ void);

		// 설명 : Container의 구성요소는 내부에서 결정됨.
		CLASS_DECL_FUNC_T(i::frame::MouseableAdapter *, create_ModelChangeButtonEvent)(
			_In_ compo::ModelViewerCompo * _mvcompo,
			_In_ unsigned int _number);

		// 설명 : Container의 구성요소는 내부에서 결정됨.
		CLASS_DECL_FUNC_T(i::frame::MouseableAdapter *, create_ViewerPadChangeButtonEvent)(
			_In_ frame::MultiContainer<2> * _centerpad,
			_In_ frame::MultiContainer<2> * _southpad,
			_In_ unsigned int _number);


	};

}