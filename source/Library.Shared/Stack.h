#pragma once
#include "SList.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Templated Stack Implementation based on Singly Linked List.
	/// </summary>
	template <typename T>
	class Stack final
	{
	public:
		/// <summary>Default Constructor for Stack.</summary>
		Stack() = default;

		/// <summary>Destructor for Stack.</summary>
		~Stack();

		/// <summary>Copy Constructor for Stack.</summary>
		/// <param name="t_rhs">Const reference to passed Stack.</param>
		Stack(const Stack& t_rhs);

		/// <summary>Move Constructor for Stack.</summary>
		/// <param name="t_rhs">R-Value reference to Stack which is to be moved.</param>
		Stack(Stack&& t_rhs);

		/// <summary>Copy Assignment operator Implementation for Stack.</summary>
		/// <param name="t_rhs">Const reference to passed Stack.</param>
		/// <returns>Returns reference to a Stack.</returns>
		Stack& operator=(const Stack& t_rhs);

		/// <summary>Move Assignment operator for Stack.</summary>
		/// <param name="t_rhs">R-Value reference to Stack which is to be moved.</param>
		/// <returns>Returns reference to a Stack.</returns>
		Stack& operator=(Stack&& t_rhs);

		/// <summary>Push an element onto Stack. </summary>
		/// <param name="t_value">Const reference to passed element of type T.</param>
		void push(const T& t_value);

		/// <summary>Remove ( Pop ) an element from Stack.</summary>
		void pop();

		/// <summary>Get Top element of the Stack (Doesn't remove it).</summary>
		/// <returns>returns Reference to an Element of type T.</returns>
		T& top();

		/// <summary>Const version of top() - Get Top element of the Stack (Doesn't remove it).</summary>
		/// <returns>Returns Const reference to an Element of type T.</returns>
		const T& top() const;

		/// <summary>Gets Size of a Stack.</summary>
		/// <returns>Returns Size of a Stack as a std::size_t.</returns>
		std::size_t size() const;

		/// <summary>To Check whether Stack is empty or not.</summary>
		/// <returns>Returns Boolean result indicating whether Stack is empty or not.</returns>
		bool isEmpty() const;

		/// <summary>Clear method for Stack. Clears underlying SList. Called by Destructor.</summary>
		void clear();

	private:
		SList<T> m_list;
	};
}

#include "Stack.inl"
