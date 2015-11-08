#pragma once



#include "common.h"



namespace compoevent
{

	// ���� : 
	DECL_STRUCT(GameStart)
	{

		// ���� : 
		unsigned int avatar;

		// ���� : 
		unsigned int skill[4];

	};

	// ���� : 
	DECL_CLASS(GoToGameButtonEvent) :
		public i::frame::MouseableAdapter
	{
	public:

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, onClick_Up)(
			_In_ i::frame::MOUSE_BUTTON _button,
			_In_ int _x,
			_In_ int _y);

	public:

		// ���� : 
		AutoDelete<GameStart> gameStart;

	};

}