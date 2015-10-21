#include <hsdk/win/frame/frame.h>
#include <hsdk/win/frame/graphics.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;


//--------------------------------------------------------------------------------------
// Grobal 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Frame, Frame)(void)
: m_inputEventHelper(this)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(Frame, Frame)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Frame, void, event_Proc)(
	/* [r] */unsigned int _uEvent,
	/* [r] */unsigned int _wParam,
	/* [r] */unsigned long _lParam)
{
	switch (_uEvent)
	{
	case WM_SIZE:
		set_W((float)(LOWORD(_lParam)));
		set_H((float)(HIWORD(_lParam)));
		return;

	case WM_KEYDOWN:
		return;
	case WM_KEYUP:
		return;
	case WM_MOUSEMOVE:
		m_inputEventHelper.xy = {
			LOWORD(_lParam),
			HIWORD(_lParam) };
		return;
	}

}