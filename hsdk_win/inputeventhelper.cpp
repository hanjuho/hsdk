#include <hsdk/win/frame/inputeventhelper.h>
#include <hsdk/win/frame/component.h>



using namespace hsdk::frame;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(inputEventHelper, inputEventHelper)(
	_In_ hsdk::i::frame::i_Component * _component)
	: my_MainComponent(_component), my_FocusComponent(_component)
{
	IF_INVALID(_component)
	{
		throw HSDK_FAIL;
	}

	xy.x = 0;
	xy.y = 0;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(inputEventHelper, inputEventHelper)(void)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(inputEventHelper, bool, chain)(
	_In_ const hsdk::i::frame::i_Component * _component)
{
	IF_FALSE(_component->is_Visible())
	{
		return false;
	}

	Component * component;
	component = (Component *)(_component);
	IF_FALSE(component)
	{
		return false;
	}

	int w = xy.x - (int)(component->get_AbsX());
	if (0 <= w && w <= component->get_W())
	{
		int h = xy.y - (int)(component->get_AbsY());
		if (0 <= h && h <= component->get_H())
		{
			if (my_FocusComponent != component)
			{
				my_FocusComponent->onMouse_Exit(xy.x, xy.y);
				component->onMouse_Enter(xy.x, xy.y);
			}

			my_FocusComponent = component;

			return true;
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(inputEventHelper, void, update)(
	_X_ void)
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
			BOOL result;

			// �θ� component.
			hsdk::i::frame::i_Component * parent;

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
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(inputEventHelper, void, restore)(
	_X_ void) 
{
	my_FocusComponent = my_MainComponent;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(inputEventHelper, void, onClick_Down)(
	_In_ hsdk::i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	xy.x = _x;
	xy.y = _y;

	// focus �缳��.
	update();

	// event ����.
	if (my_FocusComponent)
	{
		my_FocusComponent->onClick_Down(_button, _x, _y);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(inputEventHelper, void, onClick_Up)(
	_In_ hsdk::i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	xy.x = _x;
	xy.y = _y;
	
	// event ����.
	if (my_FocusComponent)
	{
		my_FocusComponent->onClick_Up(_button, _x, _y);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(inputEventHelper, void, onDrag)(
	_In_ hsdk::i::frame::MOUSE_BUTTON _button,
	_In_ int _x,
	_In_ int _y)
{
	int dx = xy.x - _x;
	int dy = xy.y - _y;

	xy.x = _x;
	xy.y = _y;

	if (my_FocusComponent)
	{
		my_FocusComponent->onDrag(_button, dx, dy);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(inputEventHelper, void, onMove)(
	_In_ int _x,
	_In_ int _y)
{
	xy.x = xy.x + _x;
	xy.y = xy.y + _y;
	
	// focus �缳��.
	update();

	if (my_FocusComponent)
	{
		my_FocusComponent->onMove(_x, _y);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(inputEventHelper, void, onWheel)(
	_In_ int _x,
	_In_ int _y,
	_In_ int _w)
{
	xy.x = _x;
	xy.y = _y;

	if (my_FocusComponent)
	{
		my_FocusComponent->onWheel(_x, _y, _w);
	}
}