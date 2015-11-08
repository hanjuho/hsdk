#pragma once



#include "common.h"
#include "modelviewercompo.h"



namespace entry
{

	// 설명 : 
	DECL_CLASS(EntryCompoFactory)
	{
	public:

		// 설명 : 
		CLASS_DECL_FUNC_T(void, build_EntryGUI)(
			_Out_ frame::Container * _container,
			_In_ float _width,
			_In_ float _height);

		// 설명 : Container의 구성요소는 내부에서 결정됨.
		CLASS_DECL_FUNC_T(void, build_CenterPad)(
			_Out_ frame::MultiContainer<2> * _container,
			_In_ compo::ModelViewerCompo * _mvcompo);

		// 설명 : Container의 구성요소는 내부에서 결정됨.
		CLASS_DECL_FUNC_T(void, build_SouthPad)(
			_Out_ frame::MultiContainer<2> * _container,
			_In_ compo::ModelViewerCompo * _mvcompo);

		// 설명 : Container의 구성요소는 내부에서 결정됨.
		CLASS_DECL_FUNC_T(void, build_EastPad)(
			_Out_ frame::Container * _container,
			_In_ frame::MultiContainer<2> * _centerpad,
			_In_ frame::MultiContainer<2> * _southpad);

		// 설명 : Container의 구성요소는 내부에서 결정됨.
		CLASS_DECL_FUNC_T(compo::ModelViewerCompo *, create_ModelViewerCompo)(
			_X_ void);

		// 설명 : 
		CLASS_DECL_FUNC_T(void, autoset_MainBorderLayout)(
			_Out_ i::frame::i_Layout * _layout);

		// 설명 : 
		CLASS_DECL_FUNC_T(void, autoset_CenterBorderLayout)(
			_Out_ i::frame::i_Layout * _layout);

		// 설명 : 
		CLASS_DECL_FUNC_T(void, autoset_CenterGridLayout)(
			_Out_ i::frame::i_Layout * _layout);
		
		// 설명 : 
		CLASS_DECL_FUNC_T(void, autoset_SouthGridLayout0)(
			_Out_ i::frame::i_Layout * _layout);

		// 설명 : 
		CLASS_DECL_FUNC_T(void, autoset_SouthGridLayout1)(
			_Out_ i::frame::i_Layout * _layout);

		// 설명 : 
		CLASS_DECL_FUNC_T(void, autoset_EastGridLayout)(
			_Out_ i::frame::i_Layout * _layout);

	private:

		// 설정 : 


	};

}