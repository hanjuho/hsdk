#pragma comment (lib, "hsdk_win.lib")
#pragma comment (lib, "FW1FontWrapper.lib")



#include <hsdk/win/frame/d3d11frame.h>
#include <FW1FontWrapper.h>



using namespace hsdk;
using namespace win;



class ComponentController : public i::frame::i_Mouseable
{
public:

	ComponentController(i::frame::i_Component * _component)
		:my_Component(_component)
	{

	}

	~ComponentController()
	{

	}

	// 설명 : mouse의 버튼이 눌렸을 때 발생하는 event
	INTERFACE_DECL_FUNC_T(void, onClick_Down)(
		/* [in] */ MOUSE_BUTTON _button,
		/* [in] */ int _x,
		/* [in] */ int _y)
	{

	}

	// 설명 : mouse의 버튼이 눌렸다가 때면 발생하는 event
	INTERFACE_DECL_FUNC_T(void, onClick_Up)(
		/* [in] */ MOUSE_BUTTON _button,
		/* [in] */ int _x,
		/* [in] */ int _y)
	{

	}

	// 설명 : mouse의 버튼을 누른 채 커서를 이동하면 발생하는 event
	INTERFACE_DECL_FUNC_T(void, onDrag)(
		/* [in] */ MOUSE_BUTTON _button,
		/* [in] */ int _x,
		/* [in] */ int _y)
	{
		my_Component->set_X(my_Component->get_X() - _x);
		my_Component->set_Y(my_Component->get_Y() - _y);
		my_Component->update();
	}

	// 설명 : mouse의 커서를 이동시키면 발생하는 event
	INTERFACE_DECL_FUNC_T(void, onMove)(
		/* [in] */ int _x,
		/* [in] */ int _y)
	{

	}

	// 설명 : mouse의 wheel을 조작하면 발생하는 event
	INTERFACE_DECL_FUNC_T(void, onWheel)(
		/* [in] */ int _x,
		/* [in] */ int _y,
		/* [in] */ int _w)
	{

	}

private:

	i::frame::i_Component * my_Component;

};

// grobal function
void drawText(ID3D11Device *pDevice, ID3D11DeviceContext *pContext);

// grobal variable
AutoRelease<IFW1Factory> pFW1Factory;
AutoRelease<IFW1FontWrapper> pFontWrapper;

// main
int CALLBACK wWinMain(HINSTANCE _hInstance, HINSTANCE, LPWSTR, int)
{
	frame::D3D11Frame frame(
		_hInstance,
		TEXT("D3D11Frame"),
		0,
		0,
		800,
		600);

	i::frame::i_Graphics * g;
	g = frame.graphics();
	if (g)
	{
		g->set_image(TEXT("4764905167076218937.png"));
		g->set_imageDetail(860.0f, 1201.0f, { 344.0f, 280.0f, 160.0f, 160.0f });
	}

	i::frame::i_Component * component_1 = new frame::Container();
	component_1->set_X(50.0f);
	component_1->set_Y(50.0f);
	component_1->set_W(150.0f);
	component_1->set_H(150.0f);
	component_1->set_Mouseable(new ComponentController(component_1));
	g = component_1->graphics();
	if (g)
	{
		g->set_image(TEXT("4764905167076218937.png"));
		g->set_imageDetail(860.0f, 1201.0f, { 344.0f, 280.0f, 160.0f, 160.0f });
	}

	i::frame::i_Component * component_2 = new frame::Component();
	component_2->set_X(25.0f);
	component_2->set_Y(25.0f);
	component_2->set_W(100.0f);
	component_2->set_H(100.0f);
	component_2->set_Mouseable(new ComponentController(component_2));
	g = component_2->graphics();
	if (g)
	{
		g->set_image(TEXT("4764905167076218937.png"));
		g->set_imageDetail(860.0f, 1201.0f, { 344.0f, 280.0f, 160.0f, 160.0f });
	}

	component_1->add_Component(component_2);
	frame.add_Component(component_1);
	frame.update();

	component_1->set_X(200.0f);
	component_1->set_Y(200.0f);
	component_1->update();
	
	HRESULT hResult = FW1CreateFactory(FW1_VERSION, &pFW1Factory);
	hResult = pFW1Factory->CreateFontWrapper(frame::D3D11::DEVICE, L"Arial", &pFontWrapper);

	while (frame.is_Valid())
	{
		frame.get_Message();

		frame::D3D11::clear_Backbuffer();

		frame.render();
		//drawText(frame::D3D11::DEVICE, frame::D3D11::CONTEXT);

		frame::D3D11::swap_Backbuffer();
	}
}

void drawText(ID3D11Device *pDevice, ID3D11DeviceContext *pContext)
{
	pFontWrapper->DrawString(
		pContext,
		L"Text",// String
		128.0f,// Font size
		100.0f,// X position
		50.0f,// Y position
		0xff0099ff,// Text color, 0xAaBbGgRr
		0// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
		);
}