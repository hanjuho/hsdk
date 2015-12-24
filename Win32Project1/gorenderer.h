#pragma once



#include "gocommon.h"



namespace go
{

	// ���� : 
	DECL_CLASS(AnimateRenderer)
		: hsdk::game::Renderer
	{
	public:

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, initialize)(
			_X_ void);

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, update)(
			_X_ void);

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, render)(
			_X_ void);

	private:

		// ���� : 


		// ���� : 
		direct3d::D3D10_Animation_Recorder my_Pos;

		// ���� :
		D3DXMATRIX my_Form;

	};

}