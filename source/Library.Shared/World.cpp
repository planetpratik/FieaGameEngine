#include "pch.h"
#include "World.h"
#include "Sector.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(World)

	World::World(const std::string& t_name) : Attributed(TypeIdInstance()), m_sectors_table(nullptr), m_world_name(t_name)
	{
		populate();
	}

	World::World(const World& t_rhs) : Attributed(t_rhs), m_world_name(t_rhs.m_world_name), m_sectors_table(t_rhs.m_sectors_table)
	{
		updateExternalStorage();
	}

	World::World(World&& t_rhs) : Attributed(std::move(t_rhs)), m_world_name(std::move(t_rhs.m_world_name)), m_sectors_table(std::move(t_rhs.m_sectors_table))
	{
		updateExternalStorage();
	}

	World& World::operator=(const World& t_rhs)
	{
		if (this != &t_rhs)
		{
			Attributed::operator=(t_rhs);
			m_world_name = t_rhs.m_world_name;
			m_sectors_table = t_rhs.m_sectors_table;
			updateExternalStorage();
		}
		return *this;
	}

	World& World::operator=(World&& t_rhs)
	{
		if (this != &t_rhs)
		{
			Attributed::operator=(std::move(t_rhs));
			m_world_name = std::move(t_rhs.m_world_name);
			m_sectors_table = std::move(t_rhs.m_sectors_table);
			updateExternalStorage();
		}
		return *this;
	}

	const std::string& World::name() const
	{
		return m_world_name;
	}

	void World::setName(const std::string& t_name)
	{
		m_world_name = t_name;
	}

	Datum& World::sectors()
	{
		return *find("Sectors");
	}

	Sector& World::createSector(const std::string& t_sector_name)
	{
		Sector* sector = new Sector(t_sector_name);
		sector->setWorld(this);
		return *sector;
	}

	void World::update(WorldState& t_world_state)
	{
		t_world_state.world = this;
		m_sectors_table = find("sectors");
		for (uint32_t i = 0; i < m_sectors_table->size(); ++i)
		{
			assert((*m_sectors_table)[i].Is(Sector::TypeIdClass()));
			(*m_sectors_table)[i].As<Sector>()->update(t_world_state);
		}
	}

	void World::populate()
	{
		m_sectors_table = &append("sectors");
		appendAuxillaryAttribute("sectors");
		(*this)["name"].setStorage(&m_world_name, 1);
	}

	void World::updateExternalStorage()
	{
		(*this)["name"].setStorage(&m_world_name, 1);
		*find("sectors") = *m_sectors_table;
	}

	gsl::owner<Scope*> World::clone() const
	{
		return new World(*this);
	}
}
