#include "SList.h"

namespace FieaGameEngine
{
	template <typename T>
	inline SList<T>::SList() :
		m_front(nullptr), m_back(nullptr),
		m_size(0)
	{
	}

	template <typename T>
	SList<T>::SList(const SList& t_rhs) :
		m_front(nullptr), m_back(nullptr),
		m_size(0)
	{
		operator=(t_rhs);
	}

	template <typename T>
	SList<T>::SList(std::initializer_list<T> t_list)
	{
		for (const auto& value : t_list)
		{
			pushBack(value);
		}
	}

	template <typename T>
	SList<T>& SList<T>::operator=(const SList& t_rhs)
	{
		if (this != &t_rhs)
		{
			// Clear list associated to "this" before assigning passed list.
			clear();
			// Deep copy passed list into this list.
			for (const auto& value : t_rhs)
			{
				pushBack(value);
			}
		}
		return *this;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::pushFront(const T& t_item)
	{
		m_front = new Node(t_item, m_front);
		// If list is empty
		if (m_back ==  nullptr)
		{
			m_back = m_front;
		}
		++m_size;
		return Iterator(m_front, *this);
	}

	template<typename T>
	inline void SList<T>::popFront()
	{
		if (isEmpty())
		{
			throw std::exception("Error: Invalid Operation. List is Empty.");
		}
		else
		{
			Node* temp = m_front->m_next;
			delete m_front;
			m_front = temp;
			m_size--;
			if (m_size == 0)
			{
				m_front = nullptr;
				m_back = nullptr;
			}
		}
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::pushBack(const T & t_item)
	{
		Node* temp = new Node(t_item);
		// Check if list is empty.
		if (isEmpty())
		{
			m_front = temp;
		}
		// Else add item to the end of list and update back pointer.
		else
		{
			m_back->m_next = temp;
		}
		m_back = temp;
		m_size++;
		return Iterator(m_back, *this);
	}

	template<typename T>
	inline void SList<T>::popBack()
	{
		// Check if the pop operation is being performed on empty list. if yes, throw exception.
		if (isEmpty())
		{
			throw std::exception("Error: Invalid Operation. List is Empty.");
		}
		// Check if there is only one element in the list
		else if (m_front->m_next == nullptr)
		{
			Node* temp = m_front;
			delete temp;
			m_front = nullptr;
			m_back = nullptr;
			m_size--;
		}
		// Else traverse list till we get last and second last items in two nodes.
		else
		{
			Node* current_node = m_front;
			Node* next_node = m_front->m_next;
			while (next_node->m_next != nullptr)
			{
				next_node = next_node->m_next;
				current_node = current_node->m_next;
			}
			current_node->m_next = nullptr;
			// Make back pointer point to second last node and delete last node.
			m_back = current_node;
			//delete current_node;
			delete next_node;
			m_size--;
		}
	}

	template<typename T>
	inline void SList<T>::clear()
	{
		while (!isEmpty())
		{
			popFront();
		}
	}

	template <typename T>
	inline bool SList<T>::isEmpty() const
	{
		return (m_size == 0);
	}

	template <typename T>
	inline const T& SList<T>::front() const
	{
		if (m_front == nullptr)
		{
			throw std::exception("Error: Invalid Operation. List is Empty.");
		}
		return m_front->m_data;
	}

	template <typename T>
	inline T& SList<T>::front()
	{
		if (m_front == nullptr)
		{
			throw std::exception("Error: Invalid Operation. List is Empty.");
		}
		return m_front->m_data;
	}

	template <typename T>
	inline const T & SList<T>::back() const
	{
		if (m_back == nullptr)
		{
			throw std::exception("Error: Invalid Operation. List is Empty.");
		}
		return m_back->m_data;
	}

	template <typename T>
	inline T & SList<T>::back()
	{
		if (m_back == nullptr)
		{
			throw std::exception("Error: Invalid Operation. List is Empty.");
		}
		return m_back->m_data;
	}

	template <typename T>
	uint32_t SList<T>::size() const
	{
		return m_size;
	}

	template <typename T>
	inline SList<T>::~SList()
	{
		clear();
	}

	template <typename T>
	inline SList<T>::Node::Node(const T& t_data, Node* t_next_node) :
		m_data(t_data), m_next(t_next_node)
	{
	}

	template <typename T>
	SList<T>::Iterator::Iterator(Node* t_node, const SList& t_owner_List) :
		m_node(t_node), m_owner_list(&t_owner_List)
	{

	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::begin() const
	{
		return Iterator(m_front, *this);
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::end() const
	{
		return Iterator(nullptr, *this);
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::insertAfter(const T& t_value, const Iterator& t_it)
	{
		if (t_it.m_owner_list != this)
		{
			throw std::exception("Passed Iterator's owner is not this SList.");
		}
		if ((t_it == end()) || (m_back == t_it.m_node))
		{
			return pushBack(t_value);
		}
		else
		{
			Node* node = new Node(t_value, t_it.m_node->m_next);
			t_it.m_node->m_next = node;
			++m_size;
			return Iterator(node, *this);
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::find(const T& t_value) const
	{
		Iterator it = begin();
		for ( ; it != end(); ++it)
		{
			if (*it == t_value)
			{
				break;
			}
		}
		return it;
	}

	template <typename T>
	inline typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (m_owner_list == nullptr)
		{
			throw std::exception("Iterator doesn't belong to any SList");
		}
		if (m_node == nullptr)
		{
			throw std::exception("Invalid Operation! Iterator points to end of list.");
		}
		m_node = m_node->m_next;
		return *this;
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();
		return temp;
	}

	template <typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& t_rhs) const
	{
		return !(*this == t_rhs);
	}

	template <typename T>
	bool SList<T>::Iterator::operator==(const Iterator& t_rhs) const
	{
		return (m_owner_list == t_rhs.m_owner_list) && (m_node == t_rhs.m_node);
	}

	template <typename T>
	T& SList<T>::Iterator::operator*() const
	{
		if (m_owner_list == nullptr)
		{
			throw std::exception("Iterator doesn't belong to any SList");
		}
		if (m_node == nullptr)
		{
			throw std::exception("Invalid Operation! Iterator points to end of list.");
		}
		return m_node->m_data;
	}

	template <typename T>
	bool SList<T>::remove(const T& t_value)
	{
		return remove(find(t_value));
	}

	template <typename T>
	bool SList<T>::remove(const Iterator& t_it)
	{
		if (t_it.m_owner_list != this)
		{
			throw std::exception("Invalid Iterator ! Iterator is owned by different container.");
		}
		bool found = false;
		if (t_it != end())
		{
			if (t_it.m_node == m_back)
			{
				popBack();
			}
			else
			{
				Node* node = t_it.m_node->m_next;
				t_it.m_node->m_data = std::move(node->m_data);
				t_it.m_node->m_next = node->m_next;
				delete node;

				if (t_it.m_node->m_next == nullptr)
				{
					m_back = t_it.m_node;
				}
				--m_size;
			}
			found = true;
		}
		return found;
	}
}