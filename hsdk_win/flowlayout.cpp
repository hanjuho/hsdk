#include <hsdk/win/frame/layout/flowlayout.h>
#include <Windows.h>
#include <float.h>



using namespace hsdk::frame::layout;
using namespace hsdk::i::frame;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(FlowLayout, FlowLayout)(
	_X_ FLOW_LAYOUT _layout)
	: my_Layout(_layout)
{
	ZeroMemory(my_Frame, sizeof(my_Frame));
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FlowLayout, void, reset)(
	_X_ void)
{
	my_LinkedForm.clear();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FlowLayout, void, resize)(
	_In_ float _width,
	_In_ float _height)
{
	unsigned int numHorizon = (unsigned int)(_width / my_EachWidthAbs);

	float width = _width * (1.0f - (get_Space(SPACE_LEFT) + get_Space(SPACE_RIGHT)));
	my_NumHorizon = (unsigned int)(width / my_EachWidthAbs);
	my_NumHorizon = max(1, my_NumHorizon);

	unsigned int numVertical = (unsigned int)(_height / my_EachHeightAbs);
	float height = _height * (1.0f - (get_Space(SPACE_TOP) + get_Space(SPACE_BOTTOM)));
	my_NumVertical = (unsigned int)(height / my_EachHeightAbs);
	my_NumVertical = max(1, my_NumVertical);

	my_Width = _width;
	my_Height = _height;

	set_Space(SPACE_LEFT, my_Frame[SPACE_LEFT]);
	set_Space(SPACE_TOP, my_Frame[SPACE_TOP]);
	set_Space(SPACE_RIGHT, my_Frame[SPACE_RIGHT]);
	set_Space(SPACE_BOTTOM, my_Frame[SPACE_BOTTOM]);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(FlowLayout, link_Form)(
	_Out_ float(&_rectangle)[4],
	_In_ LAYOUT_COMPOSITION _composition)
{
	unsigned int size = my_LinkedForm.size();

	unsigned int column;
	unsigned int row;

	if (my_Layout == FLOW_HORIZON)
	{
		column = (size / my_NumHorizon);
		row = size % my_NumHorizon;
	}
	else
	{
		row = (size / my_NumVertical);
		column = size % my_NumVertical;
	}

	float left = get_SpaceAbs(SPACE_LEFT) + (row * my_EachWidthAbs) + (get_HGapAbs() * row);
	float top = get_SpaceAbs(SPACE_TOP) + (column * my_EachHeightAbs) + (get_VGapAbs() * column);
	float right = left + my_EachWidthAbs;
	float bottom = top + my_EachHeightAbs;

	_rectangle[0] = left;
	_rectangle[1] = top;
	_rectangle[2] = right - left;
	_rectangle[3] = bottom - top;
	my_LinkedForm.push_back(_rectangle);

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(FlowLayout, unlink_Form)(
	_In_ float(&_rectangle)[4],
	_In_ LAYOUT_COMPOSITION _composition)
{
	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FlowLayout, void, set_Space)(
	_In_ LAYOUT_SPACE _space,
	_In_ float _value)
{
	IF_FALSE(_value < 1.0f)
	{
		return;
	}

	switch (_space)
	{
	case SPACE_LEFT:

		my_Frame[SPACE_LEFT] = min(_value, 1.0f - (my_Frame[SPACE_RIGHT] + (my_HGap * my_NumHorizon)));

		return;
	case SPACE_TOP:

		my_Frame[SPACE_TOP] = min(_value, 1.0f - (my_Frame[SPACE_BOTTOM] + (my_VGap * my_NumVertical)));

		return;
	case SPACE_RIGHT:

		my_Frame[SPACE_RIGHT] = min(_value, 1.0f - (my_Frame[SPACE_LEFT] + (my_HGap * my_NumHorizon)));

		return;
	case SPACE_BOTTOM:

		my_Frame[SPACE_BOTTOM] = min(_value, 1.0f - (my_Frame[SPACE_TOP] + (my_VGap * my_NumVertical)));

		return;
	default:
		return;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FlowLayout, float, get_Space)(
	_In_ LAYOUT_SPACE _space)const
{
	return my_Frame[_space];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FlowLayout, void, set_HGap)(
	_In_ float _value)
{
	my_HGap = _value;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FlowLayout, float, get_HGap)(
	_X_  void)const
{
	return my_HGap;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FlowLayout, void, set_VGap)(
	_In_ float _value)
{
	my_VGap = _value;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FlowLayout, float, get_VGap)(
	_X_ void)const
{
	return my_VGap;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FlowLayout, float, get_SpaceAbs)(
	_In_ LAYOUT_SPACE _space)const
{
	switch (_space)
	{
	case hsdk::i::frame::SPACE_LEFT:

		return my_Width * my_Frame[_space];

	case hsdk::i::frame::SPACE_TOP:

		return my_Width * my_Frame[_space];

	case hsdk::i::frame::SPACE_RIGHT:

		return my_Width * (1.0f - my_Frame[_space]);

	case hsdk::i::frame::SPACE_BOTTOM:

		return my_Height * (1.0f - my_Frame[_space]);

	default:
		return 0.0f;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FlowLayout, float, get_HGapAbs)(
	_In_ void)const
{
	return my_Width * my_HGap;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FlowLayout, float, get_VGapAbs)(
	_X_ void)const
{
	return my_Height * my_VGap;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FlowLayout, void, set_EachWidthAbs)(
	_In_ float _value)
{
	my_EachWidthAbs = _value;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FlowLayout, void, set_EachHeightAbs)(
	_In_ float _value)
{
	my_EachHeightAbs = _value;
}
