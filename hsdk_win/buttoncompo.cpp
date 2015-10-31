#include <hsdk/win/frame/buttoncompo.h>
#include <hsdk/win/direct3d/d3d10_renderer.h>



using namespace hsdk::frame;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(ButtonCompo, ButtonCompo)(
	_In_ PARENT_RELATION _relation)
	: Component(_relation)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ButtonCompo, void, onMouse_Enter)(
	_In_ int _x,
	_In_ int _y)
{
	m_light = true;
	return Component::onMouse_Enter(_x, _y);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ButtonCompo, void, onMouse_Exit)(
	_In_ int _x,
	_In_ int _y)
{
	m_light = false;
	m_alpha = false;
	return Component::onMouse_Exit(_x, _y);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ButtonCompo, void, onClick_Down)(
	_In_ hsdk::i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	if (_button == i::frame::LBUTTON)
	{
		m_alpha = true;
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
		m_alpha = false;
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

		unsigned int ps_flag = 0;

		if (m_Graphics.texture)
		{
			ps_flag = direct3d::PS_TEXTURE_0 | direct3d::PS_TEXMATRIX_0;
			if (m_light)
			{
				D3DXVECTOR4 diffuse = { 1.2f, 1.2f, 1.2f, 1.0f };
				
				if (m_alpha)
				{
					diffuse.w = 0.8f;
				}

				direct3d::g_D3D10_Renderer.set_ColorDiffuse(diffuse);

				ps_flag = ADD_FLAG(ps_flag, direct3d::PS_MARERIAL_0);
			}

			direct3d::g_D3D10_Renderer.set_ScalarVSFlag(0);
			direct3d::g_D3D10_Renderer.set_ScalarPSFlag(ps_flag);
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