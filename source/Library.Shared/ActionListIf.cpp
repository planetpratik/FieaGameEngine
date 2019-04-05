#include "pch.h"
#include "ActionListIf.h"

namespace FieaGameEngine
{

	RTTI_DEFINITIONS(ActionListIf)

	ActionListIf::ActionListIf(): ActionListIf(std::string())
	{
	}

	ActionListIf::ActionListIf(const std::string& t_name) : ActionList(TypeIdClass(),t_name)
	{
	}

	void ActionListIf::update(WorldState& t_world_state)
	{
		t_world_state.action = this;
		int32_t& condition = (*this)["Condition"].get<int32_t>();
		Datum& action_if = (*this)["Then"];
		Datum& action_else = (*this)["Else"];
		
		if (condition)
		{
			assert(action_if.get<Scope*>()->Is(Action::TypeIdClass()));
			static_cast<Action*>(action_if.get<Scope*>())->update(t_world_state);
		}
		else
		{
			assert(action_else.get<Scope*>()->Is(Action::TypeIdClass()));
			static_cast<Action*>(action_else.get<Scope*>())->update(t_world_state);
		}
	}

	void ActionListIf::setCondition(const int32_t& t_condition)
	{
		m_condition_value = t_condition;
	}

	void ActionListIf::setIfBlock(Action& t_if_action)
	{
		Datum& datum = (*this)["Then"];
		if (0 == datum.size())
		{
			adopt("Then", t_if_action);
		}
		else
		{
			datum.set(&t_if_action);
		}
	}

	void ActionListIf::setElseBlock(Action& t_else_action)
	{
		Datum& datum = (*this)["Else"];
		if (0 == datum.size())
		{
			adopt("Else", t_else_action);
		}
		else
		{
			datum.set(&t_else_action);
		}
	}

	Vector<Signature> ActionListIf::Signatures()
	{
		Vector<Signature> signatures = ActionList::Signatures();
		signatures.pushBack({ "Condition", Datum::DatumType::INTEGER, 1, offsetof(ActionListIf, m_condition_value) });
		signatures.pushBack({ "Then", Datum::DatumType::TABLE, 0, 0 });
		signatures.pushBack({ "Else", Datum::DatumType::TABLE, 0, 0 });
		return signatures;
	}

	gsl::owner<Scope*> ActionListIf::clone() const
	{
		return new ActionListIf(*this);
	}

}