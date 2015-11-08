#pragma once



#include "gocommon.h"



namespace go
{

	// ���� : 
	DECL_CLASS(AnimateModelRenderer)
		: game::ModelRenderer
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
		D3DXMATRIX my_Form;

		// ���� : 
		direct3d::D3D10_Animation_Recorder my_Pos;

	};

}