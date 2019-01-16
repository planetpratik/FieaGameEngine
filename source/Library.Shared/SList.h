#pragma once

namespace FieaGameEngine
{
	/// <summary>Template Class for Singly-linked List.</summary>
	template <typename T>
	class SList
	{
	public:
		/// <summary>Default Constructor for SList.</summary>
		SList();

		/// <summary>Copy Constructor for SList.</summary>
		/// <param name="t_rhs">Const reference to passed List of Type T.</param>
		SList(const SList<T> &t_rhs);

		/// <summary>Assignment Operator Overload.</summary>
		/// <param name="t_rhs">Const reference to passed List of Type T.</param>
		/// <returns>Returns Mutable Reference to "This" list.</returns>
		SList<T>& operator= (const SList<T> &t_rhs);

		/// <summary>Destructor for SList.</summary>
		~SList();

		/// <summary>Pushes element on front of the List. </summary>
		/// <param name="t_item">Const reference to passed element of Type T.</param>
		void pushFront(const T& t_item);
		
		/// <summary>Removes foremost element from the List.</summary>
		void popFront();

		/// <summary>Pushes element on the back of the List.</summary>
		/// <param name="t_item">Const reference to passed element of Type T.</param>
		void pushBack(const T& t_item);

		/// <summary>Removes Last element from the list</summary>
		void popBack();

		/// <summary>Conveys whether List is empty or not</summary>
		/// <returns>Returns True if List is empty else returns false.</returns>
		bool isEmpty() const;

		/// <summary>Gets First element in the list. Doesn't remove element from list.</summary>
		/// <returns>Returns Const reference to First element of Type T from the List.</returns>
		const T& front() const;

		/// <summary>Gets Last element in the list. Doesn't remove element from list.</summary>
		/// <returns>Returns Const reference to Last element of Type T from the List.</returns>
		const T& back() const;

		/// <summary>Gets Size ( No. of elements ) of given List.</summary>
		/// <returns>Returns size of list in Unsigned Int ( 32 bits ).</returns>
		uint32_t size() const;

		/// <summary>Removes all elements from the list</summary>
		void clear();
	private:
		/// <summary>Auxiliary Helper class "Node" is used to represents individual elements of List</summary>
		class Node
		{
		public:
			Node(const T& t_data);
			const T& getData() const;
			Node* next;
		private:
			T m_data;
		};
		Node* m_front;
		Node* m_back;
		uint32_t m_size;
	};
}
/// <summary>Contains SList template members definitions.</summary>
#include "SList.inl"

