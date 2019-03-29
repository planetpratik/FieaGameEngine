#pragma once
#include "Attributed.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	class Sector;

	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)
	public:
		explicit Entity(const std::string& t_name);
		Entity(const Entity& t_rhs);
		Entity(Entity&& t_rhs);
		Entity& operator=(const Entity& t_rhs);
		Entity& operator=(Entity&& t_rhs);
		virtual ~Entity() = default;
		const std::string& name() const;
		void setName(const std::string& t_name);
		Sector* getSector() const;
		void setSector(Sector* t_sector);
		void update(WorldState& t_world_state);
		void populate();
		void updateExternalStorage();
		virtual gsl::owner<Scope*> clone() const override;
	private:
		std::string m_entity_name;
	};
}

