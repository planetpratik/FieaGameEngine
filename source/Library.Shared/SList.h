#pragma once
#include <cstdint>
#include <cstring>
#include <exception>
#include <initializer_list>

namespace FieaGameEngine
{
	/// <summary>Template Class for Singly-linked List.</summary>
	template <typename T>
	class SList
	{
	private:
		/// <summary>Auxiliary Helper class "Node" is used to represents individual elements of List</summary>
		struct Node final
		{
			explicit Node(const T& t_data, Node* t_next_node = nullptr);
			Node(const Node& t_rhs) = delete;
			Node& operator=(const Node& t_rhs) = delete;
			T m_data;
			Node* m_next;
			~Node() = default;
		};

	public:
		/// <summary>Embedded Iterator class for SList to iterate over it's items.</summary>
		class Iterator final
		{
			friend SList;

		public:
			/// <summary>Default Constructor for Iterator.</summary>
			Iterator() = default;
	
			/// <summary>Copy Constructor for Iterator.</summary>
			/// <param name="t_rhs">Const reference to passed Iterator</param>
			Iterator(const Iterator&) = default;

			/// <summary>Assignment Operator Overload</summary>
			/// <param name="t_rhs">Const reference to passed Iterator</param>
			/// <returns>Reference to Iterator</returns>
			Iterator& operator=(const Iterator& t_rhs) = default;

			/// <summary>Compare two iterators based on their owner container and current node.</summary>
			/// <param name="t_rhs">Const reference to passed Iterator.</param>
			/// <returns>Boolean value indicating whether Iterators are equal or not.</returns>
			bool operator==(const Iterator& t_rhs) const;

			/// <summary>Not Equal Operator overload for two iterators.</summary>
			/// <param name="t_rhs">Const reference to passed Iterator.</param>
			/// <returns>Boolean value indicating whether Iterators aren't equal or not.</returns>
			bool operator!=(const Iterator& t_rhs) const;

			/// <summary>Prefix Increment Operator Overload. Moves iterator to next item in the list.</summary>
			/// <returns>Reference to an Iterator.</returns>
			Iterator& operator++();

			/// <summary>Postfix Increment Operator Overload. Moves iterator to next item in the list.</summary>
			/// <param name="int">Just to differentiate between Prefix and Postfix Increment Operator Overload</param>
			/// <returns>Returns copy of current value of Iterator</returns>
			Iterator operator++(int);

			/// <summary>Dereference Operator overloading.</summary>
			/// <returns>Element stored in the list at current position of the iterator</returns>
			T& operator*() const;

			/// <summary>Destructor for Iterator</summary>
			~Iterator() = default;

		private:
			/// <summary>Constructor for Iterator</summary>
			/// <param name="t_node">Current node which this iterator will point to</param>
			/// <param name="t_owner_List">Const reference to Owner Container List for this iterator</param>
			Iterator(Node* t_node, const SList& t_owner_List);
			Node* m_node = nullptr;
			const SList<T>* m_owner_list = nullptr;
		};

		/// <summary>Default Constructor for SList.</summary>
		SList();

		/// <summary>Support for Initializer list syntax ( C++11 ).</summary>
		/// <param name="t_list"> Initializer arguments list of Type T.</param>
		SList(std::initializer_list<T> t_list);

		/// <summary>Copy Constructor for SList.</summary>
		/// <param name="t_rhs">Const reference to passed List of Type T.</param>
		SList(const SList &t_rhs);

		/// <summary>Assignment Operator Overload.</summary>
		/// <param name="t_rhs">Const reference to passed List of Type T.</param>
		/// <returns>Returns Mutable Reference to "This" list.</returns>
		SList<T>& operator= (const SList &t_rhs);

		/// <summary>Destructor for SList.</summary>
		~SList();

		/// <summary>Pushes element on front of the List. </summary>
		/// <param name="t_item">Const reference to passed element of Type T.</param>
		//void pushFront(const T& t_item);

