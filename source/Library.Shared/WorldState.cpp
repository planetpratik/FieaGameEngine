#include "pch.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	WorldState::WorldState() : m_game_clock(), m_game_time(), world(nullptr), sector(nullptr), entity(nullptr)
	{
	}

	const GameTime& WorldState::getGameTime() const
	{
		return m_game_time;
	}

	void WorldState::setGameTime(const GameTime & t_game_time)
	{
		m_game_time = t_game_time;
	}


}