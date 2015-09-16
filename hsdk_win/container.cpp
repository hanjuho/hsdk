#include <hsdk/win/frame/container.h>
#include <hsdk/interface/frame/inputeventhelper.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(Container, Container)(void)
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
CLASS_REALIZE_FUNC(Container, add_Component)(
	/* [in] */ i_Component * _component)
{
	if (contain_Component(_component))
	{
		return E_INVALIDARG;
	}
	else
	{
		// is - a ���� ����
		Component * component;
		component = (Component *)(_component);
		IF_FALSE(component)
		{
			return E_INVALIDARG;
		}

		// �θ���� �߰�
		component->my_Parent = this;

		// �����̳ʿ� �߰�
		m_Container.insert(std::hash_map<unsigned int, Component *>::value_type(
			_component->get_id(), component));

		return S_OK;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Container, remove_Component)(
	/* [in] */ i_Component * _component)
{
	if (contain_Component(_component))
	{
		// ���ԵǾ� �ִٴ� ���� is - a ���谡 Ȯ���ϴٴ� ���� ��Ÿ��
		Component * component = (Component *)(_component);

		// �θ���� ����
		component->my_Parent = nullptr;

		// �����̳ʿ��� ����
		m_Container.erase(component->get_id());
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Container, bool, contain_Component)(
	/* [in] */ i_Component * _component)const
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
CLASS_REALIZE_FUNC(Container, get_Component)(
	/* [out] */ i_Component * (&_component),
	/* [in] */ unsigned int _id)const
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
CLASS_REALIZE_FUNC_T(Container, bool, event_chain)(
	/* [in] */ i_inputEventHelper * _eventhelper)
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
CLASS_REALIZE_FUNC_T(Container, void, update)(
	/* [none] */ void)
{
	Component::update();
	std::hash_map<unsigned int, Component *>::iterator iter = m_Container.begin();
	std::hash_map<unsigned int, Component *>::iterator end = m_Container.end();
	while (iter != end)
	{
		iter->second->update();
		iter++;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Container, void, render)(
	/* [none] */ void)
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

		m_D3D11Graphics.render_Panel();
	}

}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Container, set_Layout)(
	/* [none] */ void)
{
	return E_ACCESSDENIED;
}