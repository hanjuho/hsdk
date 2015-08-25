#include <hsdk/win/frame/inputeventhelper.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(inputEventHelper, inputEventHelper)(
	/* [in] */ i_Component * _component)
	: my_FocusComponent(_component)
{
	IF_FALSE(_component)
	{
		throw - 1;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(inputEventHelper, inputEventHelper)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(inputEventHelper, bool, chain)(
	/* [in] */ i::frame::i_Component * _component)
{
	IF_FALSE(_component->is_Visible())
	{
		return false;
	}

	int w = xy.x - (int)(_component->get_X());
	if (0 <= w && w <= _component->get_W())
	{
		int h = xy.y - (int)(_component->get_Y());
		if (0 <= h && h <= _component->get_H())
		{
			my_FocusComponent = _component;
			return true;
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(inputEventHelper, void, update)(
	/* [none] */ void)
{
	/*
	설명 : component 갱신 chain 이 유효한지 검사.
	$ 참고 : 여기서 chain 함수를 바로 호출하지 않은 이유는 my_FocusComponent 내부가 갱신되었을 수도 있기 때문.
	*/
	IF_FALSE(my_FocusComponent->event_chain(this))
	{
		// 아래 구문을 반복.
		while (true)
		{
			// 결과
			bool result;

			// 부모 component.
			i_Component * parent;

			/*
			설명 : chain이 실패했다는 것은 마우스의 좌표가 component를 벗어났다는 것.
			그러므로 상위 component를 조사. 이를 반복, 계속 실패할 경우 더이상 상위 컴포넌트가 없을 때 구문 종료.
			*/
			if (parent = my_FocusComponent->parent())
			{
				// 부모 컴포넌트를 조사
				if (result = parent->event_chain(this))
				{
					// chain이 성공했으므로 구문 종료.
					return;
				}
				else
				{
					/*
					설명 : 부모 componenet 조사 실패 상위 컴포넌트로 이동.
					다음 구문에서 부모의 부모 component를 조사.
					*/
					my_FocusComponent = parent;
				}
			}
			else
			{
				// 더 이상 상위 컴포넌트가 없으므로 종료.
				return;
			}
		}
	}

	// 마우스 이벤트 갱신.
	my_FocusMouseable = my_FocusComponent->get_Mouseable();
	
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(inputEventHelper, void, onClick_Down)(
	/* [in] */ i::frame::i_Mouseable::MOUSE_BUTTON _button,
	/* [in] */ int _x,
	/* [in] */ int _y)
{
	// focus 재설정.
	update();

	// event 전달.
	if (my_FocusMouseable)
	{
		my_FocusMouseable->onClick_Down(_button, _x, _y);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(inputEventHelper, void, onClick_Up)(
	/* [in] */ i::frame::i_Mouseable::MOUSE_BUTTON _button,
	/* [in] */ int _x,
	/* [in] */ int _y)
{
	if (my_FocusMouseable)
	{
		my_FocusMouseable->onClick_Up(_button, _x, _y);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(inputEventHelper, void, onDrag)(
	/* [in] */ i::frame::i_Mouseable::MOUSE_BUTTON _button,
	/* [in] */ int _x,
	/* [in] */ int _y)
{
	if (my_FocusMouseable)
	{
		my_FocusMouseable->onDrag(_button, _x, _y);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(inputEventHelper, void, onMove)(
	/* [in] */ int _x,
	/* [in] */ int _y)
{
	if (my_FocusMouseable)
	{
		my_FocusMouseable->onMove(_x, _y);
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(inputEventHelper, void, onWheel)(
	/* [in] */ int _x,
	/* [in] */ int _y,
	/* [in] */ int _w)
{
	if (my_FocusMouseable)
	{
		my_FocusMouseable->onWheel(_x, _y, _w);
	}
}