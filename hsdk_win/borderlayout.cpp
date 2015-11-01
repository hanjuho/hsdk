#include <hsdk/win/frame/layout/borderlayout.h>
#include <Windows.h>
#include <float.h>



using namespace hsdk::frame::layout;
using namespace hsdk::i::frame;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(BorderLayout, BorderLayout)(void)
{
	ZeroMemory(my_Frame, sizeof(my_Frame));
	ZeroMemory(my_LinkedForm, sizeof(my_LinkedForm));
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, void, reset)(
	_X_ void)
{
	ZeroMemory(my_LinkedForm, sizeof(my_LinkedForm));
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, void, resize)(
	_In_ float _width,
	_In_ float _height)
{
	my_Width = _width;
	my_Height = _height;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(BorderLayout, link_Form)(
	_Out_ float(&_rectangle)[4],
	_In_ LAYOUT_COMPOSITION _composition)
{
	if (_composition == COMPOSITION_CENTER)
	{
		_composition = COMPOSITION_DEFALUT;
	}

	// 레이아웃이 이미 있다면 접근 거부
	if (my_LinkedForm[_composition])
	{
		return E_ACCESSDENIED;
	}

	float left = get_Left(_composition);
	float top = get_Top(_composition);
	float right = get_Right(_composition);
	float bottom = get_Bottom(_composition);

	float width = right - left;
	float height = bottom - top;

	switch (_composition)
	{
		// 동쪽
	case COMPOSITION_EAST:

		// 동쪽의 너비가 부족하다면 (중앙, 남, 서)로부터 자리를 나눠옴
		if (width < DBL_EPSILON)
		{
			// 최소 너비
			const float my_WAbs = (my_Width * get_WidthRate()) * 0.3f;

			// 중앙
			if (my_LinkedForm[COMPOSITION_DEFALUT])
			{
				// 새로운 레이아웃 너비 = 중앙 레이아웃 가용 너비 - (동쪽 레이아웃 최소 너비 + 레이아웃 갭)
				float new_Right = my_LinkedForm[COMPOSITION_DEFALUT][SPACE_RIGHT] - (my_WAbs + get_VGapAbs());
				if (0 < new_Right)
				{
					my_LinkedForm[COMPOSITION_DEFALUT][SPACE_RIGHT] = new_Right;
					if (my_LinkedForm[COMPOSITION_SOUTH])
					{
						my_LinkedForm[COMPOSITION_SOUTH][SPACE_RIGHT] = new_Right;
					}

					left = right - my_WAbs;
				}
			}
			// 남쪽
			else if (my_LinkedForm[COMPOSITION_SOUTH])
			{
				// 새로운 레이아웃 너비 = 남쪽 레이아웃 가용 너비 - (동쪽 레이아웃 최소 너비 + 레이아웃 갭)
				float new_Right = my_LinkedForm[COMPOSITION_SOUTH][SPACE_RIGHT] - (my_WAbs + get_VGapAbs());
				if (0 < new_Right)
				{
					my_LinkedForm[COMPOSITION_SOUTH][SPACE_RIGHT] = new_Right;

					left = right - my_WAbs;
				}
			}
			// 서쪽
			else if (my_LinkedForm[COMPOSITION_WEST])
			{
				// 새로운 레이아웃 너비 = 서쪽 레이아웃 가용 너비 - (동쪽 레이아웃 최소 너비 + 레이아웃 갭)
				float new_Right = my_LinkedForm[COMPOSITION_WEST][SPACE_RIGHT] - (my_WAbs + get_VGapAbs());
				if (0 < new_Right)
				{
					my_LinkedForm[COMPOSITION_WEST][SPACE_RIGHT] = new_Right;

					left = right - my_WAbs;
				}
			}
		}

		// 동쪽의 높이가 부족하다면 방법이 없음

		break;

		// 서쪽
	case COMPOSITION_WEST:

		if (width < DBL_EPSILON)
		{
			// 최소 너비
			const float my_WAbs = (my_Width * get_WidthRate()) * 0.3f;

			// 중앙
			if (my_LinkedForm[COMPOSITION_DEFALUT])
			{
				float hgap = get_HGapAbs();

				// 새로운 레이아웃 너비 = 중앙 레이아웃 가용 너비 - (서쪽 레이아웃 최소 너비 + 레이아웃 갭)
				float new_Right = my_LinkedForm[COMPOSITION_DEFALUT][SPACE_RIGHT] - (my_WAbs + hgap);
				if (0 < new_Right)
				{
					right = left + my_WAbs;

					// 새로운 레이아웃 좌표 X = 서쪽 레이아웃 X2 + 레이아웃 갭
					my_LinkedForm[COMPOSITION_DEFALUT][SPACE_LEFT] = right + hgap;
					my_LinkedForm[COMPOSITION_DEFALUT][SPACE_RIGHT] = new_Right;
					if (my_LinkedForm[COMPOSITION_SOUTH])
					{
						my_LinkedForm[COMPOSITION_SOUTH][SPACE_LEFT] = right + hgap;
						my_LinkedForm[COMPOSITION_SOUTH][SPACE_RIGHT] = new_Right;
					}
				}
			}
			// 남쪽
			else if (my_LinkedForm[COMPOSITION_SOUTH])
			{
				float hgap = get_HGapAbs();

				// 새로운 레이아웃 너비 = 남쪽 레이아웃 가용 너비 - (서쪽 레이아웃 최소 너비 + 레이아웃 갭)
				float new_Right = my_LinkedForm[COMPOSITION_SOUTH][SPACE_RIGHT] - (my_WAbs + hgap);
				if (0 < new_Right)
				{
					right = left + my_WAbs;

					// 새로운 레이아웃 좌표 X = 서쪽 레이아웃 X2 + 레이아웃 갭
					my_LinkedForm[COMPOSITION_SOUTH][SPACE_LEFT] = right + hgap;
					my_LinkedForm[COMPOSITION_SOUTH][SPACE_RIGHT] = new_Right;
				}
			}
			// 동쪽
			else if (my_LinkedForm[COMPOSITION_EAST])
			{
				float hgap = get_HGapAbs();

				// 새로운 레이아웃 너비 = 동쪽 레이아웃 가용 너비 - (서쪽 레이아웃 최소 너비 + 레이아웃 갭)
				float new_Right = my_LinkedForm[COMPOSITION_EAST][SPACE_RIGHT] - (my_WAbs + hgap);
				if (0 < new_Right)
				{
					right = left + my_WAbs;

					// 새로운 레이아웃 좌표 X = 서쪽 레이아웃 X2 + 레이아웃 갭
					my_LinkedForm[COMPOSITION_EAST][SPACE_LEFT] = right + hgap;
					my_LinkedForm[COMPOSITION_EAST][SPACE_RIGHT] = new_Right;
				}
			}
		}

		break;

		// 남쪽
	case COMPOSITION_SOUTH:

		if (width < DBL_EPSILON)
		{
			// 중앙
			if (my_LinkedForm[COMPOSITION_DEFALUT])
			{
				left = my_LinkedForm[COMPOSITION_WEST][SPACE_LEFT];
				right = left + my_LinkedForm[COMPOSITION_WEST][SPACE_RIGHT];
			}
		}

		if (height < DBL_EPSILON)
		{
			// 최소 높이
			const float my_HAbs = (my_Height * get_HeightRate()) * 0.3f;

			// 중앙
			if (my_LinkedForm[COMPOSITION_DEFALUT])
			{
				// 새로운 레이아웃 높이 = 중앙 레이아웃 가용 높이 - (남쪽 레이아웃 최소 높이 + 레이아웃 갭)
				float new_Bottom = my_LinkedForm[COMPOSITION_DEFALUT][SPACE_BOTTOM] - (my_HAbs + get_VGapAbs());
				if (0 < new_Bottom)
				{
					my_LinkedForm[COMPOSITION_DEFALUT][SPACE_BOTTOM] = new_Bottom;
					top = bottom - my_HAbs;
				}
			}

			// 남쪽 레이아웃은 더 이상 높이에 간섭할 수 없음
		}

		break;

	case COMPOSITION_NORTH:

		if (height < DBL_EPSILON)
		{
			const float my_HAbs = (my_Height * get_HeightRate()) * 0.2f;
			bottom = top + my_HAbs;

			// 새로운 레이아웃 Y2 = 북쪽 레이아웃 Y2 + 레이아웃 갭
			float new_Top = bottom + get_VGapAbs();

			// 동쪽
			if (my_LinkedForm[COMPOSITION_EAST])
			{
				my_LinkedForm[COMPOSITION_EAST][SPACE_TOP] = new_Top;
				my_LinkedForm[COMPOSITION_EAST][SPACE_BOTTOM] -= my_HAbs;
			}

			// 서쪽
			if (my_LinkedForm[COMPOSITION_WEST])
			{
				my_LinkedForm[COMPOSITION_WEST][SPACE_TOP] = new_Top;
				my_LinkedForm[COMPOSITION_WEST][SPACE_BOTTOM] -= my_HAbs;
			}

			// 중앙
			if (my_LinkedForm[COMPOSITION_DEFALUT])
			{
				my_LinkedForm[COMPOSITION_DEFALUT][SPACE_TOP] = new_Top;
				if (my_LinkedForm[COMPOSITION_WEST][SPACE_BOTTOM] < my_HAbs)
				{
					// 모르겠다
				}
				else
				{
					my_LinkedForm[COMPOSITION_DEFALUT][SPACE_BOTTOM] -= my_HAbs;
				}
			}
		}

		break;

		// 중앙
	case COMPOSITION_DEFALUT:
	case COMPOSITION_CENTER:

		if (width < DBL_EPSILON)
		{
			// 전제 레이아웃 가용 너비
			const float my_WAbs = my_Width * get_WidthRate();
			const float hgap = get_HGapAbs();

			// 서쪽
			if (my_LinkedForm[COMPOSITION_WEST])
			{
				float west_W = my_LinkedForm[COMPOSITION_WEST][SPACE_RIGHT];
				float min_W = my_WAbs * 0.3f;

				// 서쪽 레이아웃 너비가 최소 너비보다 크다면
				if (min_W < west_W)
				{
					my_LinkedForm[COMPOSITION_WEST][SPACE_RIGHT] = min_W;
					left = (my_LinkedForm[COMPOSITION_WEST][SPACE_LEFT] + min_W + hgap);
				}
			}

			// 동쪽
			if (my_LinkedForm[COMPOSITION_EAST])
			{
				float west_W = my_LinkedForm[COMPOSITION_EAST][SPACE_RIGHT];
				float min_W = my_WAbs * 0.3f;

				// 동쪽 레이아웃 너비가 최소 너비보다 크다면
				if (min_W < west_W)
				{
					float newLeft = min_W + get_SpaceAbs(SPACE_RIGHT);
					my_LinkedForm[COMPOSITION_WEST][SPACE_LEFT] = newLeft;
					my_LinkedForm[COMPOSITION_WEST][SPACE_RIGHT] = min_W;

					right = newLeft - hgap;
				}
			}
		}

		if (height < DBL_EPSILON)
		{
			const float my_HAbs = my_Height * get_HeightRate();

			// 남쪽
			if (my_LinkedForm[COMPOSITION_SOUTH])
			{
				float west_H = my_LinkedForm[COMPOSITION_SOUTH][SPACE_BOTTOM];
				float min_H = my_HAbs * 0.3f;

				// 남쪽 레이아웃 높이가 최소 높이보다 크다면
				if (min_H < west_H)
				{
					float newTop = min_H + get_SpaceAbs(SPACE_TOP);

					my_LinkedForm[COMPOSITION_SOUTH][SPACE_TOP] = newTop;
					my_LinkedForm[COMPOSITION_SOUTH][SPACE_BOTTOM] = min_H;

					bottom = newTop - get_VGapAbs();
				}
			}
		}

		break;

	default:

		break;
	}

	width = right - left;
	height = bottom - top;

	IF_FALSE(0 < width && 0 < height)
	{
		return E_INVALIDARG;
	}

	_rectangle[SPACE_LEFT] = left;
	_rectangle[SPACE_TOP] = top;
	_rectangle[SPACE_RIGHT] = width;
	_rectangle[SPACE_BOTTOM] = height;

	my_LinkedForm[_composition] = _rectangle;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(BorderLayout, unlink_Form)(
	_In_ float(&_rectangle)[4],
	_In_ LAYOUT_COMPOSITION _composition)
{
	if (my_LinkedForm[_composition] == _rectangle)
	{
		my_LinkedForm[_composition] = nullptr;
		return S_OK;
	}

	return E_INVALIDARG;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, void, set_Space)(
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

		my_Frame[SPACE_LEFT] = min(_value, 1.0f - (my_Frame[SPACE_RIGHT] + (my_HGap * 2)));

		return;
	case SPACE_TOP:

		my_Frame[SPACE_TOP] = min(_value, 1.0f - (my_Frame[SPACE_BOTTOM] + (my_VGap * 2)));

		return;
	case SPACE_RIGHT:

		my_Frame[SPACE_RIGHT] = min(_value, 1.0f - (my_Frame[SPACE_LEFT] + (my_HGap * 2)));

		return;
	case SPACE_BOTTOM:

		my_Frame[SPACE_BOTTOM] = min(_value, 1.0f - (my_Frame[SPACE_TOP] + (my_VGap * 2)));

		return;
	default:
		return;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, float, get_Space)(
	_In_ LAYOUT_SPACE _space)const
{
	return my_Frame[_space];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, void, set_HGap)(
	_In_ float _value)
{
	IF_FALSE(_value < 0.5f)
	{
		return;
	}

	my_HGap = min(_value, (1.0f - (my_Frame[SPACE_LEFT] + my_Frame[SPACE_RIGHT])) * 0.5);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, float, get_HGap)(
	_X_  void)const
{
	return my_HGap;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, void, set_VGap)(
	_In_ float _value)
{
	IF_FALSE(_value < 0.5f)
	{
		return;
	}

	my_VGap = min(_value, (1.0f - (my_Frame[SPACE_TOP] + my_Frame[SPACE_BOTTOM])) * 0.5);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, float, get_VGap)(
	_X_ void)const
{
	return my_VGap;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, float, get_WidthRate)(
	_X_ void)const
{
	return 1.0f - (my_Frame[SPACE_LEFT] + my_Frame[SPACE_RIGHT] + (my_HGap * 2.0f));
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, float, get_HeightRate)(
	_X_ void)const
{
	return 1.0f - (my_Frame[SPACE_TOP] + my_Frame[SPACE_BOTTOM] + (my_VGap * 2.0f));
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, float, get_SpaceAbs)(
	_In_ LAYOUT_SPACE _space)const
{
	switch (_space)
	{
	case hsdk::i::frame::SPACE_LEFT:

		return my_Width * my_Frame[_space];

	case hsdk::i::frame::SPACE_TOP:

		return my_Height * my_Frame[_space];

	case hsdk::i::frame::SPACE_RIGHT:

		return my_Width * (1.0f - my_Frame[_space]);

	case hsdk::i::frame::SPACE_BOTTOM:

		return my_Height * (1.0f - my_Frame[_space]);

	default:
		return 0.0f;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, float, get_HGapAbs)(
	_In_ void)const
{
	return my_Width * my_HGap;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, float, get_VGapAbs)(
	_X_ void)const
{
	return my_Height * my_VGap;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, float, get_Left)(
	_In_ LAYOUT_COMPOSITION _composition)const
{
	float value = get_SpaceAbs(SPACE_LEFT);
	float hgap = get_HGapAbs();

	switch (_composition)
	{
		// 서쪽
	case COMPOSITION_WEST:

		// 북쪽
	case COMPOSITION_NORTH:

		return value;

		// 동쪽
	case COMPOSITION_EAST:

	{
							 // 중앙
							 if (my_LinkedForm[COMPOSITION_DEFALUT])
							 {
								 value = my_LinkedForm[COMPOSITION_DEFALUT][SPACE_LEFT] +
									 my_LinkedForm[COMPOSITION_DEFALUT][SPACE_RIGHT];

								 // 레이아웃간 차이만큼 다시 증가
								 value += hgap;
							 }
							 // 서쪽
							 else if (my_LinkedForm[COMPOSITION_WEST])
							 {
								 value += my_LinkedForm[COMPOSITION_WEST][SPACE_RIGHT];

								 // 레이아웃간 차이만큼 다시 증가
								 value += hgap;
							 }
	}

		return value;

		// 남쪽
	case COMPOSITION_SOUTH:

	{
							  // 서쪽
							  if (my_LinkedForm[COMPOSITION_WEST])
							  {
								  value += my_LinkedForm[COMPOSITION_WEST][SPACE_RIGHT];

								  // 레이아웃간 차이만큼 다시 증가
								  value += hgap;
							  }
	}

		return value;

		// 중앙
	case COMPOSITION_DEFALUT:
	case COMPOSITION_CENTER:

	{
							   // 서쪽
							   if (my_LinkedForm[COMPOSITION_WEST])
							   {
								   value += my_LinkedForm[COMPOSITION_WEST][SPACE_RIGHT];

								   // 레이아웃간 차이만큼 다시 증가
								   value += hgap;
							   }
	}

		return value;

	default:

		return 0.0f;

	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, float, get_Top)(
	_In_ LAYOUT_COMPOSITION _composition)const
{
	float value;

	// 북쪽
	if (my_LinkedForm[COMPOSITION_NORTH])
	{
		// 북쪽은 최상층, 지배자
		value = my_LinkedForm[COMPOSITION_NORTH][SPACE_LEFT] +
			my_LinkedForm[COMPOSITION_NORTH][SPACE_RIGHT];

		// 레이아웃간 차이만큼 다시 증가
		value += get_VGapAbs();
	}
	else
	{
		value = get_SpaceAbs(SPACE_TOP);

		if (_composition == COMPOSITION_NORTH)
		{
			return value;
		}
	}

	switch (_composition)
	{
		// 동쪽
	case COMPOSITION_EAST:

		// 서쪽
	case COMPOSITION_WEST:

		// 중앙
	case COMPOSITION_DEFALUT:
	case COMPOSITION_CENTER:

		return value;

		// 남쪽
	case COMPOSITION_SOUTH:

	{
							  // 중앙
							  if (my_LinkedForm[COMPOSITION_DEFALUT])
							  {
								  value += my_LinkedForm[COMPOSITION_DEFALUT][SPACE_BOTTOM];

								  // 레이아웃간 차이만큼 다시 증가
								  value += get_VGapAbs();
							  }
	}

		return value;

	default:

		return 0.0f;

	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, float, get_Right)(
	_In_ LAYOUT_COMPOSITION _composition)const
{
	float value = get_SpaceAbs(SPACE_RIGHT);
	float hgap = get_HGapAbs();

	switch (_composition)
	{
		// 동쪽
	case COMPOSITION_EAST:

		// 북쪽
	case COMPOSITION_NORTH:

		return value;

		// 서쪽
	case COMPOSITION_WEST:

	{
							 // 중앙
							 if (my_LinkedForm[COMPOSITION_DEFALUT])
							 {
								 value = my_LinkedForm[COMPOSITION_DEFALUT][SPACE_LEFT];

								 // 레이아웃간 차이만큼 다시 감소
								 value -= hgap;
							 }
							 // 동쪽
							 else if (my_LinkedForm[COMPOSITION_EAST])
							 {
								 value = my_LinkedForm[COMPOSITION_EAST][SPACE_LEFT];

								 // 레이아웃간 차이만큼 다시 감소
								 value -= hgap;
							 }
	}

		return value;

		// 남쪽
	case COMPOSITION_SOUTH:

	{
							  // 동쪽
							  if (my_LinkedForm[COMPOSITION_EAST])
							  {
								  value = my_LinkedForm[COMPOSITION_EAST][SPACE_LEFT];

								  // 레이아웃간 차이만큼 다시 감소
								  value -= hgap;
							  }
	}

		return value;

		// 중앙
	case COMPOSITION_DEFALUT:
	case COMPOSITION_CENTER:

	{
							   // 동쪽
							   if (my_LinkedForm[COMPOSITION_EAST])
							   {
								   value = my_LinkedForm[COMPOSITION_EAST][SPACE_LEFT];

								   // 레이아웃간 차이만큼 다시 감소
								   value -= hgap;
							   }
	}

		return value;

	default:

		return 0.0f;

	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(BorderLayout, float, get_Bottom)(
	_In_ LAYOUT_COMPOSITION _composition)const
{
	float value = get_SpaceAbs(SPACE_BOTTOM);
	float vgap = get_VGapAbs();

	switch (_composition)
	{
		// 동쪽
	case COMPOSITION_EAST:

		// 서쪽
	case COMPOSITION_WEST:

		// 남쪽
	case COMPOSITION_SOUTH:

		return value;

	case COMPOSITION_NORTH:

	{
							  // 동쪽
							  if (my_LinkedForm[COMPOSITION_EAST])
							  {
								  value = my_LinkedForm[COMPOSITION_EAST][SPACE_TOP];

								  // 레이아웃간 차이만큼 다시 감소
								  value -= vgap;
							  }
							  // 서쪽
							  else if (my_LinkedForm[COMPOSITION_WEST])
							  {
								  value = my_LinkedForm[COMPOSITION_WEST][SPACE_TOP];

								  // 레이아웃간 차이만큼 다시 감소
								  value -= vgap;
							  }
							  // 중앙
							  else if (my_LinkedForm[COMPOSITION_CENTER])
							  {
								  value = my_LinkedForm[COMPOSITION_CENTER][SPACE_TOP];

								  value -= vgap;

							  }
							  // 남쪽
							  else if (my_LinkedForm[COMPOSITION_SOUTH])
							  {
								  value = my_LinkedForm[COMPOSITION_SOUTH][SPACE_TOP];

								  // 레이아웃간 차이만큼 다시 감소
								  value -= vgap;
							  }
	}
		return value;

		// 중앙
	case COMPOSITION_DEFALUT:
	case COMPOSITION_CENTER:

	{
							   // 남쪽
							   if (my_LinkedForm[COMPOSITION_SOUTH])
							   {
								   value = my_LinkedForm[COMPOSITION_SOUTH][SPACE_TOP];

								   // 레이아웃간 차이만큼 다시 감소
								   value -= vgap;
							   }
	}
		return value;

	default:

		return 0.0f;

	}
}