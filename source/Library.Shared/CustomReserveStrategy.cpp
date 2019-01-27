#include "pch.h"
#include "CustomReserveStrategy.h"


namespace FieaGameEngine
{
	uint32_t CustomReserveStrategy::operator()(uint32_t t_size, uint32_t t_capacity) const
	{
		if (t_size == 0 && t_capacity == 0)
		{
			return 5;
		}
		return 2 * t_capacity;
	}
}