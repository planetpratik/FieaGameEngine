#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "Action.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Entity)

	Entity::Entity() : Entity(std::string())
	{
	}

	Entity::Entity(const std::string& t_name) : Attributed(TypeIdClass()), m_entity_name(t_name)
	{
	}

	const std::string& Entity::name() const
	{
		return m_entity_name;
	}

	void Entity::setName(const std::string & t_name)
	{
		m_entity_name = t_name;
	}

	Sector* Entity::getSector() const
	{
		assert(getParent()->Is(Sector::TypeIdClass()));
		return static_cast<Sector*>(getParent());
	}

	void Entity::setSector(Sector* t_sector)
	{
		if (t_sector != nullptr)
		{
			t_sector->adopt("Entities", *this);
		}
	}

	Datum& Entity::actions()
	{
		return (*this)["Actions"];
	}

	Action* Entity::createAction(const std::string& t_class_name, const std::string& t_instance_name)
	{
		Action* t_action = Factory<Scope>::create(t_class_name)->As<Action>();
		if (t_action == nullptr)
		{
			return nullptr;
		}
		t_action->setName(t_instance_name);
		t_action->setEntity(this);
		return t_action;
	}

	void Entity::update(WorldState& t_world_state)
	{
		t_world_state.entity = this;
		Datum& t_actions = actions();
		for (uint32_t i = 0; i < t_actions.size(); ++i)
		{
			Scope& t_action_scope = t_actions[i];
			assert(t_action_scope.Is(Action::TypeIdClass()));
			Action& t_action = static_cast<Action&>(t_action_scope);
			t_world_state.action = &t_action;
			t_action.update(t_world_state);
		}
		t_world_state.action = nullptr;
	}

	gsl::owner<Scope*> Entity::clone() const
	{
		return new Entity(*this);
	}

	Vector<Signature> Entity::Signatures()
	{
		return Vector<Signature>
		{
			{"Name", Datum::DatumType::STRING, 1, offsetof(Entity, m_entity_name)},
			{"Actions", Datum::DatumType::TABLE, 0, 0}
		};
	}

}