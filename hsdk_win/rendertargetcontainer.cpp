#include <hsdk/win/frame/rendertargetcontainer.h>
#include <hsdk/win/framework.h>



using namespace hsdk::frame;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(RenderTargetContainer, RenderTargetContainer)(
	_In_ float _w,
	_In_ float _h,
	_In_ float _x,
	_In_ float _y,
	_In_ hsdk::i::frame::FRAME_FORM _form)
	: Container(_x, _y, _w, _h, _form)
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
	IF_INVALID(my_RenderTarget)
	{
		float w = get_W();
		float h = get_H();

		if (w != 0 && h != 0)
		{
			D3D10_TEXTURE2D_DESC desc;
			desc.Width = w;
			desc.Height = h;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D10_USAGE_DEFAULT;
			desc.BindFlags = D3D10_BIND_RENDER_TARGET;
			desc.CPUAccessFlags = (D3D10_CPU_ACCESS_FLAG)(0);
			desc.MiscFlags = 0;

			AutoRelease<ID3D10Texture2D> rendertarget;
			IF_SUCCEEDED(framework::g_Framework_Device.d3d10Device->CreateTexture2D(
				&desc,
				nullptr,
				&rendertarget))
			{
				my_RenderTarget = rendertarget;

			}
		}
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RenderTargetContainer, void, render)(
	_X_ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(RenderTargetContainer, void, clear)(
	_X_ void)
{

}