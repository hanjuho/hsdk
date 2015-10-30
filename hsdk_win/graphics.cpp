#include <hsdk/win/frame/graphics.h>
#include <hsdk/win/direct3d/d3d10_factory.h>



using namespace hsdk::frame;


//--------------------------------------------------------------------------------------
// Grobal D3D10 Variable
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_Background)(
	_In_ const float(&_color)[4])
{
	bgColor = _color;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_image)(
	_In_ const wchar_t * _filename)
{
	IF_INVALID(_filename)
	{
		texture = nullptr;
		imageW = 0.0f;
		imageH = 0.0f;
		return;
	}

	const D3DX10_IMAGE_INFO * info;
	IF_SUCCEEDED(direct3d::g_D3D10_Factory.get_Texture(&texture, _filename, &info))
	{
		imageW = info->Width;
		imageH = info->Height;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_imageDetail)(
	_In_ const float(&_rectangle)[4])
{
	if (texture)
	{
		D3DXMATRIX t;
		D3DXMatrixTranslation(&t,
			_rectangle[0] / imageW,
			_rectangle[1] / imageH, 0.0f);

		D3DXMATRIX s;
		D3DXMatrixScaling(&s,
			_rectangle[2] / imageW,
			_rectangle[3] / imageH, 0.0f);

		mTexcoord = t * s;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_Font)(
	_In_ const wchar_t * _fontname,
	_In_ unsigned int _fontsize)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_FontColor)(
	_In_ const float(&_color)[4])
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_Text)(
	_In_ const wchar_t * _text)
{

}