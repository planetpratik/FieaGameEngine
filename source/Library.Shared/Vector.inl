#pragma once
#include "Vector.h"
#include <algorithm>

namespace FieaGameEngine
{
#pragma region ReserveStrategy

	inline uint32_t ReserveStrategy::operator()(uint32_t t_size, uint32_t t_capacity) const
	{
		if (t_size == 0 && t_capacity == 0)
		{
			return 5;
		}
		return 2 * t_capacity;
	}

#pragma endregion

#pragma region Vector

	template <typename T>
	inline Vector<T>::Vector(const uint32_t& t_default_capacity) :
		m_capacity(0), m_size(0), m_data_array(nullptr)
	{
		reserve(t_default_capacity);
	}

	template <typename T>
	void Vector<T>::reserve(const uint32_t t_new_capacity)
	{
		m_data_array = static_cast<T*>(realloc(m_data_array, t_new_capacity * sizeof(T)));
		m_capacity = t_new_capacity;
	}

	template <typename T>
	void Vector<T>::resize(const uint32_t t_size)
	{
		if (t_size < m_size)
		{
			for (size_t i = t_size; i < m_size; ++i)
			{
				m_data_array[i].~T();
			}
		}

		m_data_array = reinterpret_cast<T*>(realloc(m_data_array, sizeof(T) * t_size));
		for (size_t i = m_size; i < t_size; i++)
		{
			new (m_data_array + i)T();
		}

		m_size = m_capacity = t_size;
	}

	template <typename T>
	Vector<T>::Vector(std::initializer_list<T> t_list):
		m_capacity(0), m_size(0), m_data_array(nullptr)
	{
		for (const auto& value : t_list)
		{
			pushBack(value);
		}
	}

	template <typename T>
	Vector<T>::Vector(const Vector& t_rhs) :
		m_capacity(0), m_size(0), m_data_array(nullptr)
	{
		operator=(t_rhs);
	}

	template<typename T>
	Vector<T>::Vector(Vector&& t_rhs) :
		m_size(t_rhs.m_size), m_capacity(t_rhs.m_capacity), m_data_array(t_rhs.m_data_array)
	{
		t_rhs.m_size = 0;
		t_rhs.m_capacity = 0;
		t_rhs.m_data_array = nullptr;
	}

	template <typename T>
	typename Vector<T>& Vector<T>::operator=(const Vector& t_rhs)
	{
		if (this != &t_rhs)
		{
			clear();
			//reserve(static_cast<uint32_t>(t_list.size()), true);
			for (const auto& value : t_rhs)
			{
				pushBack(value);
			}
		}
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(Vector&& t_rhs)
	{
		if (this != &t_rhs)
		{
			clear();
			m_size = t_rhs.m_size;
			m_capacity = t_rhs.m_capacity;
			m_data_array = t_rhs.m_data_array;

			t_rhs.m_size = 0;
			t_rhs.m_capacity = 0;
			t_rhs.m_data_array = nullptr;
		}
		return *this;
	}

	template <typename T>
	T& Vector<T>::operator[](const uint32_t& t_index)
	{
		if (t_index >= m_size)
		{
			throw std::exception("Array out of bounds Exception !");
		}
		return m_data_array[t_index];
	}

	template <typename T>
	const T& Vector<T>::operator[](const uint32_t& t_index) const
	{
		if (t_index >= m_size)
		{
			throw std::exception("Array out of bounds Exception !");
		}
		return m_data_array[t_index];
	}

	template<typename T>
	inline bool Vector<T>::operator==(const Vector & rhs) const
	{
		if (m_size != rhs.m_size)
		{
			return false;
		}

		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (m_data_array[i] != rhs.m_data_array[i])
			{
				return false;
			}
		}

		return true;
	}

	template<typename T>
	inline bool Vector<T>::operator!=(const Vector & rhs) const
	{
		return !(operator==(rhs));
	}

