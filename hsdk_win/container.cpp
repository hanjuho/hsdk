#include <hsdk/win/frame/container.h>
#include <hsdk/interface/frame/inputeventhelper.h>



using namespace hsdk::frame;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Container, Container)(
	_In_ float _x,
	_In_ float _y,
	_In_ float _w,
	_In_ float _h)
	: Component(_x, _y, _w, _h)
{

}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(Container, Container)(void)
{
	auto begin = m_Container.begin();
	auto end = m_Container.end();
	while (begin != end)
	{
		DEL_POINTER(*begin);
		++begin;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Container, void, set_Layout)(
	_In_ hsdk::i::frame::i_Layout * _layout)
{
	m_Layout = _layout;
	if (m_Layout)
	{
		m_Layout->resize(get_W(), get_H());
		reform();
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Container, hsdk::i::frame::i_Layout *, get_Layout)(
	_X_ void)
{
	return m_Layout;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Container, add_Component)(
	_In_ i_Component * _component,
	_In_ hsdk::i::frame::LAYOUT_COMPOSITION _composition)
{
	// 부모가 있다면 아래 구문은 치명적임
	if (_component->parent())
	{
		return E_INVALIDARG;
	}

	// is - a 관계 증명
	Component * component;
	component = (Component *)(_component);
	IF_FALSE(component)
	{
		return E_INVALIDARG;
	}

	if (m_Layout)
	{
		m_Layout->get_Form(
			component->my_Rectangle,
			_composition);
	}

	// 부모관계 추가
	component->my_Parent = this;
	component->m_Composition = _composition;

	// 컨테이너에 추가
	m_Container.push_back(component);

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Container, remove_Component)(
	_In_ i_Component * _component)
{
	// 부모가 없다면 아래 구문은 의미가 없음
	IF_INVALID(_component->parent())
	{
		return E_INVALIDARG;
	}

	auto iter = std::find(m_Container.begin(), m_Container.end(), _component);
	if (iter == m_Container.end())
	{
		auto begin = m_Container.begin();
		auto end = m_Container.end();
		while (begin != end)
		{
			IF_SUCCEEDED((*begin)->remove_Component(_component))
			{
				return S_OK;
			}

			++begin;
		}

		return E_INVALIDARG;
	}
	else
	{
		// 포함되어 있다는 것은 is - a 관계가 확실하다는 것을 나타냄
		Component * component = (Component *)(_component);

		// 부모관계 제거
		component->my_Parent = nullptr;

		// 컨테이너에서 제거
		m_Container.erase(iter);
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Container, contain_Component)(
	_In_ i_Component * _component)const
{
	// 부모가 없다면 아래 구문은 의미가 없음
	IF_INVALID(_component->parent())
	{
		return E_INVALIDARG;
	}

	auto iter = std::find(m_Container.begin(), m_Container.end(), _component);
	if (iter == m_Container.end())
	{
		auto begin = m_Container.begin();
		auto end = m_Container.end();
		while (begin != end)
		{
			IF_SUCCEEDED((*begin)->contain_Component(_component))
			{
				return S_FALSE;
			}

			++begin;
		}
	}
	else
	{
		return S_OK;
	}

	return E_INVALIDARG;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Container, get_Component)(
	_Out_ i_Component * (&_component),
	_In_ unsigned int _id)const
{
	// 부모가 없다면 아래 구문은 의미가 없음
	IF_INVALID(_component->parent())
	{
		return E_INVALIDARG;
	}

	auto begin = m_Container.begin();
	auto end = m_Container.end();
	while (begin != end)
	{
		if ((*begin)->my_id == _id)
		{
			_component = (*begin);
			return S_OK;
		}
		else
		{
			IF_SUCCEEDED((*begin)->get_Component(_component, _id))
			{
				return S_OK;
			}
		}

		++begin;
	}

	return E_INVALIDARG;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Container, bool, event_chain)(
	_In_ hsdk::i::frame::i_inputEventHelper * _eventhelper)
{
	if (_eventhelper->chain(this))
	{
		auto begin = m_Container.begin();
		auto end = m_Container.end();
		while (begin != end)
		{
			if ((*begin)->event_chain(_eventhelper))
			{
				break;
			}

			++begin;
		}

		return true;
	}
	else
	{
		return false;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Container, void, update)(
	_X_ void)
{
	auto begin = m_Container.begin();
	auto end = m_Container.end();
	while (begin != end)
	{
		(*begin)->update();
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Container, void, reform)(
	_X_ void)
{
	Component::reform();

	if (m_Layout)
	{
		m_Layout->resize(get_W(), get_H());
	}

	auto begin = m_Container.begin();
	auto end = m_Container.end();
	while (begin != end)
	{
		Component * compo = (*begin);
		if (m_Layout)
		{
			m_Layout->get_Form(
				compo->my_Rectangle,
				compo->m_Composition);
		}

		compo->reform();
		++begin;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Container, void, render)(
	_X_ void)
{
	if (is_Visible())
	{
		Component::render();

		auto begin = m_Container.begin();
		auto end = m_Container.end();
		while (begin != end)
		{
			(*begin)->render();
			++begin;
		}
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Container, void, reset)(
	_X_ void)
{
	clear();
	m_Layout.~AutoDelete();
	m_Graphics = Graphics();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Container, void, clear)(
	_X_ void)
{
	m_Container.clear();
}
