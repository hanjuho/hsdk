#include <game/modelrenderer.h>



using namespace hsdk;
using namespace game;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(ModelRenderer, ModelRenderer)(void)
: m_Datatable(nullptr)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(ModelRenderer, ModelRenderer)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelRenderer, void, initialize)(
	_X_ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelRenderer, void, render)(
	_X_ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(ModelRenderer, void, link_DataTable)(
	/* [ref] */ i::i_DataTable * _datatable)
{
	m_Datatable = _datatable;
}
