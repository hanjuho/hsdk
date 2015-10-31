#pragma once



#include "common.h"



namespace gamecompo
{

	// 설명 : 
	DECL_CLASS(ModelViewCompo)
		: public frame::Component
	{

		// 설명 : 
		DECL_STRUCT(Model)
		{

			// 설명 : 
			direct3d::D3D10_Mesh mesh;

			// 설명 : 
			direct3d::D3D10_Animation meshAnimation;

			// 설명 : 
			direct3d::D3D10_Animation_Recorder meshPos;

		};

	public:

		// 생성자.
		CLASS_DECL_CONSTRUCTOR(ModelViewCompo)(
			_In_ frame::PARENT_RELATION _relation,
			_In_ const wchar_t * _path,
			_In_ const wchar_t ** _names,
			_In_ unsigned int _size);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, onMouse_Enter)(
			_In_ int _x,
			_In_ int _y);

		// 설명 : 
		INTERFACE_DECL_FUNC_T(void, onMouse_Exit)(
			_In_ int _x,
			_In_ int _y);

		// 설명 : mouse의 커서를 이동시키면 발생하는 event.
		INTERFACE_DECL_FUNC_T(void, onDrag)(
			_In_ i::frame::MOUSE_BUTTON _button,
			_In_ int _x,
			_In_ int _y);

		// 설명 : component 갱신.
		INTERFACE_DECL_FUNC_T(void, update)(
			_X_ void);

		// 설명 : component의 좌표를 갱신.
		INTERFACE_DECL_FUNC_T(void, reform)(
			_X_ void);

		// 설명 : component를 화면에 뿌려줌.
		INTERFACE_DECL_FUNC_T(void, render)(
			_X_ void);

		// 설명 :
		CLASS_DECL_FUNC_T(void, select_Model)(
			_In_ unsigned int _index);

	protected:
		
		// 설명 :
		bool my_CameraControl = false;

		// 설명 : 
		framework::Framework_Camera my_Camera;

		// 설명 : 
		D3DXMATRIX my_mView = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f, };

		// 설명 : 
		D3DXMATRIX my_mProj = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f, };

		// 설명 : 
		direct3d::D3D10_RenderTarget my_RenderTarget;

		// 설명 : 
		std::vector<Model> my_Models;

		// 설명 :
		unsigned int my_ViewModel = 0;

	};
}
