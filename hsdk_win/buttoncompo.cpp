#include <hsdk/win/frame/buttoncompo.h>



using namespace hsdk;
using namespace win::frame;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(ButtonCompo, ButtonCompo)(
	_In_ float _x,
	_In_ float _y,
	_In_ float _w,
	_In_ float _h)
	: Component(_x, _y, _w, _h)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ButtonCompo, void, onClick_Down)(
	_In_ i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	if (_button == i::frame::LBUTTON)
	{
		m_GraphicsRenderState = 0.5f;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ButtonCompo, void, onClick_Up)(
	_In_ i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	if (_button == i::frame::LBUTTON)
	{
		m_GraphicsRenderState = 1.0f;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ButtonCompo, void, render)(
	/* [x] */ void)
{
	if (is_Visible())
	{
		m_D3D10Graphics.render(m_GraphicsRenderState);
	}
}