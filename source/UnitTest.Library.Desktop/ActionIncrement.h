#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	class ActionIncrement : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action)
	public:
		ActionIncrement();
		ActionIncrement(const ActionIncrement& t_rhs) = default;
		ActionIncrement(ActionIncrement&& t_rhs) = default;
		ActionIncrement& operator=(const ActionIncrement& t_rhs) = default;
		ActionIncrement& operator=(ActionIncrement&& t_rhs) = default;
		virtual ~ActionIncrement() = default;
		virtual void update(WorldState& t_world_state) override;
		static Vector<Signature> Signatures();
		virtual gsl::owner<Scope*> clone() const override;
	private:
		std::string m_target;
	};

	ConcreteFactory(ActionIncrement, Action)
}