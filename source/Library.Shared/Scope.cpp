#include "pch.h"
#include "Scope.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Scope);

	Scope::Scope(const uint32_t& t_default_size) :
		m_parent(nullptr), m_lookup_table(), m_pointers_list(t_default_size)
	{
	}

	Scope::Scope(const Scope& t_rhs) :
		m_parent(nullptr), m_lookup_table(), m_pointers_list(t_rhs.m_pointers_list.size())
	{
		if (this != &t_rhs)
		{
			/*operator=(t_rhs);*/
			doRecursiveChildrenCopy(t_rhs);
		}
	}

	Scope::Scope(Scope&& t_rhs) :
		m_parent(t_rhs.m_parent), m_lookup_table(std::move(t_rhs.m_lookup_table)), m_pointers_list(std::move(t_rhs.m_pointers_list))
	{
		/*if (this != &t_rhs)
		{
			fixParentPointer(t_rhs);
			t_rhs.m_parent = nullptr;
		}*/
	}

	Scope& Scope::operator=(const Scope& t_rhs)
	{
		if (this != &t_rhs)
		{
			clear();
			m_lookup_table.rehash(DEFAULT_SIZE);
			m_pointers_list.reserve(t_rhs.m_lookup_table.size());
			doRecursiveChildrenCopy(t_rhs);
		}
		return *this;
	}

	/*(Scope& Scope::operator=(Scope&& t_rhs)
	{
		if (this != &t_rhs)
		{
			clear();
			m_parent = t_rhs.m_parent;
			m_lookup_table = std::move(t_rhs.m_lookup_table);
			m_pointers_list = std::move(t_rhs.m_pointers_list);
			fixParentPointer(t_rhs);
			t_rhs.m_parent = nullptr;
		}
		return *this;
	}*/

	void Scope::clear()
	{
		for (uint32_t i = 0; i < m_pointers_list.size(); ++i)
		{
			Datum& datum = m_pointers_list[i]->second;
			if (datum.type() == Datum::DatumType::TABLE)
			{
				for (uint32_t j = 0; j < datum.size(); ++j)
				{
					delete datum.get<Scope*>(j);
				}
			}
		}
		/*m_lookup_table.clear();*/
		m_pointers_list.clear();
	}

	Scope::~Scope()
	{
		clear();
	}

	Datum* Scope::find(const std::string& t_name)
	{
		if (t_name.empty())
		{
			return nullptr;
		}
		auto it = m_lookup_table.find(t_name);
		if (it == m_lookup_table.end())
		{
			return nullptr;
		}
		return &(it->second);
	}

	const Datum* Scope::find(const std::string& t_name) const
	{
		if (t_name.empty())
		{
			return nullptr;
		}
		auto it = m_lookup_table.find(t_name);
		if (it == m_lookup_table.end())
		{
			return nullptr;
		}
		return &(it->second);
	}

	Datum* Scope::search(const std::string& t_name, Scope** t_owner)
	{
		Datum* datum = find(t_name);
		if (datum != nullptr)
		{
			if (t_owner != nullptr)
			{
				*t_owner = this;
			}
			return datum;
		}
		if (m_parent == nullptr)
		{
			if (t_owner != nullptr)
			{
				*t_owner = nullptr;
			}
			return nullptr;
		}
		return m_parent->search(t_name, t_owner);
	}

	Datum& Scope::append(const std::string& t_name)
	{
		if (t_name.empty())
		{
			throw std::exception("Invalid Operation! String must not be empty.");
		}
		bool isItemNotExists = false;
		auto it = m_lookup_table.insert(std::make_pair(t_name, Datum()), isItemNotExists);

		if (isItemNotExists)
		{
			m_pointers_list.pushBack(&*it);
		}
		return it->second;
	}

	Scope& Scope::appendScope(const std::string& t_name)
	{
		Datum& datum = append(t_name);
		datum.setType(Datum::DatumType::TABLE);
		Scope* scope = new Scope();
		scope->m_parent = this;
		datum.pushBack(scope);
		return *scope;
	}

	void Scope::adopt(const std::string& t_name, Scope& t_child)
	{
		Datum* temp = find(t_name);
		if (temp == nullptr)
		{
			throw std::exception("Invalid operation! Datum for given name doesn't exist in scope.");
		}
		if (this == &t_child)
		{
			throw std::exception("Invalid operation! Child cannot adopt itself.");
		}
		Datum& datum = append(t_name);
		datum.setType(Datum::DatumType::TABLE);
		t_child.makeChildOrphan();
		t_child.m_parent = this;
		datum.pushBack(&t_child);
	}

	Scope* Scope::getParent() const
	{
		return m_parent;
	}

	Datum& Scope::operator[](const std::string& t_name)
	{
		return append(t_name);
	}

	const Datum& Scope::operator[](const std::string& t_name) const
	{
		return at(t_name);
	}

	Datum& Scope::operator[](const uint32_t& t_index)
	{
		if (t_index >= m_lookup_table.size())
		{
			throw std::exception("Invalid Operation! Index out of bounds.");
		}
		return m_pointers_list[t_index]->second;
	}

	const Datum& Scope::operator[](const uint32_t & t_index) const
	{
		if (t_index >= m_lookup_table.size())
		{
			throw std::exception("Invalid Operation! Index out of bounds.");
		}
		return m_pointers_list[t_index]->second;
	}

	Datum& Scope::at(const std::string& t_name)
	{
		Datum* datum = find(t_name);
		if (datum == nullptr)
		{
			throw std::exception("Invalid Operation! No entry found.");
		}
		return *datum;
	}

	const Datum& Scope::at(const std::string& t_name) const
	{
		const Datum* datum = find(t_name);
		if (datum == nullptr)
		{
			throw std::exception("Invalid Operation! No entry found.");
		}
		return *datum;
	}

	bool Scope::operator==(const Scope& t_rhs)
	{
		if (m_pointers_list.size() == t_rhs.m_pointers_list.size())
		{
			auto i = m_pointers_list.begin();
			auto j = t_rhs.m_pointers_list.begin();
			for (; i != m_pointers_list.end(); ++i, ++j)
			{
				if (*(*i) != *(*j))
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}

	bool Scope::operator!=(const Scope& t_rhs)
	{
		return !(operator==(t_rhs));
	}

	std::string Scope::findName(const Scope& t_child) const
	{
		std::string name;
		for (uint32_t i = 0; i < m_pointers_list.size(); ++i)
		{
			Datum child;
			child = m_pointers_list[i]->second;
			if (child.type() == Datum::DatumType::TABLE)
			{
				for (uint32_t j = 0; j < child.size(); ++j)
				{
					if (&child[j] == &t_child)
					{
						name = m_pointers_list[i]->first;
						break;
					}
				}
			}
		}
		return name;
	}

	void Scope::doRecursiveChildrenCopy(const Scope& t_rhs)
	{
		for (auto& it : t_rhs.m_pointers_list)
		{
			std::string& name = it->first;
			Datum& datum = it->second;
			if (datum.type() != Datum::DatumType::TABLE)
			{
				append(name) = datum;
			}
			else
			{
				for (uint32_t i = 0; i < datum.size(); ++i)
				{
					appendScope(name) = datum[i];
				}
			}
		}
	}


	void Scope::fixParentPointer(Scope& /*t_rhs*/)
	{
		/*if (m_parent != nullptr)
		{
			std::string name = t_rhs.m_parent->findName(t_rhs);
			t_rhs.makeChildOrphan();
			m_parent->adopt(name, *this);
		}
		for (auto& it : m_pointers_list)
		{
			if (it->second.type() == Datum::DatumType::TABLE)
			{
				for (uint32_t i = 0; i < it->second.size(); ++i)
				{
					it->second[i].m_parent = this;
				}
			}
		}*/
	}

	void Scope::makeChildOrphan()
	{
		if (m_parent != nullptr)
		{
			for (auto& it : m_parent->m_pointers_list)
			{
				if (it->second.type() == Datum::DatumType::TABLE)
				{
					if (it->second.remove(*this))
					{
						m_parent = nullptr;
						break;
					}
				}
			}
		}
	}


	bool Scope::Equals(const RTTI* t_rhs)
	{
		if (t_rhs == nullptr)
		{
			return false;
		}
		Scope* scope = t_rhs->As<Scope>();
		if (scope != nullptr)
		{
			return *scope == *this;
		}
		return false;
	}

	std::string Scope::ToString() const
	{
		return "Scope";
	}
}