	template <typename T>
	inline Vector<T>::~Vector()
	{
		clear();
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::pushBack(const T& t_item)
	{
		// If array needs to exceed, reallocate array to new location using new capacity and then add new object to the end
		if (m_size == m_capacity)
		{
			ReserveStrategy rs;
			uint32_t new_capacity = rs.operator()(m_size, m_capacity);
			if (new_capacity < m_capacity)
			{
				throw std::exception("Error: Bad Reserve Strategy! New capacity must be greater than current capacity.");
			}
			reserve(new_capacity);
		}
		//allocate memory for new object using placement new
		new(m_data_array + m_size)T(t_item);
		++m_size;
		return Iterator(m_size, *this);
	}

	template <typename T>
	inline void Vector<T>::popBack()
	{
		if (isEmpty())
		{
			throw std::exception("Invalid Operation! Vector is empty.");
		}
		m_data_array[m_size - 1].~T();
		--m_size;
	}

	template <typename T>
	inline T& Vector<T>::front()
	{
		if (isEmpty())
		{
			throw std::exception("Invalid Operation! Vector is empty.");
		}
		return m_data_array[0];
	}

	template <typename T>
	inline const T& Vector<T>::front() const
	{
		if (isEmpty())
		{
			throw std::exception("Invalid Operation! Vector is empty.");
		}
		return m_data_array[0];
	}

	template <typename T>
	inline T& Vector<T>::back()
	{
		if (isEmpty())
		{
			throw std::exception("Invalid Operation! Vector is empty.");
		}
		return m_data_array[m_size - 1];
	}

	template <typename T>
	inline const T& Vector<T>::back() const
	{
		if (isEmpty())
		{
			throw std::exception("Invalid Operation! Vector is empty.");
		}
		return m_data_array[m_size - 1];
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::begin() const
	{
		return Iterator(0, *this);
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::end() const
	{
		return Iterator(m_size, *this);
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator(0, *this);
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator(m_size, *this);
	}

	template <typename T>
	inline uint32_t Vector<T>::size() const
	{
		return m_size;
	}

	template <typename T>
	inline uint32_t Vector<T>::capacity() const
	{
		return m_capacity;
	}

	template <typename T>
	inline bool Vector<T>::isEmpty() const
	{
		return (0 == m_size);
	}

	template <typename T>
	inline void Vector<T>::clear()
	{
		for (uint32_t i = 0; i < m_size; ++i)
		{
			m_data_array[i].~T();
		}
		if (m_capacity != 0)
		{
			free(m_data_array);
		}
		m_size = 0;
		m_capacity = 0;
		m_data_array = nullptr;
	}

	template<typename T>
	inline void Vector<T>::erase()
	{
		for (uint32_t i = 0; i < m_size; ++i)
		{
			m_data_array[i].~T();
		}
		m_size = 0;
	}

	template <typename T>
	inline T& Vector<T>::at(uint32_t t_index)
	{
		if (t_index >= m_size)
		{
			throw std::exception("Invalid Index! Index must be smaller than size of Vector.");
		}
		return operator[](t_index);
	}

	template <typename T>
	inline const T& Vector<T>::at(uint32_t t_index) const
	{
		return operator[](t_index);
	}

	template<typename T>
	typename Vector<T>::Iterator Vector<T>::find(const T& t_value) const
	{
		Iterator it = begin();
		for (; it != end(); ++it)
		{
			if (t_value == *it)
			{
				break;
			}
		}
		return it;
	}

	template<typename T>
	inline bool Vector<T>::remove(const T& t_value)
	{
		return remove(find(t_value));
	}

	template<typename T>
	bool Vector<T>::remove(const Iterator& t_it)
	{
		bool found = false;
		if (t_it.m_owner_vector != this)
		{
			throw std::exception("Invalid Iterator ! Iterator is owned by different vector.");
		}
		if (t_it != end())
		{
			(*t_it).~T();
			std::memmove(m_data_array + t_it.m_current_index, m_data_array + t_it.m_current_index + 1, sizeof(T)*(m_size - t_it.m_current_index - 1));
			--m_size;
			found = true;
		}
		return found;
	}

	template <typename T>
	bool Vector<T>::remove(const Iterator& t_begin, const Iterator& t_end)
	{
		bool success = false;
		if (t_begin.m_owner_vector != this || t_end.m_owner_vector != this)
		{
			throw std::exception("Invalid Iterator ! Iterator is owned by different vector.");
		}
		if (t_begin.m_current_index > t_end.m_current_index)
		{
			throw std::exception("Begin Iterator must be smaller than End Iterator");
		}
		Iterator it = t_begin;
		for (; it != t_end; ++it)
		{
			(*it).~T();
		}
		std::memmove(m_data_array + t_begin.m_current_index, m_data_array + t_end.m_current_index + 1, sizeof(T)*(m_size - t_end.m_current_index -1));
		m_size = m_size - (t_end.m_current_index - t_begin.m_current_index + 1);
		success = true;
		return success;
	}

#pragma endregion

#pragma region Iterator

	template <typename T>
	Vector<T>::Iterator::Iterator(uint32_t t_current_index, const Vector& t_owner_vector) :
		m_current_index(t_current_index), m_owner_vector(&t_owner_vector)
	{
	}

	template <typename T>
	bool Vector<T>::Iterator::operator==(const Iterator& t_rhs) const
	{
		return (m_owner_vector == t_rhs.m_owner_vector) && (m_current_index == t_rhs.m_current_index);
	}

	template <typename T>
	bool Vector<T>::Iterator::operator!=(const Iterator& t_rhs) const
	{
		return !(*this == t_rhs);
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (m_owner_vector == nullptr)
		{
			throw std::exception("Iterator doesn't belong to any Vector");
		}
		if (0 == (m_owner_vector->size()))
		{
			throw std::exception("Invalid Operation! Vector doesn't have any elements. Iterator can't be incremented");
		}
		if (m_current_index > (m_owner_vector->size() - 1))
		{
			throw std::exception("Invalid Operation! Vector Index out of Bounds");
		}
		++m_current_index;
		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator it = *this;
		operator++();
		return it;
	}

	template <typename T>
	T& Vector<T>::Iterator::operator*() const
	{
		if (m_owner_vector == nullptr)
		{
			throw std::exception("Iterator doesn't belong to any Vector");
		}
		if (m_current_index == m_owner_vector->size())
		{
			throw std::exception("Invalid Operation. Vector index out of bounds.");
		}
		return m_owner_vector->m_data_array[m_current_index];
	}

#pragma endregion

#pragma region ConstIterator

	template <typename T>
	Vector<T>::ConstIterator::ConstIterator(const Iterator& t_rhs) :
		m_current_index(t_rhs.m_current_index), m_owner_vector(t_rhs.m_owner_vector)
	{
	}

	template <typename T>
	Vector<T>::ConstIterator::ConstIterator(uint32_t t_current_index, const Vector& t_owner_vector) :
		m_current_index(t_current_index), m_owner_vector(&t_owner_vector)
	{
	}

	template <typename T>
	bool Vector<T>::ConstIterator::operator==(const ConstIterator& t_rhs) const
	{
		return (m_owner_vector == t_rhs.m_owner_vector) && (m_current_index == t_rhs.m_current_index);
	}

	template <typename T>
	bool Vector<T>::ConstIterator::operator!=(const ConstIterator& t_rhs) const
	{
		return !(*this == t_rhs);
	}

	template <typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{

		if (m_owner_vector == nullptr)
		{
			throw std::exception("Iterator doesn't belong to any Vector");
		}
		if (0 == (m_owner_vector->size()))
		{
			throw std::exception("Invalid Operation! Vector doesn't have any elements. Iterator can't be incremented");
		}
		if (m_current_index > (m_owner_vector->size() - 1))
		{
			throw std::exception("Invalid Operation! Vector Index out of Bounds");
		}
		++m_current_index;
		return *this;
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator it = *this;
		operator++();
		return it;
	}

	template <typename T>
	const T& Vector<T>::ConstIterator::operator*() const
	{
		if (m_owner_vector == nullptr)
		{
			throw std::exception("Iterator doesn't belong to any Vector");
		}
		if (m_current_index == m_owner_vector->size())
		{
			throw std::exception("Invalid Operation. Vector index out of bounds.");
		}
		return m_owner_vector->m_data_array[m_current_index];
	}

#pragma endregion
}