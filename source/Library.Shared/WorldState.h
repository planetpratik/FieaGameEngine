#pragma once
#include "GameClock.h"
#include "GameTime.h"
namespace FieaGameEngine
{
	class WorldState final
	{
	public:
		WorldState();
		WorldState(const WorldState& t_rhs) = default;
		WorldState(WorldState&& t_rhs) = default;
		WorldState& operator=(const WorldState& t_rhs) = default;
		WorldState& operator=(WorldState&& t_rhs) = default;
		~WorldState() = default;
		const GameTime& getGameTime() const;
		void setGameTime(const GameTime& t_game_time);

		class World* world = nullptr;
		class Sector* sector = nullptr;;
		class Entity* entity = nullptr;;
		class Action* action = nullptr;
	private:
		GameClock m_game_clock;
		GameTime m_game_time;
	};

}