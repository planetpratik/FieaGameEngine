#include "pch.h"
#include "Sector.h"
#include "Factory.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Sector)

	Sector::Sector() : Attributed(TypeIdInstance())
	{
	}

	Sector::Sector(const std::string& t_name) : Attributed(TypeIdInstance()), m_entities_table(nullptr),  m_sector_name(t_name)
	{
		populate();
	}

	Sector::Sector(const Sector& t_rhs) : Attributed(t_rhs), m_sector_name(t_rhs.m_sector_name), m_entities_table(t_rhs.m_entities_table)
	{
		updateExternalStorage();
	}

	Sector::Sector(Sector&& t_rhs) : Attributed(std::move(t_rhs)), m_sector_name(std::move(t_rhs.m_sector_name)), m_entities_table(std::move(t_rhs.m_entities_table))
	{
		updateExternalStorage();
	}

	Sector& Sector::operator=(const Sector& t_rhs)
	{
		if (this != &t_rhs)
		{
			Attributed::operator=(t_rhs);
			m_sector_name = t_rhs.m_sector_name;
			m_entities_table = t_rhs.m_entities_table;
			updateExternalStorage();
		}
		return *this;
	}

	Sector& Sector::operator=(Sector&& t_rhs)
	{
		if (this != &t_rhs)
		{
			Attributed::operator=(std::move(t_rhs));
			m_sector_name = std::move(t_rhs.m_sector_name);
			m_entities_table = std::move(t_rhs.m_entities_table);
			updateExternalStorage();
		}
		return *this;
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
		assert(m_entities_table != nullptr);
		return *m_entities_table;
	}

	Entity* Sector::createEntity(const std::string& t_class_name, const std::string& t_instance_name)
	{
		Entity* entity = Factory<Entity>::create(t_class_name)->As<Entity>();
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
			t_world->adopt("sectors", *this);
		}
	}

	void Sector::update(WorldState& t_world_state)
	{
		t_world_state.sector = this;
		m_entities_table = find("entities");
		for (uint32_t i = 0; i < m_entities_table->size(); ++i)
		{
			assert((*m_entities_table)[i].Is(Entity::TypeIdClass()));
			(*m_entities_table)[i].As<Entity>()->update(t_world_state);
		}
	}

	void Sector::populate()
	{
		m_entities_table = &append("entities");
		(*this)["name"].setStorage(&m_sector_name, 1);
	}

	void Sector::updateExternalStorage()
	{
		(*this)["name"].setStorage(&m_sector_name, 1);
	}

	gsl::owner<Scope*> Sector::clone() const
	{
		return new Sector(*this);
	}

	Vector<Signature> Sector::Signatures()
	{
		return Vector<Signature>
		{
			{ "name", Datum::DatumType::STRING, 1, offsetof(Sector, m_sector_name) },
			{ "entities", Datum::DatumType::TABLE, 1, offsetof(Sector, m_entities_table) }
		};
	}
}