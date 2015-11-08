#pragma once



#include "common.h"
#include "modelviewercompo.h"



namespace entry
{

	// ���� : 
	DECL_CLASS(EntryCompoFactory)
	{
	public:

		// ���� : 
		CLASS_DECL_FUNC_T(void, build_EntryGUI)(
			_Out_ frame::Container * _container,
			_In_ float _width,
			_In_ float _height);

		// ���� : Container�� ������Ҵ� ���ο��� ������.
		CLASS_DECL_FUNC_T(void, build_CenterPad)(
			_Out_ frame::MultiContainer<2> * _container,
			_In_ compo::ModelViewerCompo * _mvcompo);

		// ���� : Container�� ������Ҵ� ���ο��� ������.
		CLASS_DECL_FUNC_T(void, build_SouthPad)(
			_Out_ frame::MultiContainer<2> * _container,
			_In_ compo::ModelViewerCompo * _mvcompo);

		// ���� : Container�� ������Ҵ� ���ο��� ������.
		CLASS_DECL_FUNC_T(void, build_EastPad)(
			_Out_ frame::Container * _container,
			_In_ frame::MultiContainer<2> * _centerpad,
			_In_ frame::MultiContainer<2> * _southpad);

		// ���� : Container�� ������Ҵ� ���ο��� ������.
		CLASS_DECL_FUNC_T(compo::ModelViewerCompo *, create_ModelViewerCompo)(
			_X_ void);

		// ���� : 
		CLASS_DECL_FUNC_T(void, autoset_MainBorderLayout)(
			_Out_ i::frame::i_Layout * _layout);

		// ���� : 
		CLASS_DECL_FUNC_T(void, autoset_CenterBorderLayout)(
			_Out_ i::frame::i_Layout * _layout);

		// ���� : 
		CLASS_DECL_FUNC_T(void, autoset_CenterGridLayout)(
			_Out_ i::frame::i_Layout * _layout);
		
		// ���� : 
		CLASS_DECL_FUNC_T(void, autoset_SouthGridLayout0)(
			_Out_ i::frame::i_Layout * _layout);

		// ���� : 
		CLASS_DECL_FUNC_T(void, autoset_SouthGridLayout1)(
			_Out_ i::frame::i_Layout * _layout);

		// ���� : 
		CLASS_DECL_FUNC_T(void, autoset_EastGridLayout)(
			_Out_ i::frame::i_Layout * _layout);

	private:

		// ���� : 


	};

}