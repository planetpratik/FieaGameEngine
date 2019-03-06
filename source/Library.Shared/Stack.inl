#include "Stack.h"

namespace FieaGameEngine
{
	template<typename T>
	inline Stack<T>::Stack(const Stack& t_rhs) :
		m_list(t_rhs.m_list)
	{
	}

	template<typename T>
	inline Stack<T>::Stack(Stack&& t_rhs) :
		m_list(std::move(t_rhs.m_list))
	{
	}

	template<typename T>
	inline Stack<T>& Stack<T>::operator=(const Stack& t_rhs)
	{
		if (this != &t_rhs)
		{
			clear();
			m_list = t_rhs.m_list;
		}
		return *this;
	}

	template<typename T>
	inline Stack<T>& Stack<T>::operator=(Stack&& t_rhs)
	{
		if (this != &t_rhs)
		{
			clear();
			m_list = std::move(t_rhs.m_list);
		}
		return *this;
	}

	template <typename T>
	inline void Stack<T>::clear()
	{
		m_list.clear();
	}

	template <typename T>
	Stack<T>::~Stack()
	{
		clear();
	}

	template <typename T>
	inline void Stack<T>::push(const T& t_value)
	{
		m_list.pushFront(t_value);
	}

	template <typename T>
	inline void Stack<T>::pop()
	{
		m_list.popFront();
	}

	template <typename T>
	inline T& Stack<T>::top()
	{
		return m_list.front();
	}

	template <typename T>
	inline const T& Stack<T>::top() const
	{
		return m_list.front();
	}

	template <typename T>
	inline std::size_t Stack<T>::size() const
	{
		return m_list.size();
	}

	template <typename T>
	inline bool Stack<T>::isEmpty() const
	{
		return m_list.isEmpty();
	}
}