#include "modelviewcompo.h"



using namespace gamecompo;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(ModelViewCompo, ModelViewCompo)(
	_In_ frame::PARENT_RELATION _relation,
	_In_ const wchar_t * _path,
	_In_ const wchar_t ** _names,
	_In_ unsigned int _size)
	: Component(_relation)
{
	if (_size < 1)
	{
		throw E_INVALIDARG;
	}

	my_Models.resize(_size);

	HRESULT hr = E_FAIL;
	for (unsigned int index = 0; index < _size; ++index)
	{
		Model & model = my_Models[index];
		if (SUCCEEDED(hr = direct3d::g_D3D10_Factory.build_MeshFromFile(
			model.mesh,
			_path,
			_names[index],
			&model.anim)))
		{
			direct3d::animation::build_Pos(
				model.pos,
				model.anim,
				0,
				0.0f);
		}
		else
		{
			throw hr;
		}
	}

	my_Camera.set_Position(D3DXVECTOR3(0.0f, 1.0f, 3.0f));
	my_Camera.set_Target(D3DXVECTOR3(0.0f, 0.5f, 0.0f));
	my_Camera.rotate_ZAxis(D3DX_PI);

	my_Camera.compute_ViewMatrix(my_mView);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelViewCompo, void, onMouse_Enter)(
	_In_ int _x,
	_In_ int _y)
{
	my_CameraControl = true;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelViewCompo, void, onMouse_Exit)(
	_In_ int _x,
	_In_ int _y)
{
	my_CameraControl = false;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelViewCompo, void, onDrag)(
	_In_ i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	if (my_CameraControl)
	{
		my_Camera.rotate_YAxis(D3DXToDegree(_x) * 0.001f, true);
		my_Camera.rotate_XAxis(D3DXToDegree(_y) * 0.001f, true);

		my_Camera.compute_ViewMatrix(my_mView);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelViewCompo, void, update)(
	_X_ void)
{
	my_Models[my_ViewModel].pos.time +=
		framework::g_Framework_TimeStream.get_ElapsedTime();

	direct3d::animation::animate_Pos(
		my_Models[my_ViewModel].pos,
		my_Models[my_ViewModel].anim);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelViewCompo, void, reform)(
	_X_ void)
{
	Component::reform();

	my_RenderTarget.initialize(
		(unsigned int)get_W(),
		(unsigned int)get_H());

	// Initialize the projection matrix
	D3DXMatrixPerspectiveFovLH(
		&my_mProj,
		(float)D3DX_PI * 0.25f,
		get_W() / get_H(), 0.1f, 1000.0f);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelViewCompo, void, render)(
	_X_ void)
{
	if (is_Visible())
	{
		IF_SUCCEEDED(my_RenderTarget.begin(m_Graphics.bgColor))
		{
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

			D3DXMATRIX t;
			direct3d::g_D3D10_Renderer.set_MatrixWorldViewProj(D3DXMatrixMultiply(&t, &my_mView, &my_mProj));

			direct3d::g_D3D10_Renderer.set_ScalarVSFlag(0);
			direct3d::g_D3D10_Renderer.set_ScalarPSFlag(direct3d::PS_TEXTURE_0);

			direct3d::g_D3D10_Renderer.render_Skinned(
				my_Models[my_ViewModel].mesh,
				my_Models[my_ViewModel].anim,
				my_Models[my_ViewModel].pos, 1);

			my_RenderTarget.end();
		}

		my_RenderTarget.viewport();

		direct3d::g_D3D10_Renderer.set_MatrixWorldViewProj(&m_Position);
		direct3d::g_D3D10_Renderer.set_ScalarVSFlag(0);
		direct3d::g_D3D10_Renderer.set_ScalarPSFlag(direct3d::PS_TEXTURE_0);
		direct3d::g_D3D10_Renderer.render_UITexture(
			my_RenderTarget.get_View(), &direct3d::g_D3D10_identityMatrix);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelViewCompo, void, select_Model)(
	_In_ unsigned int _index,
	_In_ unsigned int _animation)
{
	if (_index < my_Models.size())
	{
		Model & refmodel = my_Models[my_ViewModel];
		my_ViewModel = _index;

		refmodel.pos.time = 0.0f;
		refmodel.pos.aniamtionID =
			_animation % refmodel.anim.animations.size();

		direct3d::animation::reset_Pos(
			refmodel.pos, refmodel.anim);
	}
}