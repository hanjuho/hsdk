#include <hsdk/win/frame/container.h>
#include <hsdk/interface/frame/inputeventhelper.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;


//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(Container, Container)(void)
{
	std::hash_map<unsigned int, Component *>::iterator iter = m_Container.begin();
	std::hash_map<unsigned int, Component *>::iterator end = m_Container.end();
	while (iter != end)
	{
		DEL_POINTER(iter->second);
		iter++;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Container, add_Component)(
	/* [r] */ i_Component * _component)
{
	if (contain_Component(_component))
	{
		return E_INVALIDARG;
	}
	else
	{
		// is - a 관계 증명
		Component * component;
		component = (Component *)(_component);
		IF_FALSE(component)
		{
			return E_INVALIDARG;
		}

		// 부모관계 추가
		component->my_Parent = this;

		// 컨테이너에 추가
		m_Container.insert(std::hash_map<unsigned int, Component *>::value_type(
			_component->get_id(), component));

		return S_OK;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Container, remove_Component)(
	/* [r] */ i_Component * _component)
{
	if (contain_Component(_component))
	{
		// 포함되어 있다는 것은 is - a 관계가 확실하다는 것을 나타냄
		Component * component = (Component *)(_component);

		// 부모관계 제거
		component->my_Parent = nullptr;

		// 컨테이너에서 제거
		m_Container.erase(component->get_id());
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Container, bool, contain_Component)(
	/* [r] */ i_Component * _component)const
{
	std::hash_map<unsigned int, Component *>::const_iterator iter =
		m_Container.find(_component->get_id());

	if (iter == m_Container.end())
	{
		return false;
	}
	else
	{
		return iter->second == _component;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Container, get_Component)(
	/* [w] */ i_Component * (&_component),
	/* [r] */ unsigned int _id)const
{
	std::hash_map<unsigned int, Component *>::const_iterator iter =
		m_Container.find(_id);

	if (iter == m_Container.end())
	{
		return E_INVALIDARG;
	}
	else
	{
		_component = iter->second;
		return S_OK;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Container, bool, event_chain)(
	/* [r] */ i_inputEventHelper * _eventhelper)
{
	if (_eventhelper->chain(this))
	{
		std::hash_map<unsigned int, Component *>::iterator iter = m_Container.begin();
		std::hash_map<unsigned int, Component *>::iterator end = m_Container.end();
		while (iter != end)
		{
			Component * cmp = iter->second;
			if (cmp->event_chain(_eventhelper))
			{
				break;
			}

			iter++;
		}

		return true;
	}
	else
	{
		return false;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Container, void, reform)(
	/* [x] */ void)
{
	Component::reform();
	std::hash_map<unsigned int, Component *>::iterator iter = m_Container.begin();
	std::hash_map<unsigned int, Component *>::iterator end = m_Container.end();
	while (iter != end)
	{
		iter->second->reform();
		iter++;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Container, void, render)(
	/* [x] */ void)
{
	if (is_Visible())
	{
		std::hash_map<unsigned int, Component *>::iterator iter = m_Container.begin();
		std::hash_map<unsigned int, Component *>::iterator end = m_Container.end();
		while (iter != end)
		{
			iter->second->render();
			iter++;
		}

		m_D3D10Graphics.render(1.0f);
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Container, void, clear)(
	/* [x] */ void)
{
	m_Container.clear();
}
