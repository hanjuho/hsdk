#include "modelviewercompo.h"



using namespace compo;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(ModelViewerCompo, ModelViewerCompo)(
	_In_ frame::PARENT_RELATION _relation,
	_In_ const wchar_t * _path,
	_In_ const wchar_t ** _names,
	_In_ unsigned int _size)
	: Component(_relation),
	my_vTarget(0.0f, 0.5f, 0.0f),
	my_vPos(0.0f, 1.0f, 3.0f),
	my_vUp(0.0f, 1.0f, 0.0f)
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
			&model.mesh,
			_path,
			_names[index],
			&model.anim)))
		{
			if (hr & 0x01)
			{
				model.animation = true;
				direct3d::animation::build_Pos(
					&model.pos, model.anim,	0, 0.0f);
			}
		}
		else
		{
			throw hr;
		}
	}

	D3DXMatrixLookAtLH(&my_mView, &(my_vTarget + my_vPos), &my_vTarget, &my_vUp);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelViewerCompo, void, onMouse_Enter)(
	_In_ int _x,
	_In_ int _y)
{
	my_CameraControl = true;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelViewerCompo, void, onMouse_Exit)(
	_In_ int _x,
	_In_ int _y)
{
	my_CameraControl = false;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelViewerCompo, void, onDrag)(
	_In_ i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	if (my_CameraControl)
	{
		D3DXMATRIX r1, r2;
		D3DXVECTOR3 cross;

		D3DXVec3Normalize(&cross, &my_vPos);
		D3DXMatrixRotationAxis(&r1, D3DXVec3Cross(&cross, &cross, &my_vUp), D3DXToDegree(_y) * 0.001f);
		D3DXMatrixRotationY(&r2, D3DXToDegree(_x) * 0.001f);

		D3DXVec3TransformNormal(&my_vPos, &my_vPos, &(r1 * r2));

		D3DXMatrixLookAtLH(&my_mView, &(my_vTarget + my_vPos), &my_vTarget, &my_vUp);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelViewerCompo, void, onWheel)(
	_In_ int _x,
	_In_ int _y,
	_In_ int _w)
{
	if (my_CameraControl)
	{
		D3DXVECTOR3 normal;
		D3DXVec3Normalize(&normal, &my_vPos);
		my_vPos += (normal * float(_w / 120));

		D3DXMatrixLookAtLH(&my_mView, &(my_vTarget + my_vPos), &my_vTarget, &my_vUp);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelViewerCompo, void, update)(
	_X_ void)
{
	if (my_Models[my_ViewModel].animation)
	{
		my_Models[my_ViewModel].pos.time +=
			framework::g_Framework_TimeStream.get_ElapsedTime();

		direct3d::animation::animate_Pos(
			&my_Models[my_ViewModel].pos,
			my_Models[my_ViewModel].anim);
	}

	if (parent())
	{
		parent()->redraw();
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelViewerCompo, void, reform)(
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
CLASS_IMPL_FUNC_T(ModelViewerCompo, void, render)(
	_X_ void)
{
	if (is_Visible())
	{
		IF_SUCCEEDED(my_RenderTarget.begin(m_Graphics.bgColor))
		{
			direct3d::g_D3D10_Renderer.set_MatrixWorldViewProj(&direct3d::g_D3D10_identityMatrix);
			direct3d::g_D3D10_Renderer.set_ScalarPSTime(1.0f);
			if (m_Graphics.refTexture)
			{
				direct3d::g_D3D10_Renderer.set_ScalarVSFlag(0);
				direct3d::g_D3D10_Renderer.set_ScalarPSFlag(direct3d::PS_TEXTURE_0 | direct3d::PS_CALLFUNCTION_0 | direct3d::PS_TEXMATRIX_0);
				direct3d::g_D3D10_Renderer.render_UITexture(
					m_Graphics.refTexture,
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
			direct3d::g_D3D10_Renderer.set_ScalarPSFlag(direct3d::PS_MARERIAL_0 | direct3d::PS_TEXTURE_0);

			if (my_Models[my_ViewModel].animation)
			{
				direct3d::g_D3D10_Renderer.render_Skinned(
					my_Models[my_ViewModel].mesh,
					my_Models[my_ViewModel].anim,
					my_Models[my_ViewModel].pos, 1);
			}
			else
			{
				direct3d::g_D3D10_Renderer.render_Mesh(
					my_Models[my_ViewModel].mesh, 2);
			}

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
CLASS_IMPL_FUNC_T(ModelViewerCompo, void, select_Model)(
	_In_ unsigned int _index,
	_In_ unsigned int _animation)
{
	if (_index < my_Models.size())
	{
		Model & refmodel = my_Models[_index];
		my_ViewModel = _index;

		IF_FALSE(refmodel.animation)
		{
			return;
		}

		refmodel.pos.time = 0.0f;
		refmodel.pos.aniamtionID =
			_animation % refmodel.anim.animations.size();

		direct3d::animation::reset_Pos(
			&refmodel.pos, refmodel.anim);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelViewerCompo, unsigned int, get_NumModel)(
	_X_ void)
{
	return my_ViewModel;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelViewerCompo, unsigned int, get_NumModelAnimation)(
	_X_ void)
{
	return my_Models[my_ViewModel].pos.aniamtionID;
}