#include <hsdk/win/frame/buttoncompo.h>
#include <hsdk/win/direct3d/d3d10_renderer.h>



using namespace hsdk::frame;


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
	_In_ hsdk::i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	if (_button == i::frame::LBUTTON)
	{
		m_GraphicsRenderState = 0.5f;
	}

	return Component::onClick_Down(_button, _x, _y);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ButtonCompo, void, onClick_Up)(
	_In_ hsdk::i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	if (_button == i::frame::LBUTTON)
	{
		m_GraphicsRenderState = 1.0f;
	}

	return Component::onClick_Up(_button, _x, _y);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ButtonCompo, void, render)(
	_X_ void)
{
	if (is_Visible())
	{
		direct3d::g_D3D10_Renderer.set_MatrixWorldViewProj(&m_Position);
		direct3d::g_D3D10_Renderer.set_ScalarPSTime(m_GraphicsRenderState);
		if (m_Graphics.texture)
		{
			direct3d::g_D3D10_Renderer.set_ScalarVSFlag(0);
			direct3d::g_D3D10_Renderer.set_ScalarPSFlag(direct3d::PS_TEXTURE_0 | direct3d::PS_CALLFUNCTION_0 | direct3d::PS_TEXMATRIX_0);
			direct3d::g_D3D10_Renderer.render_UITexture(
				m_Graphics.texture,
				&m_Graphics.mTexcoord);
		}
		else
		{
			direct3d::g_D3D10_Renderer.set_ScalarVSFlag(0);
			direct3d::g_D3D10_Renderer.set_ScalarPSFlag(direct3d::PS_MARERIAL_0 | direct3d::PS_CALLFUNCTION_0);
			direct3d::g_D3D10_Renderer.render_UIRectangle(
				&m_Graphics.bgColor);
		}
	}
}