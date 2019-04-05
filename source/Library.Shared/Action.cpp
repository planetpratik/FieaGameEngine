#include "pch.h"
#include "Action.h"
#include "Entity.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action)

	const std::string & Action::name() const
	{
		return m_action_name;
	}

	void Action::setName(const std::string& t_name)
	{
		m_action_name = t_name;
	}

	Entity* Action::getEntity() const
	{
		assert(getParent()->Is(Entity::TypeIdClass()));
		return static_cast<Entity*>(getParent());
	}

	void Action::setEntity(Entity * t_entity)
	{
		if (t_entity != nullptr)
		{
			t_entity->adopt("Actions", *this);
		}
	}

	Vector<Signature> Action::Signatures()
	{
		return Vector<Signature>
		{
			{"Name", Datum::DatumType::STRING, 1, offsetof(Action, m_action_name)}
		};
	}

	Action::Action(const uint64_t& t_type_id, const std::string& t_name) : Attributed(t_type_id), m_action_name(t_name)
	{
	}

}