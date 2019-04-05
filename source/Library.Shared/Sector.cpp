#include "pch.h"
#include "Sector.h"
#include "Factory.h"
#include "World.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Sector)

	Sector::Sector() : Sector(std::string())
	{
	}

	Sector::Sector(const std::string& t_name) : Attributed(TypeIdClass()), m_sector_name(t_name)
	{
	}

	const std::string& Sector::name() const
	{
		return m_sector_name;
	}

	void Sector::setName(const std::string& t_name)
	{
		m_sector_name = t_name;
	}

	Datum& Sector::entities()
	{
		return (*this)[entities_index];
	}

	Entity* Sector::createEntity(const std::string& t_class_name, const std::string& t_instance_name)
	{
		Entity* entity = Factory<Scope>::create(t_class_name)->As<Entity>();
		if (entity == nullptr)
		{
			return nullptr;
		}
		entity->setName(t_instance_name);
		entity->setSector(this);
		return entity;
	}

	World* Sector::getWorld() const
	{
		assert(getParent()->Is(World::TypeIdClass()));
		return static_cast<World*>(getParent());
	}

	void Sector::setWorld(World* t_world)
	{
		if (t_world != nullptr)
		{
			t_world->adopt("Sectors", *this);
		}
	}

	void Sector::update(WorldState& t_world_state)
	{
		t_world_state.sector = this;
		Datum& t_entities = entities();
		for (uint32_t i = 0; i < t_entities.size(); ++i)
		{
			Scope& t_entity_scope = t_entities[i];
			assert(t_entity_scope.Is(Entity::TypeIdClass()));
			Entity& t_entity = static_cast<Entity&>(t_entity_scope);
			t_world_state.entity = &t_entity;
			t_entity.update(t_world_state);
		}
		t_world_state.entity = nullptr;
	}

	gsl::owner<Scope*> Sector::clone() const
	{
		return new Sector(*this);
	}

	Vector<Signature> Sector::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumType::STRING, 1, offsetof(Sector, m_sector_name) },
			{ "Entities", Datum::DatumType::TABLE, 0, 0 }
		};
	}
}