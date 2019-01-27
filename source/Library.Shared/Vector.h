#pragma once
#include <cstdint>
#include <exception>
#include <initializer_list>

namespace FieaGameEngine
{
	/// <summary>
	/// Default Reserve Strategy Functor Declaration.
	/// For custom reserve strategy, user may override method "operator()" in CustomReserveStrategy class. 
	/// If user hasn't implemented own reserve strategy, default reserve strategy declared here will be used.
	/// </summary>
	class ReserveStrategy
	{
	public:
		/// <summary>Functor ( Function Object ) to supply default increment strategy for Vector.</summary>
		/// <param name="t_size">Current Size of the vector in Unsigned int (32-bit)</param>
		/// <param name="t_capacity">Current Capacity of the vector in Unsigned int (32-bit).</param>
		/// <returns>Returns new capacity of the Vector</returns>
		uint32_t operator()(uint32_t t_size, uint32_t t_capacity) const;
	};

	/// <summary>Template Class for Vector.</summary>
	template <typename T>
	class Vector
	{
	private:
		uint32_t m_capacity;
		uint32_t m_size;
		T* m_data_array = nullptr;
		static const inline uint32_t DEFAULT_CAPACITY = 5;

	public:
		/// <summary>Embedded Iterator class for Vector.</summary>
		class Iterator final
		{
			friend Vector;
			friend class ConstIterator;

		public:
			/// <summary>Default Constructor for Iterator.</summary>
			Iterator() = default;

			/// <summary>Copy Constructor for Iterator.</summary>
			/// <param name="t_rhs">Const reference to passed Iterator</param>
			Iterator(const Iterator& t_rhs) = default;

			/// <summary>Assignment Operator Overload</summary>
			/// <param name="t_rhs">Const reference to passed Iterator</param>
			/// <returns>Returns Reference to an Iterator</returns>
			Iterator& operator=(const Iterator& t_rhs) = default;

			/// <summary>Compare two iterators based on their owner container and current node.</summary>
			/// <param name="t_rhs">Const reference to passed Iterator.</param>
			/// <returns>Boolean value indicating whether two iterators point to the same node or not.</returns>
			bool operator==(const Iterator& t_rhs) const;

			/// <summary>Not Equal Operator overload for two iterators.</summary>
			/// <param name="t_rhs">Const reference to passed Iterator.</param>
			/// <returns>Boolean value indicating whether Iterators aren't equal or not.</returns>
			bool operator!=(const Iterator& t_rhs) const;

			/// <summary>Prefix Increment Operator Overload. Moves iterator to next item in the list.</summary>
			/// <returns>Returns Reference to an Iterator.</returns>
			Iterator& operator++();

			/// <summary>Postfix Increment Operator Overload. Moves iterator to next item in the list.</summary>
			/// <param name="int">Just to differentiate between Prefix and Postfix Increment Operator Overload</param>
			/// <returns>Returns copy of current value of Iterator</returns>
			Iterator operator++(int);

			/// <summary>Non-Const version of Dereference Operator overloading.</summary>
			/// <returns>Returns non-const reference to the item contained by the node the node points to.</returns>
			T& operator*() const;

			/// <summary>Destructor for Iterator</summary>
			~Iterator() = default;

		private:
			/// <summary>Constructor for Iterator</summary>
			/// <param name="t_node">Current node which this iterator will point to</param>
			/// <param name="t_owner_vector">Const reference to Owner Container List for this iterator</param>
			Iterator(uint32_t t_current_index, const Vector& t_owner_vector);
			uint32_t m_current_index;
			const Vector<T>* m_owner_vector = nullptr;
		};
		/// <summary>Embedded ConstIterator class for Vector.</summary>
		class ConstIterator final
		{
			friend Vector;

		public:
			/// <summary>Default Constructor for ConstIterator.</summary>
			ConstIterator() = default;

			/// <summary>Implicit Conversion for ConstIterator.</summary>
			/// <param name="t_rhs">Const reference to passed Iterator</param>
			ConstIterator(const Iterator& t_rhs);

			/// <summary>Copy Constructor for ConstIterator.</summary>
			/// <param name="t_rhs">Const reference to passed ConstIterator</param>
			ConstIterator(const ConstIterator& t_rhs) = default;

