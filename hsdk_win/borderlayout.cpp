#include <hsdk/win/frame/layout/borderlayout.h>



using namespace hsdk::frame::layout;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(BorderLayout, BorderLayout)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, void, resize)(
	_In_ float _width,
	_In_ float _hegith)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, void, set_Space)(
	_In_ hsdk::i::frame::LAYOUT_SPACE _space,
	_In_ float value)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, float, get_Space)(
	_In_ hsdk::i::frame::LAYOUT_SPACE _space)const
{
	return 0.0f;
}


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(BorderLayout, get_Form)(
	_Out_ float(&_rectangle)[4],
	_In_ hsdk::i::frame::LAYOUT_COMPOSITION _composition)const
{
	return 0;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(BorderLayout, get_Form)(
	_Out_ float(&_rectangle)[4],
	_In_ unsigned int _index)const
{
	return 0;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, void, reset)(
	_X_ void)
{

}