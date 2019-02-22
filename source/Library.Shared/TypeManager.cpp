#include "pch.h"
#include "TypeManager.h"


namespace FieaGameEngine
{
	HashMap<uint64_t, Vector<Signature>> TypeManager::type_map;

	void TypeManager::registerType(const uint64_t& t_type_id, const Vector<Signature>& signatures)
	{
		type_map.insert(std::make_pair(t_type_id, signatures));
	}

	void TypeManager::removeType(uint64_t& t_type_id)
	{
		type_map.remove(t_type_id);
	}

	Vector<Signature>& TypeManager::getSignatures(const uint64_t& t_type_id)
	{
		return type_map.at(t_type_id);
	}

	void TypeManager::clear()
	{
		type_map.clear();
	}
}