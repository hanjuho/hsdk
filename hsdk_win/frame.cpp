#include <hsdk/win/frame/frame.h>
#include <hsdk/win/frame/graphics.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;


//--------------------------------------------------------------------------------------
// Grobal 
//--------------------------------------------------------------------------------------
direct3d::Direct3D g_D3D_0;

//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Frame, Frame)(void)
: m_inputEventHelper(this)
{
	
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(Frame, Frame)(void)
{
	// g_D3D_0.is_KeyDown();
	// g_D3D_0.is_MouseButtonDown();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Frame, void, message_Proc)(
	/* [r] */unsigned int _uMsg,
	/* [r] */unsigned int _wParam,
	/* [r] */unsigned long _lParam)
{

}