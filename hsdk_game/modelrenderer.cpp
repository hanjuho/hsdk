#include <game/modelrenderer.h>



using namespace hsdk;
using namespace game;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(ModelRenderer, ModelRenderer)(void)
: m_Datatable(nullptr)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(ModelRenderer, ModelRenderer)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ModelRenderer, void, initialize)(
	/* [x] */ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ModelRenderer, void, render)(
	/* [x] */ void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(ModelRenderer, void, link_DataTable)(
	/* [ref] */ i::i_DataTable * _datatable)
{
	m_Datatable = _datatable;
}
