#include "pch.h"
#include "ActionList.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionList)

	ActionList::ActionList() : ActionList(std::string())
	{
	}

	ActionList::ActionList(const std::string& t_name) : Action(TypeIdClass(), t_name)
	{
	}

	void ActionList::update(WorldState& t_world_state)
	{
		t_world_state.action = this;
		Datum& t_actions = actions();
		for (uint32_t i = 0; i < t_actions.size(); ++i)
		{
			Scope& t_action_scope = t_actions[i];
			assert(t_action_scope.Is(Action::TypeIdClass()));
			Action* t_action = static_cast<Action*>(&t_action_scope);
			t_action->update(t_world_state);
		}
	}

	Action* ActionList::createAction(const std::string& t_class_name, const std::string& t_instance_name)
	{
		Action* action = Factory<Action>::create(t_class_name)->As<Action>();
		if (action == nullptr)
		{
			return nullptr;
		}
		action->setName(t_instance_name);
		adopt("Actions", *action);
		return action;
	}

	Datum& ActionList::actions()
	{
		return (*this)["Actions"];
	}

	Vector<Signature> ActionList::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();
		signatures.pushBack({ "Actions", Datum::DatumType::TABLE, 0, 0 });
		return signatures;
	}

	gsl::owner<Scope*> ActionList::clone() const
	{
		return new ActionList(*this);;
	}

	ActionList::ActionList(const uint64_t& t_type_id, const std::string& t_name) : Action(t_type_id,t_name)
	{
	}

}