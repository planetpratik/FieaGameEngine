#pragma once
#include "pch.h"
#include "Factory.h"
namespace FieaGameEngine
{
	template <typename AbstractProductT>
	HashMap<std::string, Factory<AbstractProductT>*> Factory<AbstractProductT>::m_concrete_factories_table(10);

	template<typename AbstractProductT>
	inline Factory<AbstractProductT>* Factory<AbstractProductT>::find(const std::string& t_concrete_factory_name)
	{
		auto it = m_concrete_factories_table.find(t_concrete_factory_name);
		if (it == m_concrete_factories_table.end())
		{
			return nullptr;
		}
		return (*it).second;
	}

	template<typename AbstractProductT>
	inline gsl::owner<RTTI*> Factory<AbstractProductT>::create(const std::string& t_concrete_factory_name)
	{
		return m_concrete_factories_table.at(t_concrete_factory_name)->create();
	}

	template<typename AbstractProductT>
	inline uint32_t Factory<AbstractProductT>::getFactoryTableSize()
	{
		return m_concrete_factories_table.size();
	}

	template<typename AbstractProductT>
	inline void Factory<AbstractProductT>::add(Factory& t_concrete_factory)
	{
		m_concrete_factories_table.insert(std::make_pair(t_concrete_factory.className(), &t_concrete_factory));
	}

	template<typename AbstractProductT>
	inline void Factory<AbstractProductT>::remove(Factory& t_concrete_factory)
	{
		m_concrete_factories_table.remove(t_concrete_factory.className());
	}
}