#pragma once



#include "common.h"



namespace entry
{

	// 설명 : 
	DECL_CLASS(EntryCompo)
		: public frame::Container
	{
	public:

		// 생성자
		CLASS_DECL_CONSTRUCTOR(EntryCompo)(void);

		// 설명 :
		INTERFACE_DECL_FUNC_T(void, set_Layout)(
			_In_ i::frame::i_Layout * _layout);


	private:



	};

}