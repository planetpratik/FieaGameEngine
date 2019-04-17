#include "pch.h"
#include "EventMessageAttributed.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventMessageAttributed)

	EventMessageAttributed::EventMessageAttributed() : EventMessageAttributed(std::string())
	{
	}

	EventMessageAttributed::EventMessageAttributed(const std::string& t_name) : Attributed(TypeIdClass()), m_subtype(t_name)
	{
	}

	void EventMessageAttributed::setSubType(const std::string& t_subtype)
	{
		m_subtype = t_subtype;
	}

	void EventMessageAttributed::setWorld(World& t_world)
	{
		m_world = &t_world;
	}

	const std::string& EventMessageAttributed::getSubType() const
	{
		return m_subtype;
	}

	World* EventMessageAttributed::getWorld() const
	{
		return m_world;
	}

	Vector<Signature> EventMessageAttributed::Signatures()
	{
		return Vector<Signature>
		{
			{ "SubType", Datum::DatumType::STRING, 1, offsetof(EventMessageAttributed, m_subtype) }
		};
	}

	gsl::owner<Scope*> EventMessageAttributed::clone() const
	{
		return new EventMessageAttributed(*this);
	}

}