		/// <summary>Pushes element on front of the List. </summary>
		/// <param name="t_item">Const reference to passed element of Type T.</param>
		/// <returns>Returns Iterator to front element in List</returns>
		Iterator pushFront(const T& t_item);
		
		/// <summary>Removes foremost element from the List.</summary>
		void popFront();

		/// <summary>Pushes element on the back of the List.</summary>
		/// <param name="t_item">Const reference to passed element of Type T.</param>
		//void pushBack(const T& t_item);

		/// <summary>Pushes element on the back of the List.</summary>
		/// <param name="t_item">Const reference to passed element of Type T.</param>
		/// <returns>Return Iterator to last element in the List.</returns>
		Iterator pushBack(const T& t_item);

		/// <summary>Removes Last element from the list</summary>
		void popBack();

		/// <summary>Conveys whether List is empty or not</summary>
		/// <returns>Returns True if List is empty else returns false.</returns>
		bool isEmpty() const;

		/// <summary>Gets First element of the List. Doesn't remove element from list.</summary>
		/// <returns>Returns Const reference to First element of Type T from the List.</returns>
		const T& front() const;

		/// <summary>Non Const Version of front() method. Gets First element in the list. Doesn't remove element from list.</summary>
		/// <returns>Returns Non-Const reference to First element of Type T from the List.</returns>
		T& front();

		/// <summary>Gets Last element in the list. Doesn't remove element from list.</summary>
		/// <returns>Returns Const reference to Last element of Type T from the List.</returns>
		const T& back() const;

		/// <summary>Non Const Version of back() method. Gets Last element in the list. Doesn't remove element from list.</summary>
		/// <returns>Returns Non-Const reference to Last element of Type T from the List.</returns>
		T& back();

		/// <summary>Gets Size ( No. of elements ) of given List.</summary>
		/// <returns>Returns size of list in Unsigned Int ( 32 bits ).</returns>
		uint32_t size() const;

		/// <summary>Removes all elements from the list</summary>
		void clear();

		/// <summary>Iterator start point in the list.</summary>
		/// <returns>Copy of an Iterator pointing to first element in the list.</returns>
		Iterator begin() const;

		/// <summary>Iterator past the end of the list.</summary>
		/// <returns>Copy of an Iterator pointing to one past last element in the list.</returns>
		Iterator end() const;

		/// <summary>Insert passed value after the element pointed by Iterator.</summary>
		/// <param name="t_value">Value to Insert.</param>
		/// <param name="t_it">Position at which value to be inserted.</param>
		//void insertAfter(const T& t_value, const Iterator& t_it);

		/// <summary>Insert passed value after the element pointed by Iterator.</summary>
		/// <param name="t_value">Value to Insert.</param>
		/// <param name="t_it">Position at which value to be inserted.</param>
		/// <returns>Returns Iterator to newly inserted item in the List.</returns>
		Iterator insertAfter(const T& t_value, const Iterator& t_it);

		/// <summary>Finds the given value in the list.</summary>
		/// <param name="t_value">Value to find in list</param>
		/// <returns>Returns a copy of an Iterator pointing to a given item.</returns>
		Iterator find(const T& t_value) const;

		/// <summary>Removes first instance of given value in the list.</summary>
		/// <param name="t_value">Value to remove from list</param>
		//void remove(const T& t_value);

		/// <summary>Removes first instance of given value in the list.</summary>
		/// <param name="t_value">Const reference to the Value to remove from list</param>
		/// <returns>Returns if node corresponding to given value is deleted from the list or not.</returns>
		bool remove(const T& t_value);

		/// <summary>Removes first instance of given value in the list.</summary>
		/// <param name="t_it">Const reference to the Iterator corresponding to the value to remove from list</param>
		/// <returns>Returns if node corresponding to given Iterator is deleted from the list or not.</returns>
		bool remove(const Iterator& t_it);

	private:
		Node* m_front;
		Node* m_back;
		uint32_t m_size;
	};
}
/// <summary>Contains SList template members definitions.</summary>
#include "SList.inl"

