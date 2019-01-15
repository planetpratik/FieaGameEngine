#include "pch.h"
#include "SList.h"

namespace FieaGameEngine
{
	// Default constructor implementation
	template <typename T>
	SList<T>::SList() :
		m_front(nullptr), m_back(nullptr),
		m_size(0)
	{
	}

	// Copy Constructor Implementation - Create Deep copy of list given as argument
	template <typename T>
	SList<T>::SList(const SList<T>& t_rhs) :
		m_front(nullptr), m_back(nullptr),
		m_size(0)
	{
		// Check if passed list is not empty
		if (t_rhs.isEmpty() != true)
		{
			// Get first item in list
			Node* current_node = t_rhs.m_front;
			// Loop till we reach end of the list
			while (current_node != nullptr)
			{
				// Create new node and copy data from current node into it.
				Node* new_node = new Node(current_node->getData());
				// If first element, assign m_front and m_back to point to it.
				if (m_size == 0)
				{
					m_front = new_node;
					m_back = new_node;
				}
				else
				{
					// Update next pointer of last item to point to new node.
					m_back->next = new_node;
					// Update m_back node to point to last item in list.
					m_back = m_back->next;
				}
				// Update current node to next node
				current_node = current_node->next;
				// Increment list size counter
				m_size++;
			}
		}
	}

	// Assignment Operator Overload Implementation
	template <typename T>
	SList<T>& SList<T>::operator=(const SList<T>& t_rhs)
	{
		// If "this" object and passed object aren't same, then only proceed else skip.
		if (this != &t_rhs)
		{
			// Clear list associated to "this" before assigning passed list.
			clear();
			// Deep copy passed list into this list.
			if (t_rhs.isEmpty() != true)
			{
				Node* current_node = t_rhs.m_front;
				while (current_node != nullptr)
				{
					Node* new_node = new Node(current_node->getData());
					if (m_size == 0)
					{
						m_front = new_node;
						m_back = new_node;
					}
					else
					{
						m_back->next = new_node;
						m_back = m_back->next;
					}
					current_node = current_node->next;
					m_size++;
				}
			}
		}
		return *this;
	}

	// Implementation of pushFront() method.
	template<typename T>
	void SList<T>::pushFront(const T & t_item)
	{
		// Check if list is empty.
		if (m_front == nullptr)
		{
			m_front = new Node(t_item);
			m_back = m_front;
		}
		// Else add item to the front of list and update front pointer.
		else
		{
			Node* temp = new Node(t_item);
			temp->next = m_front;
			m_front = temp;
		}
		m_size++;
	}

	// Implementation of popFront() method
	template<typename T>
	void SList<T>::popFront()
	{
		// Check if the pop operation is being performed on empty list. if yes, throw exception.
		if (isEmpty())
		{
			throw std::exception("Error: Invalid Operation. List is Empty.");
		}
		// Else remove first element of list and make m_front point to second element in the list.
		else
		{
			Node* temp = m_front->next;
			delete m_front;
			m_front = temp;
			m_size--;
			// Check if list is empty, if yes, set front and back pointers to null pointers.
			if (m_size == 0)
			{
				m_front = nullptr;
				m_back = nullptr;
			}
		}
	}

	// Implementation of pushBack() method
	template<typename T>
	void SList<T>::pushBack(const T & t_item)
	{
		Node* temp = new Node(t_item);
		// Check if list is empty.
		if (m_back == nullptr)
		{
			m_back = temp;
			m_front = m_back;
		}
		// Else add item to the end of list and update back pointer.
		else
		{
			m_back->next = temp;
			m_back = m_back->next;
		}
		m_size++;
	}

	// Implementation of popBack() method
	template<typename T>
	void SList<T>::popBack()
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

	// Implementation of clear() method
	template<typename T>
	void SList<T>::clear()
	{
		while (!isEmpty())
		{
			popFront();
		}
		m_front = nullptr;
		m_back = nullptr;
	}

	// isEmpty() implementation.
	template <typename T>
	bool SList<T>::isEmpty() const
	{
		return (m_size == 0) ? true : false;
	}

	// front() implementation
	template <typename T>
	const T & SList<T>::front() const
	{
		if (m_front == nullptr)
		{
			throw std::exception("Error: Invalid Operation. List is Empty.");
		}
		return m_front->getData();
	}

	// back() implementation
	template <typename T>
	const T & SList<T>::back() const
	{
		if (m_back == nullptr)
		{
			throw std::exception("Error: Invalid Operation. List is Empty.");
		}
		return m_back->getData();
	}

	// size() implementation
	template <typename T>
	uint32_t SList<T>::size() const
	{
		return m_size;
	}

	// Destructor implementation
	template <typename T>
	SList<T>::~SList()
	{
		clear();
	}

	// Node constructor
	template <typename T>
	SList<T>::Node::Node(const T & t_data) :
		next(nullptr),
		m_data(t_data)
	{
	}

	// Node getData() implementation
	template <typename T>
	const T & SList<T>::Node::getData() const
	{
		return m_data;
	}
}