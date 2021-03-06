#include "pch.h"
#include "Reaction.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Reaction)

	Vector<Signature> Reaction::Signatures()
	{
		return ActionList::Signatures();
	}

	Reaction::Reaction(const uint64_t& t_type_id, const std::string& t_name) : ActionList(t_type_id, t_name)
	{
	}

}