#include <hsdk/win/frame/rendertargetcontainer.h>
#include <hsdk/win/direct3d/d3d10_renderer.h>
#include <hsdk/win/framework.h>



using namespace hsdk::frame;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(RenderTargetContainer, RenderTargetContainer)(
	_In_ PARENT_RELATION _relation)
	: Container(_relation)
{
	my_Vp.Width = 0;
	my_Vp.Height = 0;
	my_Vp.MinDepth = 0;
	my_Vp.MaxDepth = 1;
	my_Vp.TopLeftX = 0;
	my_Vp.TopLeftY = 0;

	m_Graphics.bgColor.w = 0.0f;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(RenderTargetContainer, RenderTargetContainer)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RenderTargetContainer, void, reform)(
	_X_ void)
{
	Container::reform();

	unsigned int w = (unsigned int)get_W();
	unsigned int h = (unsigned int)get_H();

	my_RenderTarget.initialize(w, h);

	my_Vp.Width = w;
	my_Vp.Height = h;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RenderTargetContainer, void, render)(
	_X_ void)
{
	if (is_Visible())
	{
		IF_SUCCEEDED(my_RenderTarget.begin(m_Graphics.bgColor))
		{
			// Setup the viewport to match the backbuffer
			framework::g_Framework_Device.d3d10Device->RSSetViewports(1, &my_Vp);

			direct3d::g_D3D10_Renderer.set_MatrixWorldViewProj(&direct3d::g_D3D10_identityMatrix);
			direct3d::g_D3D10_Renderer.set_ScalarPSTime(1.0f);
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

			// Setup the viewport to match the backbuffer
			framework::g_Framework_Device.d3d10Device->RSSetViewports(1,
				&framework::g_Framework_Device.d3d10ViewPort);

			auto begin = m_Container.begin();
			auto end = m_Container.end();
			while (begin != end)
			{
				(*begin)->render();
				++begin;
			}

			my_RenderTarget.end();
		}

		direct3d::g_D3D10_Renderer.set_MatrixWorldViewProj(&m_Position);
		direct3d::g_D3D10_Renderer.set_ScalarVSFlag(0);
		direct3d::g_D3D10_Renderer.set_ScalarPSFlag(direct3d::PS_TEXTURE_0);
		direct3d::g_D3D10_Renderer.render_UITexture(
			my_RenderTarget.get_View(), &direct3d::g_D3D10_identityMatrix);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RenderTargetContainer, void, clear)(
	_X_ void)
{
	Container::clear();

	my_RenderTarget = direct3d::D3D10_RenderTarget();
}