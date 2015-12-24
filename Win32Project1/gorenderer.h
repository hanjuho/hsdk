#pragma once



#include "gocommon.h"



namespace go
{

	// 설명 : 
	DECL_CLASS(AnimateRenderer)
		: hsdk::game::Renderer
	{
	public:

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, initialize)(
			_X_ void);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, update)(
			_X_ void);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, render)(
			_X_ void);

	private:

		// 설명 : 


		// 설명 : 
		direct3d::D3D10_Animation_Recorder my_Pos;

		// 설명 :
		D3DXMATRIX my_Form;

	};

}