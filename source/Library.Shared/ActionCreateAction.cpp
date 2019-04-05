#include "pch.h"
#include "ActionCreateAction.h"
#include "ActionList.h"
#include "Entity.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionCreateAction)

	ActionCreateAction::ActionCreateAction() : ActionCreateAction(std::string())
	{
	}

	ActionCreateAction::ActionCreateAction(const std::string& t_name) : Action(TypeIdClass(),t_name)
	{
	}

	void ActionCreateAction::setClassName(const std::string& t_class_name)
	{
		m_class_name = t_class_name;
	}

	void ActionCreateAction::setInstanceName(const std::string& t_instance_name)
	{
		m_instance_name = t_instance_name;
	}

	void ActionCreateAction::update(WorldState& t_world_state)
	{
		t_world_state.action = this;
		auto parent = getParent();
		if (parent->Is(Entity::TypeIdClass()))
		{
			parent->As<Entity>()->createAction((*this)["PrototypeName"].get<std::string>(), (*this)["InstanceName"].get<std::string>());
		}
		else if (parent->Is(ActionList::TypeIdClass()))
		{
			parent->As<ActionList>()->createAction((*this)["PrototypeName"].get<std::string>(), (*this)["InstanceName"].get<std::string>());
		}
	}

	gsl::owner<Scope*> ActionCreateAction::clone() const
	{
		return new ActionCreateAction(*this);
	}

	Vector<Signature> ActionCreateAction::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();
		signatures.pushBack({ "PrototypeName", Datum::DatumType::STRING, 1, offsetof(ActionCreateAction, m_class_name) });
		signatures.pushBack({ "InstanceName", Datum::DatumType::STRING, 1, offsetof(ActionCreateAction, m_instance_name) });
		return signatures;
	}

}