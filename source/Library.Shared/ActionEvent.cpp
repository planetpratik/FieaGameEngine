#include "pch.h"
#include "ActionEvent.h"
#include "EventMessageAttributed.h"
#include "Event.h"
#include "World.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionEvent)

	ActionEvent::ActionEvent() : ActionEvent(std::string())
	{
	}

	ActionEvent::ActionEvent(const std::string& t_name) : Action(TypeIdClass(),t_name)
	{
	}

	void ActionEvent::update(WorldState& t_world_state)
	{
		t_world_state.action = this;
		EventMessageAttributed event;
		event.setWorld(*t_world_state.world);
		event.setSubType(m_subtype);
		auto attributes = getAttributes();
		for (auto& attribute : attributes)
		{
			if (!isPrescribedAttribute(attribute->first))
			{
				event.appendAuxillaryAttribute(attribute->first) = attribute->second;
			}
		}
		std::shared_ptr<Event<EventMessageAttributed>> event_ptr = std::make_shared<Event<EventMessageAttributed>>(event);
		t_world_state.world->getEventQueue().enqueue(event_ptr, t_world_state.getGameTime(), MilliSeconds(m_delay));
	}

	gsl::owner<Scope*> ActionEvent::clone() const
	{
		return new ActionEvent(*this);
	}

	Vector<Signature> ActionEvent::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();
		signatures.pushBack({ "SubType", Datum::DatumType::STRING, 1, offsetof(ActionEvent, m_subtype) });
		signatures.pushBack({ "Delay", Datum::DatumType::INTEGER, 1, offsetof(ActionEvent, m_delay) });
		return signatures;
	}

	void ActionEvent::setDelay(const std::chrono::milliseconds & t_milliseconds)
	{
		m_delay = static_cast<int32_t>(t_milliseconds.count());
	}

	void ActionEvent::setSubtype(const std::string & t_subtype)
	{
		m_subtype = t_subtype;
	}

	const std::chrono::milliseconds ActionEvent::getDelay() const
	{
		return std::chrono::milliseconds(m_delay);
	}

	const std::string& ActionEvent::getSubtype() const
	{
		return m_subtype;
	}
}