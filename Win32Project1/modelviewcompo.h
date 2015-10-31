#pragma once



#include "common.h"



namespace gamecompo
{

	// ���� : 
	DECL_CLASS(ModelViewCompo)
		: public frame::Component
	{

		// ���� : 
		DECL_STRUCT(Model)
		{

			// ���� : 
			direct3d::D3D10_Mesh mesh;

			// ���� : 
			direct3d::D3D10_Animation meshAnimation;

			// ���� : 
			direct3d::D3D10_Animation_Recorder meshPos;

		};

	public:

		// ������.
		CLASS_DECL_CONSTRUCTOR(ModelViewCompo)(
			_In_ frame::PARENT_RELATION _relation,
			_In_ const wchar_t * _path,
			_In_ const wchar_t ** _names,
			_In_ unsigned int _size);

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, onMouse_Enter)(
			_In_ int _x,
			_In_ int _y);

		// ���� : 
		INTERFACE_DECL_FUNC_T(void, onMouse_Exit)(
			_In_ int _x,
			_In_ int _y);

		// ���� : mouse�� Ŀ���� �̵���Ű�� �߻��ϴ� event.
		INTERFACE_DECL_FUNC_T(void, onDrag)(
			_In_ i::frame::MOUSE_BUTTON _button,
			_In_ int _x,
			_In_ int _y);

		// ���� : component ����.
		INTERFACE_DECL_FUNC_T(void, update)(
			_X_ void);

		// ���� : component�� ��ǥ�� ����.
		INTERFACE_DECL_FUNC_T(void, reform)(
			_X_ void);

		// ���� : component�� ȭ�鿡 �ѷ���.
		INTERFACE_DECL_FUNC_T(void, render)(
			_X_ void);

		// ���� :
		CLASS_DECL_FUNC_T(void, select_Model)(
			_In_ unsigned int _index);

	protected:
		
		// ���� :
		bool my_CameraControl = false;

		// ���� : 
		framework::Framework_Camera my_Camera;

		// ���� : 
		D3DXMATRIX my_mView = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f, };

		// ���� : 
		D3DXMATRIX my_mProj = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f, };

		// ���� : 
		direct3d::D3D10_RenderTarget my_RenderTarget;

		// ���� : 
		std::vector<Model> my_Models;

		// ���� :
		unsigned int my_ViewModel = 0;

	};
}
