#pragma once



#include "common.h"
#include "modelviewercompo.h"



namespace entry
{

	// 설명 : 
	DECL_CLASS(EntryCompoFactory)
	{
	public:

		// 설명 : Container의 구성요소는 내부에서 결정됨.
		CLASS_DECL_FUNC_T(frame::MultiContainer<2> *, create_CenterPad)(
			_X_ void);

		// 설명 : Container의 구성요소는 내부에서 결정됨.
		CLASS_DECL_FUNC_T(frame::MultiContainer<2> *, create_SouthPad)(
			_X_ void);

		// 설명 : Container의 구성요소는 내부에서 결정됨.
		CLASS_DECL_FUNC_T(frame::Container *, create_EasePad)(
			_X_ void);

		// 설명 :
		CLASS_DECL_FUNC_T(compo::ModelViewerCompo *, create_ModelViewerCompo)(
			_X_ void);

	private:

		// 설정 : 


	};

}