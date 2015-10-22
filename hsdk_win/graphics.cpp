#include <hsdk/win/frame/graphics.h>
#include <hsdk/win/frame/direct3d/d3d10_meshrenderer.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;
using namespace direct3d;

//--------------------------------------------------------------------------------------
// Grobal D3D10 Variable
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Graphics, Graphics)(void)
{
	D3DXMatrixIdentity(&my_Position);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(Graphics, Graphics)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_Background)(
	/* [r] */ const float(&_color)[4])
{
	my_BGColor = _color;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_image)(
	/* [r] */ const wchar_t * _filename)
{
	IF_INVALID(_filename)
	{
		my_Texture_info = nullptr;
		my_Texture = nullptr;
		return;
	}

	g_D3D10_Master.get_Texture(&my_Texture, _filename, &my_Texture_info);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_imageDetail)(
	/* [r] */ const float(&_rectangle)[4])
{
	if (my_Texture_info)
	{
		my_Sprite[0] = _rectangle[0] / my_Texture_info->Width;
		my_Sprite[1] = _rectangle[1] / my_Texture_info->Height;
		my_Sprite[2] = (_rectangle[0] + _rectangle[2]) / my_Texture_info->Width;
		my_Sprite[3] = (_rectangle[1] + _rectangle[3]) / my_Texture_info->Height;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_Font)(
	/* [r] */ const wchar_t * _fontname,
	/* [r] */ unsigned int _fontsize)
{
	
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_FontColor)(
	/* [r] */ const float(&_color)[4])
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_Text)(
	/* [r] */ const wchar_t * _text)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, update)(
	/* [r] */ const float(&_rectangle)[4])
{
	float screenWidth = (float)g_Direct3D_Window.width;
	float screenHeigth = (float)g_Direct3D_Window.height;
	float myWidth = _rectangle[2] / screenWidth;
	float myHeight = _rectangle[3] / screenHeigth;
	
	D3DXMATRIX scale;
	D3DXMatrixScaling(
		&scale,
		myWidth,
		myHeight,
		0.0f);
	
	D3DXMatrixTranslation(
		&my_Position,
		(_rectangle[0] / screenWidth * 2.0f) + (myWidth - 1.0f),
		(1.0f - myHeight) - (_rectangle[1] / screenHeigth * 2.0f),
		0.0f);
	
	D3DXMatrixMultiply(&my_Position, &scale, &my_Position);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, render)(
	/* [x] */ float _persent)
{
	if (my_Texture)
	{
		g_D3D10_MeshRenderer.render_UITexture(
			my_Position,
			my_Texture,
			my_Sprite,
			_persent);
	}
	else
	{
		g_D3D10_MeshRenderer.render_UIRectangle(
			my_Position,
			my_BGColor,
			_persent);
	}
}