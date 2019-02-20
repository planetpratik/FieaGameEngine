#pragma once
#include <exception>
#include <utility>
#include <initializer_list>
#include "SList.h"
#include "Vector.h"
#include "DefaultHash.h"

namespace FieaGameEngine
{
	/// <summary>Template Class for HashMap</summary>
	template <typename TKey, typename TData, typename HashFunctor = DefaultHash<TKey>>
	class HashMap final
	{
	public:
		/// <summary>PairType (Key-Value Pair) format for storing data.</summary>
		using PairType = std::pair<TKey, TData>;
	private:
		using ChainType = SList<PairType>;
		using BucketType = Vector<ChainType>;
		using ChainIterator = typename ChainType::Iterator;
	public:
		/// <summary>Embedded Iterator Auxillary class in HashMap Template.</summary>
		class Iterator final
		{
			friend HashMap;
			friend class ConstIterator;
		public:
			/// <summary>Default Constructor for HashMap Iterator.</summary>
			Iterator() = default;

			/// <summary>Default Copy Constructor for HashMap Iterator.</summary>
			/// <param name="t_rhs">Const reference to passed Iterator.</param>
			Iterator(const Iterator& t_rhs) = default;

			/// <summary>Default Move Constructor for Hashmap Iterator.</summary>
			/// <param name="t_rhs">R-Value reference to an Iterator which is to be moved.</param>
			Iterator(Iterator&& t_rhs) = default;

			/// <summary>Default Copy-Assignment operator for HashMap Iterator.</summary>
			/// <param name="t_rhs">Const reference to passed Iterator.</param>
			/// <returns>Returns reference to an Iterator.</returns>
			Iterator& operator=(const Iterator& t_rhs) = default;

			/// <summary>Default Move-Assignment operator for HashMap Iterator.</summary>
			/// <param name="t_rhs">R-Value reference to an Iterator which is to be moved.</param>
			/// <returns>Returns reference to an Iterator.</returns>
			Iterator& operator=(Iterator&& t_rhs) = default;

			/// <summary>Default destructor for a HashMap Iterator</summary>
			~Iterator() = default;

			/// <summary>Prefix Increment operator overload for HashMap Iterator.</summary>
			/// <returns>Returns reference to an Iterator.</returns>
			Iterator& operator++();

			/// <summary>Postfix Increment operator overload for HashMap Iterator.</summary>
			/// <param name="int">Just a placeholder to distinguish between prefix and postfix increment operator overloads.</param>
			/// <returns>Returns copy of an Iterator before increment operation.</returns>
			Iterator operator++(int);

			/// <summary>Dereference Operator (*) overload for HashMap Iterator.</summary>
			/// <returns>Returns reference to Key-Value PairType pointed by current Iterator.</returns>
			PairType& operator*() const;

			/// <summary>Dereference Operator - Pointer member access (->) overload for HashMap Iterator.</summary>
			/// <returns>Returns Pointer to Key-Value PairType pointed by current Iterator.</returns>
			PairType* operator->() const;

			/// <summary>Comparison Operator (==) overload for HashMap Iterator.</summary>
			/// <param name="t_ths">Const reference to a passed Iterator.</param>
			/// <returns>Boolean value indicating whether two iterators are equal or not.</returns>
			bool operator==(const Iterator& t_ths) const;

			/// <summary>Not Equal Operator (!=) overload for HashMap Iterator.</summary>
			/// <param name="t_ths">Const reference to a passed Iterator.</param>
			/// <returns>Boolean value indicating whether two iterators aren't equal or not.</returns>
			bool operator!=(const Iterator& t_ths) const;
		private:
			Iterator(const HashMap& t_owner, uint32_t t_current_bucket_index, const ChainIterator& t_current_pair);
			uint32_t m_current_bucket_index = 0;
			ChainIterator m_current_pair;
			const HashMap* m_owner = nullptr;
		};
		/// <summary>Embedded ConstIterator Auxillary class in HashMap Template</summary>
		class ConstIterator final
		{
			friend HashMap;
			friend class Iterator;
		public:
			/// <summary>Default Constructor for HashMap ConstIterator.</summary>
			ConstIterator() = default;

			/// <summary>Implicit Copy Constructor for converting Iterator into HashMap ConstIterator.</summary>
			/// <param name="t_rhs">Const reference to passed Iterator.</param>
			ConstIterator(const Iterator& t_rhs);

			/// <summary>Default Copy Constructor for HashMap ConstIterator.</summary>
			/// <param name="t_rhs">Const reference to passed ConstIterator.</param>
			ConstIterator(const ConstIterator& t_rhs) = default;

			/// <summary>Default Move Constructor for Hashmap ConstIterator.</summary>
			/// <param name="t_rhs">R-Value reference to a ConstIterator which is to be moved.</param>
			ConstIterator(ConstIterator&& t_rhs) = default;

