#pragma once
#include "Attributed.h"
#include "WorldState.h"
namespace FieaGameEngine
{
	class Sector;

	class World final: public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed)
	public:
		World(const std::string& t_name);
		World(const World& t_rhs);
		World(World&& t_rhs);
		World& operator=(const World& t_rhs);
		World& operator=(World&& t_rhs);
		virtual ~World() = default;
		const std::string& name() const;
		void setName(const std::string& t_name);
		Datum& sectors();
		Sector& createSector(const std::string& t_sector_name);
		void update(WorldState& t_world_state);
		void populate();
		void updateExternalStorage();
		virtual gsl::owner<Scope*> clone() const override;
	private:
		std::string m_world_name;
		Datum* m_sectors_table;
	};
}
