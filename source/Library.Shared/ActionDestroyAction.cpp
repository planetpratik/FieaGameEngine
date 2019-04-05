#include "pch.h"
#include "ActionDestroyAction.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionDestroyAction)

	ActionDestroyAction::ActionDestroyAction() : ActionDestroyAction(std::string())
	{
	}

	ActionDestroyAction::ActionDestroyAction(const std::string& t_name) : Action(TypeIdClass(),t_name), m_action_instance_name(t_name)
	{
	}

	void ActionDestroyAction::update(WorldState & t_world_state)
	{
		t_world_state.action = this;
		if (!m_action_instance_name.empty())
		{
			bool found = false;
			Scope* current_scope = this;
			Datum* datum = current_scope->search("Actions");
			while (nullptr != datum && false == found)
			{
				for (uint32_t i = 0; i < datum->size(); ++i)
				{
					Scope* action_scope = datum->get<Scope*>(i);
					if ((*action_scope)["Name"].get<std::string>() == m_action_instance_name)
					{
						// Implement Graveyard Concept
						found = true;
						break;
					}
				}
				current_scope = current_scope->getParent();
				datum = current_scope->find("Actions");
			}
		}
	}

	void ActionDestroyAction::setActionInstanceName(const std::string& t_instance_name)
	{
		m_action_instance_name = t_instance_name;
	}

	gsl::owner<Scope*> ActionDestroyAction::clone() const
	{
		return new ActionDestroyAction(*this);
	}

	Vector<Signature> ActionDestroyAction::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();
		signatures.pushBack({ "ActionInstanceName", Datum::DatumType::STRING, 1, offsetof(ActionDestroyAction, m_action_instance_name) });
		return signatures;
	}


}