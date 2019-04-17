#include "pch.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "ReactionAttributed.h"
#include "World.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ReactionAttributed)

	ReactionAttributed::ReactionAttributed() : ReactionAttributed(std::string())
	{
	}

	ReactionAttributed::ReactionAttributed(const std::string& t_name) : Reaction(TypeIdClass(), t_name)
	{
		Event<EventMessageAttributed>::subscribe(*this);
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::unsubscribe(*this);
	}

	void ReactionAttributed::notify(const IEventPublisher& t_event)
	{
		assert(t_event.Is(Event<EventMessageAttributed>::TypeIdClass()));
		const Event<EventMessageAttributed>* event = static_cast<const Event<EventMessageAttributed>*>(&t_event);
		const EventMessageAttributed& payload = event->message();
		auto attributes = payload.getAttributes();
		for (auto& attribute : attributes)
		{
			if (!payload.isPrescribedAttribute(attribute->first))
			{
				auto& datum = append(attribute->first);
				datum = attribute->second;
			}
		}
		if (payload.getSubType() == m_subtype)
		{
			update(payload.getWorld()->getWorldState());
		}
	}

	Vector<Signature> ReactionAttributed::Signatures()
	{
		Vector<Signature> signatures = Reaction::Signatures();
		signatures.pushBack({ "SubType", Datum::DatumType::STRING, 1, offsetof(ReactionAttributed, m_subtype) });
		return signatures;
	}

	gsl::owner<Scope*> ReactionAttributed::clone() const
	{
		return new ReactionAttributed(*this);
	}

	void ReactionAttributed::setSubtype(const std::string& t_subtype)
	{
		m_subtype = t_subtype;
	}

	const std::string& ReactionAttributed::getSubtype() const
	{
		return m_subtype;
	}
}