			/// <summary>Assignment Operator Overload</summary>
			/// <param name="t_rhs">Const reference to passed ConstIterator</param>
			/// <returns>Returns Reference to a ConstIterator</returns>
			ConstIterator& operator=(const ConstIterator& t_rhs) = default;

			/// <summary>Compare two ConstIterators based on their owner container and current node.</summary>
			/// <param name="t_rhs">Const reference to passed ConstIterator.</param>
			/// <returns>Boolean value indicating whether two ConstIterators point to the same node or not.</returns>
			bool operator==(const ConstIterator& t_rhs) const;

			/// <summary>Not Equal Operator overload for two iterators.</summary>
			/// <param name="t_rhs">Const reference to passed ConstIterator.</param>
			/// <returns>Boolean value indicating whether two ConstIterators aren't equal or not.</returns>
			bool operator!=(const ConstIterator& t_rhs) const;

			/// <summary>Prefix Increment Operator Overload. Moves ConstIterator to next item in the list.</summary>
			/// <returns>Returns Reference to an ConstIterator.</returns>
			ConstIterator& operator++();

			/// <summary>Postfix Increment Operator Overload. Moves ConstIterator to next item in the list.</summary>
			/// <param name="int">Just to differentiate between Prefix and Postfix Increment Operator Overload</param>
			/// <returns>Returns copy of current value of ConstIterator</returns>
			ConstIterator operator++(int);

			/// <summary>Non-Const version of Dereference Operator overloading.</summary>
			/// <returns>Returns non-const reference to the item contained by the node the node points to.</returns>
			const T& operator*() const;

			/// <summary>Destructor for ConstIterator</summary>
			~ConstIterator() = default;

		private:
			/// <summary>Constructor for ConstIterator</summary>
			/// <param name="t_node">Current node which this ConstIterator will point to</param>
			/// <param name="t_owner_vector">Const reference to Owner Container List for this ConstIterator</param>
			ConstIterator(uint32_t current_index, const Vector& t_owner_vector);
			uint32_t m_current_index;
			const Vector<T>* m_owner_vector = nullptr;
		};
		/// <summary>Default constructor for Vector array initialisation. It'll be empty initially.</summary>
		/// <param name="default_capacity">Default capacity for new Vector array.</param>
		explicit Vector(const uint32_t& t_default_capacity = DEFAULT_CAPACITY);

		/// <summary>Reserve capacity for the Vector array.</summary>
		/// <param name="new_capacity">New capacity for the Vector array.</param>
		void reserve(const uint32_t t_new_capacity);

		/// <summary>Support for Initializer list syntax ( C++11 ).</summary>
		/// <param name="t_list"> Initializer arguments list of Type T.</param>
		Vector(std::initializer_list<T> t_list);

		/// <summary>Copy Constructor for Vector.</summary>
		/// <param name="t_rhs">Const reference to passed Vector of Type T.</param>
		Vector(const Vector& t_rhs);

		/// <summary>Assignment Operator Overload.</summary>
		/// <param name="t_rhs">Const reference to passed Vector of Type T.</param>
		/// <returns>Returns Mutable Reference to "This" Vector.</returns>
		Vector& operator=(const Vector& t_rhs);

		/// <summary>Non-const version of Subscript Operator overload. Throws an exception if index is invalid.</summary>
		/// <param name="t_rhs">Const reference to passed Vector of Type T.</param>
		/// <returns>Returns Mutable Reference to item at given index.</returns>
		T& operator[](const uint32_t& t_index);

		/// <summary>Const version of Subscript Operator overload. Throws an exception if index is invalid.</summary>
		/// <param name="t_rhs">Const reference to passed Vector of Type T.</param>
		/// <returns>Returns Const Reference to item at given index.</returns>
		const T& operator[](const uint32_t& t_index) const;

		/// <summary>Destructor for Vector.</summary>
		~Vector();

		/// <summary>Pushes element to the end of the Vector Array.</summary>
		/// <param name="t_item">Const reference to passed element of Type T.</param>
		/// <returns>Return Iterator to last element in the List.</returns>
		Iterator pushBack(const T& t_item);

