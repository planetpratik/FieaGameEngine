#pragma once
#include <cstdint>
#include <string>

namespace FieaGameEngine
{
	template <typename TKey>
	/// <summary>DefaultHash Functor declaration</summary>
	class DefaultHash
	{
	public:
		/// <summary>Functor Operator () overload for Generic Keys of Type TKey </summary>
		/// <param name="t_key">Const Reference to key of type TKey</param>
		/// <returns>Hashed value in Unsigned Int ( 32-bit )</returns>
		uint32_t operator()(const TKey& t_key) const;
	};

	template <>
	/// <summary>Template Specialization for DefaultHash. (Unsigned Integer)</summary>
	class DefaultHash<uint32_t>
	{
	public:
		/// <summary>Functor Operator () overload for Keys of Type unsigned integer</summary>
		/// <param name="t_key">Const Reference to key of type uint32_t</param>
		/// <returns>Hashed value in Unsigned Int ( 32-bit )</returns>
		uint32_t operator()(const uint32_t& t_key) const;
	};

	template <>
	/// <summary>Template Specialization for DefaultHash. (String)</summary>
	class DefaultHash<std::string>
	{
	public:
		/// <summary>Functor Operator () overload for Keys of Type string</summary>
		/// <param name="t_key">Const Reference to key of type std::string</param>
		/// <returns>Hashed value in Unsigned Int ( 32-bit )</returns>
		uint32_t operator()(const std::string& t_key) const;
	};

	template <>
	/// <summary>Template Specialization for DefaultHash. (char*)</summary>
	class DefaultHash<char*>
	{
	public:
		/// <summary>Functor Operator () overload for Keys of Type char*</summary>
		/// <param name="t_key">Const pointer to key of type char</param>
		/// <returns>Hashed value in Unsigned Int ( 32-bit )</returns>
		uint32_t operator()(const char* t_key) const;
	};
}

#include "DefaultHash.inl"