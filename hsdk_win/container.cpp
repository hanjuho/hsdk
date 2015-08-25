#include <hsdk/win/frame/container.h>
#include <hsdk/interface/frame/inputeventhelper.h>



using namespace hsdk;
using namespace i::frame;
using namespace win::frame;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(Container, Container)(void)
{
	std::hash_map<unsigned int, i_Component *>::iterator iter = m_Container.begin();
	std::hash_map<unsigned int, i_Component *>::iterator end = m_Container.end();
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
		m_Container.insert(std::hash_map<unsigned int, i_Component *>::value_type(
			_component->get_id(), _component));

		return S_OK;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Container, remove_Component)(
	/* [out] */ i_Component * _component)
{
	m_Container.erase(_component->get_id());

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC_T(Container, bool, contain_Component)(
	/* [out] */ i_Component * _component)
{
	return m_Container.find(_component->get_id()) != m_Container.end();
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Container, get_Component)(
	/* [in] */ unsigned int _id,
	/* [out] */ i_Component * (&_component))
{
	std::hash_map<unsigned int, i_Component *>::iterator iter =
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
		std::hash_map<unsigned int, i_Component *>::iterator iter = m_Container.begin();
		std::hash_map<unsigned int, i_Component *>::iterator end = m_Container.end();
		while (iter != end)
		{
			i_Component * cmp = iter->second;
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
	std::hash_map<unsigned int, i_Component *>::iterator iter = m_Container.begin();
	std::hash_map<unsigned int, i_Component *>::iterator end = m_Container.end();
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
	m_d3d11Graphics.render();
	std::hash_map<unsigned int, i_Component *>::iterator iter = m_Container.begin();
	std::hash_map<unsigned int, i_Component *>::iterator end = m_Container.end();
	while (iter != end)
	{
		iter->second->render();
		iter++;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Container, set_Layout)(
	/* [none] */ void)
{
	return E_ACCESSDENIED;
}
