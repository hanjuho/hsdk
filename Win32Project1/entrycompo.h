#pragma once



#include "common.h"



namespace entry
{

	// ���� : 
	DECL_CLASS(EntryCompo)
		: public frame::Container
	{
	public:

		// ������
		CLASS_DECL_CONSTRUCTOR(EntryCompo)(void);

		// ���� :
		INTERFACE_DECL_FUNC_T(void, set_Layout)(
			_In_ i::frame::i_Layout * _layout);


	private:



	};

}