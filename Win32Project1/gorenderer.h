#pragma once



#include "gocommon.h"



namespace go
{

	// 설명 : 
	DECL_CLASS(AnimateRenderer)
		: public hsdk::game::Renderer
	{
	public:

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, update)(
			_X_ void);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, render)(
			_X_ void);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, link_Model)(
			_Ref_ GOAnimateMesh * _meshs);

	private:

		// 설명 : 
		GOAnimateMesh * my_Meshs = nullptr;

		// 설명 : 
		direct3d::D3D10_Animation_Recorder my_Pos;

	};

}