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
	SList<T>::SList(const SList<T>& t_rhs) :
		m_front(nullptr), m_back(nullptr),
		m_size(0)
	{
		operator=(t_rhs);
	}

	template <typename T>
	SList<T>& SList<T>::operator=(const SList<T>& t_rhs)
	{
		if (this != &t_rhs)
		{
			// Clear list associated to "this" before assigning passed list.
			clear();
			// Deep copy passed list into this list.
			SList<T>::Iterator it = t_rhs.begin();
			for (it; it != t_rhs.end(); ++it)
			{
				pushBack(*it);
			}
		}
		return *this;
	}

	template<typename T>
	inline void SList<T>::pushFront(const T & t_item)
	{
		m_front = new Node(t_item, m_front);
		// If list is empty
		if (isEmpty())
		{
			m_back = m_front;
		}
		m_size++;
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
			Node* temp = m_front->next;
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
	inline void SList<T>::pushBack(const T & t_item)
	{
		Node* temp = new Node(t_item);
		// Check if list is empty.
		if (isEmpty())
		{
			m_back = temp;
			m_front = m_back;
		}
		// Else add item to the end of list and update back pointer.
		else
		{
			m_back->next = temp;
			m_back = temp;
		}
		m_size++;
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
		else if (m_front->next == nullptr)
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
			Node* next_node = m_front->next;
			while (next_node->next != nullptr)
			{
				next_node = next_node->next;
				current_node = current_node->next;
			}
			current_node->next = nullptr;
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
		m_data(t_data), next(t_next_node)
	{
	}

	template <typename T>
	SList<T>::Iterator::Iterator(Node* t_node, const SList<T>* t_owner_List) :
		m_node(t_node), m_owner_list(t_owner_List)
	{

	}

	template <typename T>
	SList<T>::Iterator::Iterator(const Iterator& t_rhs) :
		m_node(t_rhs.m_node), m_owner_list(t_rhs.m_owner_list)
	{

	}

	template <typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator=(const Iterator& t_rhs)
	{
		if (this != &t_rhs)
		{
			m_node = t_rhs.m_node;
			m_owner_list = t_rhs.m_owner_list;
		}
		return *this;
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::begin() const
	{
		return Iterator(m_front, this);
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::end() const
	{
		return Iterator(nullptr, this);
	}

	template <typename T>
	typename void SList<T>::insertAfter(const T& t_value, const Iterator& t_it)
	{
		if (t_it.m_owner_list != this)
		{
			throw std::exception("Passed Iterator's owner is not this SList.");
		}
		if (t_it.m_node == nullptr)
		{
			pushBack(t_value);
		}
		else
		{
			Node* node_to_insert = new Node(t_value);
			node_to_insert->next = t_it.m_node->next;
			t_it.m_node->next = node_to_insert;
			m_size++;
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::find(const T& t_value) const
	{
		SList<T>::Iterator it = begin();
		for (it ; it != end(); ++it)
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
		m_node = m_node->next;
		return *this;
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		return (*this).operator++();
	}

	template <typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& t_rhs) const
	{
		return !(*this == t_rhs);
	}

	template <typename T>
	bool SList<T>::Iterator::operator==(const Iterator& t_rhs) const
	{
		if (m_owner_list == nullptr || t_rhs.m_owner_list == nullptr)
		{
			return false;
		}
		else if (m_node != t_rhs.m_node)
		{
			return false;
		}
		else
		{
			return true;
		}
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
	void SList<T>::remove(const T& t_value)
	{
		SList<T>::Iterator it = begin();
		if (!isEmpty())
		{
			// If there is only one node in the list do pop_front()
			if (*it == t_value)
			{
				popFront();
			}
			else
			{
				++it;
				for (SList<T>::Iterator previous_it = begin(); it != end(); ++it, ++previous_it)
				{
					if (*it == t_value)
					{
						(previous_it.m_node)->next = (it.m_node)->next;
						if (it.m_node == m_back)
						{
							m_back = previous_it.m_node;
						}
						m_size--;
						delete it.m_node;
						break;
					}
				}
			}
		}
	}
}