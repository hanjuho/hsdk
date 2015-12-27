#include "gorenderer.h"



using namespace go;


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(AnimateRenderer, void, update)(
	_X_ void)
{
	if (m_Datatable && my_Meshs)
	{
		direct3d::animation::build_Pos(
			&my_Pos,
			my_Meshs[m_Datatable->type()].anim,
			m_Datatable->state(),
			0.0);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(AnimateRenderer, void, render)(
	_X_ void)
{
	direct3d::g_D3D10_Renderer.set_MatrixWorldViewProj(m_Datatable->matrix());

	direct3d::g_D3D10_Renderer.set_ScalarVSFlag(0);
	direct3d::g_D3D10_Renderer.set_ScalarPSFlag(direct3d::PS_MARERIAL_0 | direct3d::PS_TEXTURE_0);

	GOAnimateMesh & mesh = my_Meshs[m_Datatable->type()];
	m_Datatable->read_Table(&my_Pos.time, 0, 0);
	direct3d::animation::animate_Pos(&my_Pos, mesh.anim);

	direct3d::g_D3D10_Renderer.render_Skinned(mesh, mesh.anim, my_Pos);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(AnimateRenderer, void, link_Model)(
	_Ref_ GOAnimateMesh * _meshs)
{
	my_Meshs = _meshs;
	update();
}
