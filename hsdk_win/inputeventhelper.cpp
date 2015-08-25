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
	���� : component ���� chain �� ��ȿ���� �˻�.
	$ ���� : ���⼭ chain �Լ��� �ٷ� ȣ������ ���� ������ my_FocusComponent ���ΰ� ���ŵǾ��� ���� �ֱ� ����.
	*/
	IF_FALSE(my_FocusComponent->event_chain(this))
	{
		// �Ʒ� ������ �ݺ�.
		while (true)
		{
			// ���
			bool result;

			// �θ� component.
			i_Component * parent;

			/*
			���� : chain�� �����ߴٴ� ���� ���콺�� ��ǥ�� component�� ����ٴ� ��.
			�׷��Ƿ� ���� component�� ����. �̸� �ݺ�, ��� ������ ��� ���̻� ���� ������Ʈ�� ���� �� ���� ����.
			*/
			if (parent = my_FocusComponent->parent())
			{
				// �θ� ������Ʈ�� ����
				if (result = parent->event_chain(this))
				{
					// chain�� ���������Ƿ� ���� ����.
					return;
				}
				else
				{
					/*
					���� : �θ� componenet ���� ���� ���� ������Ʈ�� �̵�.
					���� �������� �θ��� �θ� component�� ����.
					*/
					my_FocusComponent = parent;
				}
			}
			else
			{
				// �� �̻� ���� ������Ʈ�� �����Ƿ� ����.
				return;
			}
		}
	}

	// ���콺 �̺�Ʈ ����.
	my_FocusMouseable = my_FocusComponent->get_Mouseable();
	
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(inputEventHelper, void, onClick_Down)(
	/* [in] */ i::frame::i_Mouseable::MOUSE_BUTTON _button,
	/* [in] */ int _x,
	/* [in] */ int _y)
{
	// focus �缳��.
	update();

	// event ����.
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