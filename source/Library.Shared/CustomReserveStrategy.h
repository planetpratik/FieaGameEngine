#pragma once
#include "Vector.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Custom Reserve Strategy class to override default memory reserve strategy required for dynamically growing containers.
	/// </summary>
	class CustomReserveStrategy : public ReserveStrategy
	{
	public:
		/// <summary>
		/// Functor ( Function Object ) to supply default increment strategy for dynamic growing containers</summary>
		/// <param name="t_size">Current Size of the given container in Unsigned int (32-bit)</param>
		/// <param name="t_capacity">Current Capacity of the given container in Unsigned int (32-bit).</param>
		/// <returns>Returns new capacity of the Container</returns>
		uint32_t operator()(uint32_t t_size, uint32_t t_capacity) const;
	};
}

