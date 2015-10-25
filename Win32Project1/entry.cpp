#include "entry.h"



//--------------------------------------------------------------------------------------
// Grobal Variable
//--------------------------------------------------------------------------------------

// 설명 : 
ButtonCompo g_Button(100.0f, 100.0f, 540.0f, 413.0f);
inputEventHelper g_Helper(&g_Button);

// 설명 : 
Framework_Camera g_Camera;

// 설명 : 
D3D10_Mesh g_Mesh;
D3D10_MeshAnimation g_MeshAnimation;
std::vector<D3DXMATRIX> g_boneMatrixBuffer;

D3D10_Mesh g_SkyBox;

Direct8_SoundPlayer g_Player[3];

D3DXMATRIX g_World;

int x = 0, y = 0;

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(LRESULT, entry::OnMsgProc)(
	_Out_opt_ BOOL * _bNoFurtherProcessing,
	_In_ HWND _hWnd,
	_In_ UINT _uMsg,
	_In_ WPARAM _wParam,
	_In_ LPARAM _lParam,
	_Inout_ void * _userContext)
{
	return 0;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, entry::OnMouse)(
	_In_ const short * _buttonsDown,
	_In_ unsigned int _buttonCount,
	_In_ int _mouseWheelDelta,
	_In_ int _xPos,
	_In_ int _yPos,
	_Inout_ void * _userContext)
{
	if (IS_FLAG(_buttonsDown[Direct3D_LEFTBUTTON], 0x01))
	{
		g_Helper.onClick_Down(i::frame::LBUTTON, _xPos, _yPos);

		int dx;
		if (dx = _xPos - x)
		{
			g_Camera.rotate_YAxis(D3DXToRadian(dx));
		}

		int dy;
		if (dy = _yPos - y)
		{
			g_Camera.rotate_XAxis(D3DXToRadian(dy));
		}
	}
	else if (IS_FLAG(_buttonsDown[Direct3D_LEFTBUTTON], 0x02))
	{
		g_Helper.onClick_Up(i::frame::LBUTTON, _xPos, _yPos);
	}

	x = _xPos;
	y = _yPos;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, OnKeyboard)(
	_In_ unsigned char _nKey,
	_In_ short _bKeyDown,
	_In_ short _bAltDown,
	_Inout_ void * _userContext)
{
	if (_nKey == VK_UP)
	{
		g_Camera.move_ZDir(1.0f);
	}

	if (_nKey == VK_DOWN)
	{
		g_Camera.move_ZDir(-1.0f);
	}

	if (_nKey == VK_LEFT)
	{
		g_Camera.move_XDir(-1.0f);
	}

	if (_nKey == VK_RIGHT)
	{
		g_Camera.move_XDir(1.0f);
	}
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, OnFrameMove)(
	_In_ double _fTime,
	_In_ float _fElapsedTime,
	_Inout_ void * _userContext)
{

}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(BOOL, ModifyDeviceSettings)(
	_In_ const D3D10_DEVICE_DESC & _deviceSettings,
	_Inout_ void * _userContext)
{
	return true;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(BOOL, IsD3D10DeviceAcceptable)(
	_In_ unsigned int _adapter,
	_In_ unsigned int _output,
	_In_ D3D10_DRIVER_TYPE _deviceType,
	_In_ DXGI_FORMAT _backBufferFormat,
	_In_ BOOL _windowed,
	_Inout_ void * _userContext)
{
	return true;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(OnD3D10CreateDevice)(
	_In_ ID3D10Device * _d3dDevice,
	_In_ const DXGI_SURFACE_DESC & _backBufferSurfaceDesc,
	_Inout_ void * _userContext)
{
	g_Camera.set_Position(D3DXVECTOR3(0.0f, 0.0f, -10.0f));
	g_Camera.compute_ViewMatrix(direct3d::g_D3D10_ViewMatrix);
	
	// gui
	g_Button.graphics()->set_Background({ 0.0f, 1.0f, 0.0f, 1.0f });
	g_Button.graphics()->set_image(L"background/seafloor.dds");
	g_Button.reform();
	g_Button.set_Visible(true);

	// g_Bone
	g_D3D10_Factory.create_MeshFromFile(g_Mesh, nullptr, L"data/DeathwingHuman.X", &g_MeshAnimation);

	g_boneMatrixBuffer.resize(g_MeshAnimation.get_NumOfBones());
	g_MeshAnimation.transbone(&g_boneMatrixBuffer[0], g_boneMatrixBuffer.size(), g_D3D10_ViewMatrix);

	// g_SkyBox
	g_D3D10_Factory.create_MeshSkyBox(g_SkyBox, 10.0f);

	AutoRelease<ID3D10ShaderResourceView> texture;
	g_D3D10_Factory.create_SkyBoxTexture(&texture, 512, 512,
		L"background/blood_sport512_front.jpg",
		L"background/blood_sport512_back.jpg",
		L"background/blood_sport512_left.jpg",
		L"background/blood_sport512_right.jpg",
		L"background/blood_sport512_top.jpg",
		L"");

	g_SkyBox.setup1_Texture(0, 0, texture);

	return S_OK;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, OnD3D10FrameRender)(
	_In_ ID3D10Device * _d3dDevice,
	_In_ double _fTime,
	_In_ float _fElapsedTime,
	_Inout_ void * _userContext)
{
	g_Camera.compute_ViewMatrix(direct3d::g_D3D10_ViewMatrix);

	// Clear the render target and depth stencil
	float ClearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	ID3D10RenderTargetView * pRTV = framework::g_Framework_Device.d3d10RTV;
	_d3dDevice->ClearRenderTargetView(pRTV, ClearColor);
	ID3D10DepthStencilView * pDSV = framework::g_Framework_Device.d3d10DSV;
	_d3dDevice->ClearDepthStencilView(pDSV, D3D10_CLEAR_DEPTH, 1.0, 0);
	
	D3DXMATRIX m = direct3d::g_D3D10_ViewMatrix * direct3d::g_D3D10_ProjMatrix;

	g_D3D10_MeshRenderer.render_SkyBox(
		m, g_SkyBox);

	g_D3D10_MeshRenderer.render_Skinned(
		m, g_Mesh,
		&g_boneMatrixBuffer[0],
		g_boneMatrixBuffer.size());

	g_Button.render();

	g_Direct8_SoundDevice.get_SoundListener()->CommitDeferredSettings();
}

//--------------------------------------------------------------------------------------
DECL_FUNC_T(void, OnD3D10DestroyDevice)(
	_Inout_ void * _userContext)
{
	g_SkyBox.clear();
	g_MeshAnimation.clear();
	g_Mesh.clear();
}