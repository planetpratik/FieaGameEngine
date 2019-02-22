#pragma once
#include <cstdint>
#include "Datum.h"
#include "HashMap.h"

namespace FieaGameEngine
{
	struct Signature
	{
		std::string sig_name;
		Datum::DatumType sig_type;
		uint32_t sig_size;
		size_t sig_offset;

		bool operator==(const Signature& rhs)
		{
			return sig_name == rhs.sig_name &&
				sig_type == sig_type &&
				sig_size == sig_size &&
				sig_offset == sig_offset;
		}

		bool operator!=(const Signature& rhs)
		{
			return !(operator==(rhs));
		}
	};

	class TypeManager
	{
	public:
		TypeManager() = delete;
		TypeManager(const TypeManager& t_rhs) = delete;
		TypeManager(TypeManager&& t_rhs) = delete;
		TypeManager& operator=(const TypeManager& t_rhs) = delete;
		TypeManager& operator=(TypeManager&& t_rhs) = delete;
		
		static void registerType(const uint64_t& t_type_id, const Vector<Signature>& signatures);
		static void removeType(uint64_t& t_type_id);
		static Vector<Signature>& getSignatures(const uint64_t& t_type_id);
		static void clear();
		
	private:
		static HashMap<uint64_t, Vector<Signature>> type_map;
	};
}