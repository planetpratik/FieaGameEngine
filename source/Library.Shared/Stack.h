#pragma once
#include "SList.h"

namespace FieaGameEngine
{
	template <typename T>
	class Stack final
	{
	public:
		Stack() = default;
		~Stack();
		Stack(const Stack& t_rhs);
		Stack(Stack&& t_rhs);
		Stack& operator=(const Stack& t_rhs);
		Stack& operator=(Stack&& t_rhs);
		void push(const T& t_value);
		void pop();
		T& top();
		const T& top() const;

		std::size_t size() const;
		bool isEmpty() const;
		void clear();

	private:
		SList<T> m_list;
	};
}

#include "Stack.inl"
