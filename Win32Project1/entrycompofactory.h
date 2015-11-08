#pragma once



#include "common.h"
#include "modelviewercompo.h"



namespace entry
{

	// ���� : 
	DECL_CLASS(EntryCompoFactory)
	{
	public:

		// ���� : Container�� ������Ҵ� ���ο��� ������.
		CLASS_DECL_FUNC_T(frame::MultiContainer<2> *, create_CenterPad)(
			_X_ void);

		// ���� : Container�� ������Ҵ� ���ο��� ������.
		CLASS_DECL_FUNC_T(frame::MultiContainer<2> *, create_SouthPad)(
			_X_ void);

		// ���� : Container�� ������Ҵ� ���ο��� ������.
		CLASS_DECL_FUNC_T(frame::Container *, create_EasePad)(
			_X_ void);

		// ���� :
		CLASS_DECL_FUNC_T(compo::ModelViewerCompo *, create_ModelViewerCompo)(
			_X_ void);

	private:

		// ���� : 


	};

}