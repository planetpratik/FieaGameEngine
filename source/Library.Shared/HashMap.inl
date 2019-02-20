#pragma once
#include "HashMap.h"

namespace FieaGameEngine
{
	template <typename TKey, typename TData, typename HashFunctor>
	const HashFunctor HashMap<TKey, TData, HashFunctor>::m_hash_function;

#pragma region HashMap

	template <typename TKey, typename TData, typename HashFunctor>
	HashMap<TKey, TData, HashFunctor>::HashMap(uint32_t t_bucket_size) :
		m_size(0), m_buckets(t_bucket_size, true)
	{
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline HashMap<TKey, TData, HashFunctor>::HashMap(std::initializer_list<PairType> t_list) :
		m_size(0), m_buckets(static_cast<uint32_t>(t_list.size()), true)
	{
		for (const auto& value : t_list)
		{
			insert(value);
		}
	}

	template <typename TKey, typename TData, typename HashFunctor>
	typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::find(const TKey& t_index)
	{
		Iterator it = end();
		uint32_t index = m_hash_function(t_index) % m_buckets.size();
		ChainIterator chain_it = m_buckets[index].begin();
		for (; chain_it != m_buckets[index].end(); ++chain_it)
		{
			if ((*chain_it).first == t_index)
			{
				it = Iterator(*this, index, chain_it);
				break;
			}
		}
		return it;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	typename HashMap<TKey, TData, HashFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor>::find(const TKey& t_index) const
	{
		ConstIterator it = end();
		uint32_t index = m_hash_function(t_index) % m_buckets.size();
		ChainIterator chain_it = m_buckets[index].begin();
		for (; chain_it != m_buckets[index].end(); ++chain_it)
		{
			if ((*chain_it).first == t_index)
			{
				it = Iterator(*this, index, chain_it);
				break;
			}
		}
		return it;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::insert(const PairType& t_pair)
	{
		uint32_t index = m_hash_function(t_pair.first) % m_buckets.size();
		Iterator it;
		if (containsKey(t_pair.first, it))
		{
			return it;
		}
		it = Iterator(*this, index, m_buckets[index].pushBack(t_pair));
		++m_size;
		return it;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::insert(const PairType& t_pair, bool& t_result)
	{
		t_result = false;
		uint32_t index = m_hash_function(t_pair.first) % m_buckets.size();
		Iterator it;
		if (containsKey(t_pair.first, it))
		{
			return it;
		}
		it = Iterator(*this, index, m_buckets[index].pushBack(t_pair));
		++m_size;
		t_result = true;
		return it;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline TData& HashMap<TKey, TData, HashFunctor>::operator[](const TKey& t_key)
	{
		return ((*(insert(std::make_pair(t_key, TData())))).second);
	}

	template <typename TKey, typename TData, typename HashFunctor>
	void HashMap<TKey, TData, HashFunctor>::remove(const TKey& t_key)
	{
		Iterator it;
		if (containsKey(t_key, it))
		{
			m_buckets[it.m_current_bucket_index].remove(*it);
			--m_size;
		}
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline void HashMap<TKey, TData, HashFunctor>::clear()
	{
		for (auto& chain : m_buckets)
		{
			chain.clear();
		}

		m_size = 0;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline uint32_t HashMap<TKey, TData, HashFunctor>::size() const
	{
		return m_size;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline bool HashMap<TKey, TData, HashFunctor>::containsKey(const TKey& t_key)
	{
		Iterator it;
		return containsKey(t_key, it);
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline bool HashMap<TKey, TData, HashFunctor>::containsKey(const TKey & t_key, Iterator& t_it)
	{
		bool found = false;
		t_it = find(t_key);
		if (t_it != end())
		{
			found = true;
		}
		return found;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline TData& FieaGameEngine::HashMap<TKey, TData, HashFunctor>::at(const TKey& t_key)
	{
		Iterator it = find(t_key);
		if (it == end())
		{
			throw std::exception("Invalid Operation! Key is not present in hashmap.");
		}
		return((*it).second);
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline const TData& FieaGameEngine::HashMap<TKey, TData, HashFunctor>::at(const TKey& t_key) const
	{
		ConstIterator it = find(t_key);
		if (it == end())
		{
			throw std::exception("Invalid Operation! Key is not present in hashmap.");
		}
		return((*it).second);
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::begin()
	{
		for (uint32_t i = 0; i < m_buckets.size(); ++i)
		{
			if (!(0 == m_buckets[i].size()))
			{
				return Iterator(*this, i, m_buckets[i].begin());
			}
		}
		return end();
	}

	template <typename TKey, typename TData, typename HashFunctor>
	typename HashMap<TKey, TData, HashFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor>::begin() const
	{
		for (uint32_t i = 0; i < m_buckets.size(); ++i)
		{
			if (!(0 == m_buckets[i].size()))
			{
				return ConstIterator(*this, i, m_buckets[i].begin());
			}
		}
		return end();
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor>::cbegin() const
	{
		for (uint32_t i = 0; i < m_buckets.size(); ++i)
		{
			if (!(0 == m_buckets[i].size()))
			{
				return ConstIterator(*this, i, m_buckets[i].begin());
			}
		}
		return end();
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::end()
	{
		return Iterator(*this, m_buckets.size(), m_buckets[m_buckets.size() - 1].end());
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor>::end() const
	{
		return ConstIterator(*this, m_buckets.size(), m_buckets[m_buckets.size() - 1].end());
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor>::cend() const
	{
		return ConstIterator(*this, m_buckets.size(), m_buckets[m_buckets.size() - 1].end());
	}

	template <typename TKey, typename TData, typename HashFunctor>
	void HashMap<TKey, TData, HashFunctor>::rehash(uint32_t t_number_of_buckets)
	{
		HashMap new_map(t_number_of_buckets);
		for (Iterator it = begin(); it != end(); ++it)
		{
			new_map.insert(*it);
		}
		*this = std::move(new_map);
	}

	template <typename TKey, typename TData, typename HashFunctor>
	float_t HashMap<TKey, TData, HashFunctor>::loadFactor() const
	{
		uint32_t no_of_buckets_in_use = 0;
		for (auto& it : m_buckets)
		{
			if (!it.isEmpty())
			{
				++no_of_buckets_in_use;
			}
		}
		return static_cast<float_t>(no_of_buckets_in_use) / m_buckets.size();
	}

	template <typename TKey, typename TData, typename HashFunctor>
	uint32_t HashMap<TKey, TData, HashFunctor>::noOfBuckets() const
	{
		return m_buckets.size();
	}
#pragma endregion

#pragma region Iterator

	template<typename TKey, typename TData, typename HashFunctor>
	HashMap<TKey, TData, HashFunctor>::Iterator::Iterator(const HashMap& t_owner, uint32_t t_current_bucket_index, typename const HashMap::ChainIterator& t_current_pair) :
		m_current_pair(t_current_pair), m_current_bucket_index(t_current_bucket_index), m_owner(&t_owner)
	{
	}

	template <typename TKey, typename TData, typename HashFunctor>
	typename HashMap<TKey, TData, HashFunctor>::Iterator& HashMap<TKey, TData, HashFunctor>::Iterator::operator++()
	{
		if (m_owner == nullptr)
		{
			throw std::exception("Invalid operation! Iterator doesn't belong to any HashMap.");
		}
		if (ConstIterator(*this) == m_owner->end())
		{
			throw std::exception("Invalid operation! Iterator can't be incremented as it's already reached at end.");
		}
		++m_current_pair;
		if (m_current_pair == (m_owner->m_buckets[m_current_bucket_index]).end())
		{
			++m_current_bucket_index;
			while ((m_current_bucket_index < m_owner->m_buckets.size()) && m_owner->m_buckets[m_current_bucket_index].isEmpty())
			{
				++m_current_bucket_index;
			}
			if (m_current_bucket_index < m_owner->m_buckets.size())
			{
				m_current_pair = m_owner->m_buckets[m_current_bucket_index].begin();
			}
			else
			{
				m_current_pair = m_owner->m_buckets[m_current_bucket_index - 1].end();
			}
		}
		return *this;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::Iterator::operator++(int)
	{
		Iterator it = *this;
		operator++();
		return it;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	typename HashMap<TKey, TData, HashFunctor>::PairType& HashMap<TKey, TData, HashFunctor>::Iterator::operator*() const
	{
		if (m_owner == nullptr)
		{
			throw std::exception("Invalid operation! Iterator doesn't belong to any HashMap.");
		}
		if (m_current_pair == (m_owner->m_buckets[m_current_bucket_index]).end())
		{
			throw std::exception("Invalid operation! Cannot dereference an Iterator pointing to the end of list.");
		}
		return *m_current_pair;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	typename HashMap<TKey, TData, HashFunctor>::PairType* HashMap<TKey, TData, HashFunctor>::Iterator::operator->() const
	{
		return &(operator*());
	}

	template <typename TKey, typename TData, typename HashFunctor>
	bool HashMap<TKey, TData, HashFunctor>::Iterator::operator==(const Iterator& t_rhs) const
	{
		return (m_owner == t_rhs.m_owner) && (m_current_bucket_index == t_rhs.m_current_bucket_index) && (m_current_pair == t_rhs.m_current_pair);
	}

	template <typename TKey, typename TData, typename HashFunctor>
	bool HashMap<TKey, TData, HashFunctor>::Iterator::operator!=(const Iterator& t_rhs) const
	{
		return !(*this == t_rhs);
	}

#pragma endregion

#pragma region ConstIterator

	template<typename TKey, typename TData, typename HashFunctor>
	HashMap<TKey, TData, HashFunctor>::ConstIterator::ConstIterator(const HashMap& t_owner, uint32_t t_current_bucket_index, typename const HashMap::ChainIterator& t_current_pair) :
		m_current_pair(t_current_pair), m_current_bucket_index(t_current_bucket_index), m_owner(&t_owner)
	{
	}

	template <typename TKey, typename TData, typename HashFunctor>
	HashMap<TKey, TData, HashFunctor>::ConstIterator::ConstIterator(const Iterator& t_rhs) :
		m_current_bucket_index(t_rhs.m_current_bucket_index), m_owner(t_rhs.m_owner), m_current_pair(t_rhs.m_current_pair)
	{
	}

	template <typename TKey, typename TData, typename HashFunctor>
	typename HashMap<TKey, TData, HashFunctor>::ConstIterator& HashMap<TKey, TData, HashFunctor>::ConstIterator::operator++()
	{
		if (m_owner == nullptr)
		{
			throw std::exception("Invalid operation! Iterator doesn't belong to any HashMap.");
		}
		if (*this == m_owner->end())
		{
			throw std::exception("Invalid operation! Iterator can't be incremented as it's already reached at end.");
		}
		++m_current_pair;
		if (m_current_pair == (m_owner->m_buckets[m_current_bucket_index]).end())
		{
			++m_current_bucket_index;
			while ((m_current_bucket_index < m_owner->m_buckets.size()) && m_owner->m_buckets[m_current_bucket_index].isEmpty())
			{
				++m_current_bucket_index;
			}
			if (m_current_bucket_index < m_owner->m_buckets.size())
			{
				m_current_pair = m_owner->m_buckets[m_current_bucket_index].begin();
			}
			else
			{
				m_current_pair = m_owner->m_buckets[m_current_bucket_index - 1].end();
			}
		}
		return *this;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	typename HashMap<TKey, TData, HashFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor>::ConstIterator::operator++(int)
	{
		ConstIterator it = *this;
		operator++();
		return it;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	typename const HashMap<TKey, TData, HashFunctor>::PairType& HashMap<TKey, TData, HashFunctor>::ConstIterator::operator*() const
	{
		if (m_owner == nullptr)
		{
			throw std::exception("Invalid operation! Iterator doesn't belong to any HashMap.");
		}
		if (m_current_pair == (m_owner->m_buckets[m_current_bucket_index]).end())
		{
			throw std::exception("Invalid operation! Cannot dereference an Iterator pointing to the end of list.");
		}
		return *m_current_pair;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	typename const HashMap<TKey, TData, HashFunctor>::PairType* HashMap<TKey, TData, HashFunctor>::ConstIterator::operator->() const
	{
		return &(operator*());
	}

	template <typename TKey, typename TData, typename HashFunctor>
	bool HashMap<TKey, TData, HashFunctor>::ConstIterator::operator==(const ConstIterator& t_rhs) const
	{
		return (m_owner == t_rhs.m_owner) && (m_current_bucket_index == t_rhs.m_current_bucket_index) && (m_current_pair == t_rhs.m_current_pair);
	}

	template <typename TKey, typename TData, typename HashFunctor>
	bool HashMap<TKey, TData, HashFunctor>::ConstIterator::operator!=(const ConstIterator& t_rhs) const
	{
		return !(*this == t_rhs);
	}

#pragma endregion

}