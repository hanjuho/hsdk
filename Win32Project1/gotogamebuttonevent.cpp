#include "gotogamebuttonevent.h"
#include "game.h"



using namespace compoevent;


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(GoToGameButtonEvent, void, onClick_Up)(
	_In_ i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	game::initialize_Game(gameStart);
}