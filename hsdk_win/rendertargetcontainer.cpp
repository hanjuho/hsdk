#include <hsdk/win/frame/rendertargetcontainer.h>
#include <hsdk/win/direct3d/d3d10_renderer.h>



using namespace hsdk::frame;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(RenderTargetContainer, RenderTargetContainer)(
	_In_ float _w,
	_In_ float _h,
	_In_ float _x,
	_In_ float _y)
	: Container(_x, _y, _w, _h)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(RenderTargetContainer, RenderTargetContainer)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RenderTargetContainer, void, update)(
	_X_ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RenderTargetContainer, void, reform)(
	_X_ void)
{
	Container::reform();

	my_RenderTarget.initialize(
		(unsigned int)get_W(), 
		(unsigned int)get_H());
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RenderTargetContainer, void, render)(
	_X_ void)
{
	IF_SUCCEEDED(my_RenderTarget.begin(m_Graphics.bgColor))
	{
		Container::render();

		my_RenderTarget.end();
	}

	direct3d::g_D3D10_Renderer.set_MatrixWorldView(&m_Graphics.mPosition);
	direct3d::g_D3D10_Renderer.set_ScalarVSFlag(0);
	direct3d::g_D3D10_Renderer.set_ScalarPSFlag(direct3d::PS_TEXTURE_0);
	direct3d::g_D3D10_Renderer.render_UITexture(
		my_RenderTarget.get_View(), &direct3d::g_D3D10_identityMatrix);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RenderTargetContainer, void, clear)(
	_X_ void)
{
	Container::clear();
	my_RenderTarget = direct3d::D3D10_RenderTarget();
}