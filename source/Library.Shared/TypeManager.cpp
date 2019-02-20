#include "TypeManager.h"


namespace FieaGameEngine
{
	void TypeManager::registerType(const uint32_t& t_type_id, const Vector<Signature>& signatures)
	{
		type_map.insert(std::make_pair(t_type_id, signatures));
	}

	void TypeManager::removeType(uint32_t& t_type_id)
	{
		type_map.remove(t_type_id);
	}

	Vector<Signature>& TypeManager::getSignatures(const uint32_t& t_type_id)
	{
		return type_map.at(t_type_id);
	}

	void TypeManager::clear()
	{
		type_map.clear();
	}

}