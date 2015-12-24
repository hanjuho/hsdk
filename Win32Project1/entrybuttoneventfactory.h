#pragma once



#include "common.h"
#include "modelviewercompo.h"



namespace entryloop
{

	// ���� : 
	DECL_CLASS(EntryButtonEventFactory)
	{
	public:
		
		// ���� : Container�� ������Ҵ� ���ο��� ������.
		CLASS_DECL_FUNC_T(i::frame::MouseableAdapter *, create_GoToGameButtonEvent)(
			_X_ void);

		// ���� : Container�� ������Ҵ� ���ο��� ������.
		CLASS_DECL_FUNC_T(i::frame::MouseableAdapter *, create_ModelChangeButtonEvent)(
			_In_ compo::ModelViewerCompo * _mvcompo,
			_In_ unsigned int _number);

		// ���� : Container�� ������Ҵ� ���ο��� ������.
		CLASS_DECL_FUNC_T(i::frame::MouseableAdapter *, create_ViewerPadChangeButtonEvent)(
			_In_ frame::MultiContainer<2> * _centerpad,
			_In_ frame::MultiContainer<2> * _southpad,
			_In_ unsigned int _number);


	};

}