#pragma once



#include "common.h"



namespace compoevent
{

	// 설명 : 
	DECL_STRUCT(GameStart)
	{

		// 설명 : 
		unsigned int avatar;

		// 설명 : 
		unsigned int skill[4];

	};

	// 설명 : 
	DECL_CLASS(GoToGameButtonEvent) :
		public i::frame::MouseableAdapter
	{
	public:

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, onClick_Up)(
			_In_ i::frame::MOUSE_BUTTON _button,
			_In_ int _x,
			_In_ int _y);

	public:

		// 설명 : 
		AutoDelete<GameStart> gameStart;

	};

}