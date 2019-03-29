#include "pch.h"
#include "Entity.h"
#include "Sector.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Entity)

	Entity::Entity(const std::string& t_name) : Attributed(TypeIdClass()), m_entity_name(t_name)
	{
		populate();
	}

	Entity::Entity(const Entity& t_rhs) : Attributed(t_rhs), m_entity_name(t_rhs.m_entity_name)
	{
		updateExternalStorage();
	}

	Entity::Entity(Entity&& t_rhs) : Attributed(std::move(t_rhs)), m_entity_name(std::move(t_rhs.m_entity_name))
	{
		updateExternalStorage();
	}

	Entity& Entity::operator=(const Entity& t_rhs)
	{
		if (this != &t_rhs)
		{
			Attributed::operator=(t_rhs);
			m_entity_name = t_rhs.m_entity_name;
			updateExternalStorage();
		}
		return *this;
	}

	Entity& Entity::operator=(Entity&& t_rhs) 
	{
		if (this != &t_rhs)
		{
			Attributed::operator=(std::move(t_rhs));
			m_entity_name = std::move(t_rhs.m_entity_name);
			updateExternalStorage();
		}
		return *this;
	}

	const std::string & Entity::name() const
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
			t_sector->adopt("entities", *this);
		}
	}

	void Entity::update(WorldState& t_world_state)
	{
		t_world_state;
		// Does nothing for now.
	}

	void Entity::populate()
	{
		(*this)["name"].setStorage(&m_entity_name, 1);
	}

	void Entity::updateExternalStorage()
	{
		(*this)["name"].setStorage(&m_entity_name, 1);
	}

	gsl::owner<Scope*> Entity::clone() const
	{
		return new Entity(*this);
	}

}