			/// <summary>Default Copy-Assignment operator for HashMap ConstIterator.</summary>
			/// <param name="t_rhs">Const reference to passed ConstIterator.</param>
			/// <returns>Returns reference to a ConstIterator.</returns>
			ConstIterator& operator=(const ConstIterator& t_rhs) = default;

			/// <summary>Default Move-Assignment operator for HashMap ConstIterator.</summary>
			/// <param name="t_rhs">R-Value reference to a ConstIterator which is to be moved.</param>
			/// <returns>Returns reference to a ConstIterator.</returns>
			ConstIterator& operator=(ConstIterator&& t_rhs) = default;

			/// <summary>Default destructor for a HashMap ConstIterator</summary>
			~ConstIterator() = default;

			/// <summary>Prefix Increment operator overload for HashMap ConstIterator.</summary>
			/// <returns>Returns reference to a ConstIterator.</returns>
			ConstIterator& operator++();

			/// <summary>Postfix Increment operator overload for HashMap ConstIterator.</summary>
			/// <param name="int">Just a placeholder to distinguish between prefix and postfix increment operator overloads.</param>
			/// <returns>Returns copy of a ConstIterator before increment operation.</returns>
			ConstIterator operator++(int);

			/// <summary>Dereference Operator (*) overload for HashMap ConstIterator.</summary>
			/// <returns>Returns const reference to Key-Value PairType pointed by current ConstIterator.</returns>
			const PairType& operator*() const;

			/// <summary>Dereference Operator - Pointer member access (->) overload for HashMap ConstIterator.</summary>
			/// <returns>Returns const Pointer to Key-Value PairType pointed by current ConstIterator.</returns>
			const PairType* operator->() const;

			/// <summary>Comparison Operator (==) overload for HashMap ConstIterator.</summary>
			/// <param name="t_ths">Const reference to a passed ConstIterator.</param>
			/// <returns>Boolean value indicating whether two iterators are equal or not.</returns>
			bool operator==(const ConstIterator& t_ths) const;

			/// <summary>Not Equal Operator (!=) overload for HashMap ConstIterator.</summary>
			/// <param name="t_ths">Const reference to a passed ConstIterator.</param>
			/// <returns>Boolean value indicating whether two iterators aren't equal or not.</returns>
			bool operator!=(const ConstIterator& t_ths) const;
		private:
			ConstIterator(const HashMap& t_owner, uint32_t t_current_bucket_index, const ChainIterator& t_current_pair);
			uint32_t m_current_bucket_index = 0;
			ChainIterator m_current_pair;
			const HashMap* m_owner = nullptr;
		};

		/// <summary>Parameterised Constructor for HashMap with number of specified buckets.</summary>
		/// <param name="t_bucket_size">Initial number of buckets for constructing HashMap.</param>
		explicit HashMap(uint32_t t_bucket_size = DEFAULT_SIZE);

		/// <summary>Support for Initializer list syntax ( C++11 ).</summary>
		/// <param name="t_list"> Initializer arguments list of Type PairType ( Key-Value pairs ).</param>
		HashMap(std::initializer_list<PairType> t_list);

		/// <summary>Default Copy Constructor for HashMap.</summary>
		/// <param name="t_rhs"></param>
		HashMap(const HashMap& t_rhs) = default;

		/// <summary>Default Copy Assignment Operator implementation for HashMap.</summary>
		/// <param name="t_rhs">Const reference to passed HashMap.</param>
		/// <returns>Returns reference to HashMap.</returns>
		HashMap& operator=(const HashMap& t_rhs) = default;

		/// <summary>Default Move Constructor for HashMap.</summary>
		/// <param name="t_rhs">R-Value reference to a HashMap which is to be moved.</param>
		HashMap(HashMap&& t_rhs) = default;

		/// <summary>Default Move-Assignment operator for HashMap.</summary>
		/// <param name="t_rhs">R-Value reference to a HashMap which is to be moved.</param>
		/// <returns>Returns reference to a HashMap.</returns>
		HashMap& operator=(HashMap&& t_rhs) = default;

		/// <summary>Default Destructor for a HashMap.</summary>
		~HashMap() = default;

		/// <summary>Non-Const version of find method to Search a given key in HashMap.</summary>
		/// <param name="t_key">Const reference to a key of type TKey.</param>
		/// <returns> Returns copy of an Iterator to an element if found else return an end.</returns>
		Iterator find(const TKey& t_key);

		/// <summary>Const version of find method to Search a given key in HashMap.</summary>
		/// <param name="t_key">Const reference to a key of type TKey.</param>
		/// <returns> Returns copy of a ConstIterator to an element if found else return an end.</returns>
		ConstIterator find(const TKey& t_key) const;

		/// <summary>Inserts element into HashMap for a specified PairType.</summary>
		/// <param name="t_pair">Const reference to a PairType ( Key-Value Pair ).</param>
		/// <returns>Returns copy of an Iterator if element is inserted else return Iterator to an element if already exists.</returns>
		Iterator insert(const PairType& t_pair);

