#pragma once
#include "GameClock.h"
#include "GameTime.h"
namespace FieaGameEngine
{
	class WorldState final
	{
	public:
		WorldState();
		WorldState(const WorldState& t_rhs) = delete;
		WorldState(WorldState&& t_rhs) = default;
		WorldState& operator=(const WorldState& t_rhs) = delete;
		WorldState& operator=(WorldState&& t_rhs) = default;
		~WorldState() = default;
		const GameTime& getGameTime() const;
		void setGameTime(const GameTime& t_game_time);

		class World* world;
		class Sector* sector;
		class Entity* entity;
	private:
		GameClock m_game_clock;
		GameTime m_game_time;
	};

}