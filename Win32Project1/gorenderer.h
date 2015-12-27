#pragma once



#include "gocommon.h"



namespace go
{

	// ���� : 
	DECL_CLASS(AnimateRenderer)
		: public hsdk::game::Renderer
	{
	public:

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, update)(
			_X_ void);

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, render)(
			_X_ void);

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, link_Model)(
			_Ref_ GOAnimateMesh * _meshs);

	private:

		// ���� : 
		GOAnimateMesh * my_Meshs = nullptr;

		// ���� : 
		direct3d::D3D10_Animation_Recorder my_Pos;

	};

}