		/// <summary>Inserts element into HashMap for a specified PairType.</summary>
		/// <param name="t_pair">Const reference to a PairType ( Key-Value Pair ).</param>
		/// <param name="t_result">Const reference to a boolean value to store result (Used as an out parameter)</param>
		/// <returns>Returns copy of an Iterator if element is inserted else return Iterator to an element if already exists.</returns>
		Iterator insert(const PairType& t_pair, bool& t_result);

		/// <summary>Index operator overload to get an element at specified Key. Creates entry if not previously exist.</summary>
		/// <param name="t_key">Const reference to a key of type TKey.</param>
		/// <returns>returns TData (Value) reference for specified key.</returns>
		TData& operator[](const TKey& t_key);

		/// <summary>Removes element from HashMap for specified key. If element doesn't exist, then does nothing.</summary>
		/// <param name="t_key">Const reference to a key of type TKey.</param>
		void remove(const TKey& t_key);

		/// <summary>Clear entire HashMap.</summary>
		void clear();

		/// <summary>Total no. of key value pairs stored in the HashMap.</summary>
		/// <returns>Returns total no. of Key-value pairs stored in HashMap in Unsigned Int ( 32-bit ) format.</returns>
		uint32_t size() const;

		/// <summary>Checks if Pair associated with given key exists in HashMap or not.</summary>
		/// <param name="t_key">Const reference to a key of type TKey.</param>
		/// <returns>Returns boolean value indicating presence of key within HashMap.</returns>
		bool containsKey(const TKey& t_key);

		/// <summary>Checks if Pair associated with given key exists in HashMap or not.</summary>
		/// <param name="t_key">Const reference to a key of type TKey.</param>
		/// <param name="t_it">Const reference to an Iterator to store Iterator belonging given key (Used as an out parameter).</param>
		/// <returns>Returns boolean value indicating presence of key within HashMap.</returns>
		bool containsKey(const TKey& t_key, Iterator& t_it);

		/// <summary>
		/// Non-Const version of at() method to checks if Pair associated with given key exists in HashMap or not.
		/// If found, return associated value.
		/// </summary>
		/// <param name="t_key">Const reference to a key of type TKey.</param>
		/// <returns>returns TData (Value) reference for specified key.</returns>
		TData& at(const TKey& t_key);

		/// <summary>
		/// Const version of at() method to checks if Pair associated with given key exists in HashMap or not.
		/// If found, return associated value.
		/// </summary>
		/// <param name="t_key">Const reference to a key of type TKey.</param>
		/// <returns>returns const reference to TData (Value) for specified key.</returns>
		const TData& at(const TKey& t_key) const;

		/// <summary>Non-const version of begin() - Get an Iterator to first element in the HashMap.</summary>
		/// <returns>Returns copy of an Iterator to first element in the HashMap.</returns>
		Iterator begin();

		/// <summary>Const version of begin() - Get an Iterator to first element in the HashMap.</summary>
		/// <returns>Returns copy of an ConstIterator to first element in the HashMap.</returns>
		ConstIterator begin() const;

		/// <summary>Get an Iterator to first element in the HashMap.</summary>
		/// <returns>Returns copy of an ConstIterator to first element in the HashMap.</returns>
		ConstIterator cbegin() const;

		/// <summary>Non const version of end() - Gets an Iterator pointing to end of HashMap.</summary>
		/// <returns>Copy of an Iterator pointing to end of HashMap</returns>
		Iterator end();

		/// <summary>Const version of end() - Gets an ConstIterator pointing to end of HashMap.</summary>
		/// <returns>Copy of an ConstIterator pointing to end of HashMap</returns>
		ConstIterator end() const;

		/// <summary>Gets an ConstIterator pointing to end of HashMap.</summary>
		/// <returns>Copy of an ConstIterator pointing to end of HashMap</returns>
		ConstIterator cend() const;

		/// <summary>Rehashes the HashMap using passed bucket size.</summary>
		/// <param name="t_number_of_buckets">Total number of buckets to be used in new Rehashed HashMap.</param>
		void rehash(uint32_t t_number_of_buckets);

		/// <summary>Gives ratio of buckets in use with respect to total buckets in HashMap.</summary>
		/// <returns>Returns ratio in float_t format.</returns>
		float_t loadFactor() const;

		/// <summary>Get total no.of buckets in HashMap.</summary>
		/// <returns>Returns total no. of buckets in HashMap as an Unsigned Integer (32-bit).</returns>
		uint32_t noOfBuckets() const;
	private:
		BucketType m_buckets;
		static const HashFunctor m_hash_function;
		uint32_t m_size;
		static const uint32_t DEFAULT_SIZE = 10;
	};
}
#include "HashMap.inl"
#include "DefaultHash.h"