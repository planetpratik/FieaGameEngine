#include "pch.h"
#include "DefaultHash.h"

namespace FieaGameEngine
{
	template<typename TKey>
	inline uint32_t DefaultHash<TKey>::operator()(const TKey& t_key) const
	{
		uint32_t hash = 0;
		const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&t_key);
		for (uint32_t i = 0; i < sizeof(t_key); ++i)
		{
			hash ^= bytes[i];
		}
		return hash;
	}

	inline uint32_t DefaultHash<uint32_t>::operator()(const uint32_t& t_key) const
	{
		uint32_t hash = 0;
		uint32_t temp = t_key;
		while (temp != 0)
		{
			hash += temp % 10;
			temp /= 10;
		}
		return hash;
	}


	inline uint32_t DefaultHash<std::string>::operator()(const std::string& t_key) const
	{
		uint32_t hash = 0;
		for (uint32_t i = 0; i < t_key.length(); ++i)
		{
			hash += static_cast<uint32_t>(t_key.length() + 2U) * static_cast<uint32_t>(t_key.at(i));
		}
		return hash;
	}

	inline uint32_t DefaultHash<char*>::operator()(const char* t_key) const
	{
		uint32_t hash = 0;
		while (*(t_key) != '\0')
		{
			hash += static_cast<uint32_t>(*t_key);
			t_key += 1;
		}
		return hash;
	}
}