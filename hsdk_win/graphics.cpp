#include <hsdk/win/frame/graphics.h>
#include <hsdk/win/shader/vs_ui.h>
#include <hsdk/win/shader/ps_ui.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;
using namespace direct3d;


//--------------------------------------------------------------------------------------
// Grobal declare function
//--------------------------------------------------------------------------------------

// 설명 : 
DECL_FUNC_T(D3DCOLORVALUE, get_ColorToValue)(
	/* [r] */ D3DCOLOR _color);

//--------------------------------------------------------------------------------------
// Grobal D3D10 Variable
//--------------------------------------------------------------------------------------

// 설명 : 
direct3d::D3D10_Master * g_Graphics_refMaster;

//--------------------------------------------------------------------------------------
AutoRelease<ID3D10RasterizerState> CULL_BACK_RASTERIZER;
AutoRelease<ID3D10DepthStencilState> FRONT_CLIPPING_DEPTH;

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Graphics, initialize)(
	/* [r] */ direct3d::D3D10_Master * _master)
{
	g_Graphics_refMaster = _master;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, destroy)(
	/* [x] */ void)
{
	g_Graphics_refMaster = nullptr;
}

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
	/* [r] */ const float(&_color)[4])
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_image)(
	/* [r] */ const wchar_t * _filename)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, set_imageDetail)(
	/* [r] */ const float(&_rectangle)[4])
{

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
	m_Text = _text;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, update)(
	/* [r] */ const float(&_rectangle)[4])
{
	// Convert the rect from screen coordinates to clip space coordinates.
	/*float Left, Right, Top, Bottom;

	Left = ((_rectangle[0] / g_Graphics_refMaster->get_Width()) * 2.0f) - 1.0f;
	Right = ((_rectangle[2] / g_Graphics_refMaster->get_Width()) * 2.0f) - 1.0f;
	Top = 1.0f - ((_rectangle[1] / g_Graphics_refMaster->get_Height()) * 2.0f);
	Bottom = 1.0f - ((_rectangle[3] / g_Graphics_refMaster->get_Height()) * 2.0f);

	float uv[4] = {
	m_uvRectangle[0] / m_imageW,
	m_uvRectangle[1] / m_imageH,
	(m_uvRectangle[0] + m_uvRectangle[2]) / m_imageW,
	(m_uvRectangle[1] + m_uvRectangle[3]) / m_imageH };

	D3D10_UIFormat vertices[4] = {
	{ { Left, Top, 0.5f }, get_ColorToValue(0), { uv[0], uv[1] } },
	{ { Right, Top, 0.5f }, get_ColorToValue(0), { uv[2], uv[1] } },
	{ { Left, Bottom, 0.5f }, get_ColorToValue(0), { uv[0], uv[3] } },
	{ { Right, Bottom, 0.5f }, get_ColorToValue(0), { uv[2], uv[3] } } };

	D3D10_UIFormat * pVB;
	if (SUCCEEDED(my_Panel->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&pVB)))
	{
	CopyMemory(pVB, vertices, sizeof(vertices));
	my_Panel->Unmap();
	}*/
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Graphics, void, render)(
	/* [x] */ void)
{
	//ID3D10Device * const device =
	//	g_Graphics_refMaster->get_D3D10_Device();

	//// Set the quad VB as current
	//UINT stride = sizeof(D3D10_UIFormat);
	//UINT offset = 0;

	//device->IASetVertexBuffers(0, 1, &my_Panel, &stride, &offset);
	//device->IASetInputLayout(g_Graphics_inputLayout);
	//device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//// Draw dialog background
	//D3D10_TECHNIQUE_DESC techDesc;
	//g_Graphics_RenderUIUntex_Technique->GetDesc(&techDesc);
	//for (unsigned int index = 0; index < techDesc.Passes; ++index)
	//{
	//	g_Graphics_RenderUI_Technique->GetPassByIndex(index)->Apply(0);
	//	device->Draw(4, 0);
	//}
}

//--------------------------------------------------------------------------------------
// Grobal implements function
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(D3DCOLORVALUE, get_ColorToValue)(
	/* [r] */ D3DCOLOR _color)
{
	D3DCOLORVALUE value = {
		((_color >> 16) & 0xFF) / 255.0f,
		((_color >> 8) & 0xFF) / 255.0f,
		(_color & 0xFF) / 255.0f,
		((_color >> 24) & 0xFF) / 255.0f };

	return value;
}
