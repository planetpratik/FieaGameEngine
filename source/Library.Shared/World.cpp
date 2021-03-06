#include "pch.h"
#include "World.h"
#include "Sector.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(World)

	World::World() : Attributed(TypeIdClass())
	{
	}

	World::World(const std::string& t_name) : Attributed(TypeIdClass()), m_world_name(t_name)
	{
	}

	World::World(const std::string& t_name, EventQueue& t_event_queue) : Attributed(TypeIdClass()), m_world_name(t_name), m_event_queue(&t_event_queue)
	{
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
		return (*this)[sectors_index];
	}

	Sector* World::createSector(const std::string& t_sector_name)
	{
		Sector* sector = new Sector(t_sector_name);
		sector->setWorld(this);
		return sector;
	}

	void World::update(WorldState& t_world_state)
	{
		t_world_state.world = this;
		m_world_state = &t_world_state;
		if (m_event_queue != nullptr)
		{
			m_event_queue->update(m_world_state->getGameTime());
		}

		Datum& t_sectors = sectors();
		for (uint32_t i = 0; i < t_sectors.size(); ++i)
		{
			Scope& t_sector_scope = t_sectors[i];
			assert(t_sector_scope.Is(Sector::TypeIdClass()));
			Sector& t_sector = static_cast<Sector&>(t_sector_scope);
			t_world_state.sector = &t_sector;
			t_sector.update(t_world_state);
		}

		t_world_state.sector = nullptr;
	}

	gsl::owner<Scope*> World::clone() const
	{
		return new World(*this);
	}

	Vector<Signature> World::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumType::STRING, 1, offsetof(World, m_world_name) },
			{ "Sectors", Datum::DatumType::TABLE, 0, 0 }
		};
	}

	WorldState& World::getWorldState()
	{
		return *m_world_state;
	}

	EventQueue& World::getEventQueue()
	{
		return *m_event_queue;
	}

	Datum& World::reactions()
	{
		return (*this)["Reactions"];
	}

	Reaction* World::createReaction(const std::string& t_class_name)
	{
		Reaction* reaction = Factory<Scope>::create(t_class_name)->As<Reaction>();
		if (nullptr != reaction)
		{
			adopt("Reactions", *reaction);
		}
		return reaction;
	}
}
