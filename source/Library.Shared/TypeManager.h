#pragma once
#include <cstdint>
#include "Attributed.h"

namespace FieaGameEngine
{
	class TypeManager
	{
	public:
		static void registerType(const uint32_t& t_type_id, const Vector<Signature>& signatures);
		static void removeType(uint32_t& t_type_id);
		static Vector<Signature>& getSignatures(const uint32_t& t_type_id);
		static void clear();
	private:
		static HashMap<uint32_t, Vector<Signature>> type_map;
	};
}