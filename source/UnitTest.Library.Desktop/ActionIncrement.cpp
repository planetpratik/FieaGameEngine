#include "pch.h"
#include "ActionIncrement.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionIncrement)

	ActionIncrement::ActionIncrement() : Action(TypeIdClass(), std::string())
	{
	}

	void ActionIncrement::update(WorldState& t_world_state)
	{
		t_world_state.action = this;
		Datum& datum = (*this)["Target"];
		Datum* target = find(datum.get<std::string>());
		if (nullptr != target && (Datum::DatumType::INTEGER == target->type()))
		{
			++(target->get<int32_t>());
		}
	}

	Vector<Signature> ActionIncrement::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();
		signatures.pushBack({ "Target", Datum::DatumType::STRING, 1, offsetof(ActionIncrement, m_target) });
		return signatures;
	}

	gsl::owner<Scope*> ActionIncrement::clone() const
	{
		return new ActionIncrement(*this);
	}

}