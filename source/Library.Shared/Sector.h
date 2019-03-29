#pragma once
#include "Attributed.h"
#include "Entity.h"
#include "World.h"

namespace FieaGameEngine
{
	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed)
	public:
		explicit Sector(const std::string& t_name);
		Sector(const Sector& t_rhs);
		Sector(Sector&& t_rhs);
		Sector& operator=(const Sector& t_rhs);
		Sector& operator=(Sector&& t_rhs);
		virtual ~Sector() = default;
		const std::string& name() const;
		void setName(const std::string& t_name);
		Datum& entities();
		Entity* createEntity(const std::string& t_class_name, const std::string& t_instance_name);
		World* getWorld() const;
		void setWorld(World* t_world);
		void update(WorldState& t_world_state);
		void populate();
		void updateExternalStorage();
		virtual gsl::owner<Scope*> clone() const override;
	private:
		std::string m_sector_name;
		Datum* m_entities_table;
	};

}
