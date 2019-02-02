#pragma once
#include <cstdint>
#include <string>

namespace FieaGameEngine
{
	template <typename TKey>
	class DefaultHash
	{
	public:
		uint32_t operator()(const TKey& t_key) const;
	};

	template <>
	class DefaultHash<uint32_t>
	{
	public:
		uint32_t operator()(const uint32_t& t_key) const;
	};

	template <>
	class DefaultHash<std::string>
	{
	public:
		uint32_t operator()(const std::string& t_key) const;
	};

	template <>
	class DefaultHash<char*>
	{
	public:
		uint32_t operator()(const char* t_key) const;
	};
}

#include "DefaultHash.inl"