#pragma comment (lib, "hsdk_win.lib")



#include <hsdk/win/frame/d3d11frame.h>


using namespace hsdk;
using namespace win;

class A
{
public:
	int qewr;
};

class B
{
public:
	int asdf;
};

class C
	:public B, public A
{
public:
	int zxcv;
};


int CALLBACK wWinMain(HINSTANCE _hInstance, HINSTANCE, LPWSTR, int)
{
	frame::D3D11Frame frame(
		_hInstance,
		TEXT("D3D11Frame"),
		0, 
		0, 
		800, 
		600);
	
	while (frame.is_Valid())
	{
		frame.get_Message();

		frame::D3D11::clear_Backbuffer();

		frame.render();

		frame::D3D11::swap_Backbuffer();
	}
}