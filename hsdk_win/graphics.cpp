#include <hsdk/win/frame/graphics.h>
#include <hsdk/win/framework.h>
#include <hsdk/win/direct3d/d3d10_font.h>
#include <hsdk/win/direct3d/d3d10_factory.h>
#include <hsdk/win/direct3d/d3d10_renderer.h>



using namespace hsdk::frame;
using namespace hsdk::direct3d;


//--------------------------------------------------------------------------------------
// Grobal D3D10 Variable
//--------------------------------------------------------------------------------------

D3D10_Font font;

//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Graphics, Graphics)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(Graphics, Graphics)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_Background)(
	_In_ const float(&_color)[4])
{
	my_BGColor = _color;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_image)(
	_In_ const wchar_t * _filename)
{
	IF_INVALID(_filename)
	{
		my_Texture_info = nullptr;
		my_Texture = nullptr;
		return;
	}

	g_D3D10_Factory.get_Texture(&my_Texture, _filename, &my_Texture_info);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_imageDetail)(
	_In_ const float(&_rectangle)[4])
{
	if (my_Texture_info)
	{
		D3DXMATRIX t;
		D3DXMatrixTranslation(&t,
			_rectangle[0] / my_Texture_info->Width,
			_rectangle[1] / my_Texture_info->Height, 0.0f);

		D3DXMATRIX s;
		D3DXMatrixScaling(&s,
			_rectangle[2] / my_Texture_info->Width,
			_rectangle[3] / my_Texture_info->Height, 0.0f);

		my_Texcoord = t * s;
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
	if (font.font() == nullptr)
	{
		font.initialize("font/fontdata.txt", L"font/font.dds");
	}

	char atow[256];
	wcstombs_s<256>(nullptr, atow, _text, sizeof(atow));
	font.build_Text(my_Context, atow);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, update)(
	_In_ const float(&_rectangle)[4])
{
	float screenWidth = (float)framework::g_Framework_Window.width;
	float screenHeigth = (float)framework::g_Framework_Window.height;
	float myWidth = _rectangle[2] / screenWidth;
	float myHeight = _rectangle[3] / screenHeigth;

	D3DXMATRIX t;
	D3DXMatrixTranslation(&t,
		(_rectangle[0] / screenWidth * 2.0f) + (myWidth - 1.0f),
		(1.0f - myHeight) - (_rectangle[1] / screenHeigth * 2.0f), 0.0f);

	D3DXMATRIX s;
	D3DXMatrixScaling(&s,
		myWidth, myHeight, 0.0f);

	my_Position = s * t;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, render)(
	_X_ float _persent)
{
	if (my_Context.textSlot)
	{

	}

	g_D3D10_Renderer.set_MatrixWorldViewProj(&my_Position);
	g_D3D10_Renderer.set_ScalarPSTime(_persent);
	if (my_Texture)
	{
		g_D3D10_Renderer.set_ScalarVSFlag(VS_TEXMATRIX_0);
		g_D3D10_Renderer.set_ScalarPSFlag(PS_TEXTURE_0 | PS_CALLFUNCTION_0);
		g_D3D10_Renderer.render_UITexture(
			my_Texture,
			&my_Texcoord);
	}
	else
	{
		g_D3D10_Renderer.set_ScalarVSFlag(0);
		g_D3D10_Renderer.set_ScalarPSFlag(PS_MARERIAL_0 | PS_CALLFUNCTION_0);
		g_D3D10_Renderer.render_UIRectangle(
			&my_BGColor);
	}
}