		/// <summary>Removes Last element from the Vector</summary>
		void popBack();

		/// <summary>Gets First element in the Vector. Doesn't remove element from Vector.</summary>
		/// <returns>Returns Const reference to First element of Type T from the Vector Array.</returns>
		const T& front() const;

		/// <summary>Non Const Version of front() method. Gets First element in the Vector. Doesn't remove element from Vector.</summary>
		/// <returns>Returns Non-Const reference to First element of Type T from the Vector Array.</returns>
		T& front();

		/// <summary>Gets Last element in the Vector. Doesn't remove element from Vector.</summary>
		/// <returns>Returns Const reference to Last element of Type T from the Vector Array.</returns>
		const T& back() const;

		/// <summary>Non Const Version of back() method. Gets Last element of the Vector. Doesn't remove element from Vector.</summary>
		/// <returns>Returns Non-Const reference to Last element of Type T from the Vector Array.</returns>
		T& back();

		/// <summary>Iterator for first element in the Vector.</summary>
		/// <returns>Copy of an Iterator pointing to first element in the Vector.</returns>
		Iterator begin() const;

		/// <summary>Iterator past the end of the Vector.</summary>
		/// <returns>Copy of an Iterator pointing to one past last element in the Vector.</returns>
		Iterator end() const;

		/// <summary>ConstIterator for the first element in the Vector.</summary>
		/// <returns>ConstIterator referring to first element in the Vector.</returns>
		ConstIterator cbegin() const;

		/// <summary>ConstIterator past the end of the Vector.</summary>
		/// <returns>ConstIterator referring to one past last element in the Vector.</returns>
		ConstIterator cend() const;

		/// <summary>Total no. of elements currently present in Vector.</summary>
		/// <returns>Returns no. of elements in the Vector</returns>
		uint32_t size() const;

		/// <summary>Total no. of elements for which memory is reserved in Vector.</summary>
		/// <returns>Returns no. of elements allocated within Vector.</returns>
		uint32_t capacity() const;

		/// <summary>Conveys whether Vector is empty or not</summary>
		/// <returns>Returns True if Vector is empty else returns false.</returns>
		bool isEmpty() const;

		/// <summary>Removes all elements from the Vector</summary>
		void clear();

		/// <summary>
		/// Non-Const implementation of at() method. Reference of an element at given index.
		/// Throws exception if the index is invalid.
		/// </summary>
		/// <param name="t_index">Index of the stored data.</param>
		/// <returns>Returns Mutable reference to an element at given index.</returns>
		T& at(uint32_t t_index);

		/// <summary>
		/// Const Implementation of at() method. Reference of an element at given index.
		/// Throws exception if the index is invalid.
		/// </summary>
		/// <param name="t_index">Index of the stored data.</param>
		/// <returns>Returns const reference to an element at given index.</returns>
		const T& at(uint32_t t_index) const;

		/// <summary>Finds the given value in the Vector.</summary>
		/// <param name="t_value">Value to find in Vector</param>
		/// <returns>Returns a copy of an Iterator pointing to a given item.</returns>
		Iterator find(const T& t_value) const;

		/// <summary>Removes first instance of given value in the Vector.</summary>
		/// <param name="t_value">Const reference to the Value to remove from Vector.</param>
		/// <returns>Returns true if node corresponding to given value is deleted from the Vector else false.</returns>
		bool remove(const T& t_value);

		/// <summary>Removes first instance of given value in the Vector.</summary>
		/// <param name="t_it">Const reference to the Iterator corresponding to the value to remove from Vector</param>
		/// <returns>Returns true if node corresponding to given Iterator is deleted from the Vector else false</returns>
		bool remove(const Iterator& t_it);

		/// <summary>Removes range of values from the vector based on start and end points ( Iterators ).</summary>
		/// <param name="t_begin">Const reference to the Iterator corresponding to the starting element in the range</param>
		/// <param name="t_begin">Const reference to the Iterator corresponding to the last element in the range</param>
		/// /// <returns>Returns if nodes corresponding to the given range are deleted from the Vector or not.</returns>
		bool remove(const Iterator& t_begin, const Iterator& t_end);
	};
}

#include "Vector.inl"
#include "CustomReserveStrategy.h"