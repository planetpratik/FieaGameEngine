#include "pch.h"
#include "SubscriberFoo.h"
#include "Event.h"
#include "Foo.h"

namespace FieaGameEngine
{

	SubscriberFoo::SubscriberFoo() : m_value(100)
	{
	}

	int32_t SubscriberFoo::data() const
	{
		return m_value;
	}

	void SubscriberFoo::notify(const IEventPublisher& t_event)
	{
		Event<Foo>* foo = t_event.As<Event<Foo>>();
		if (foo)
		{
			m_value = foo->message().data();
		}
	}

	void SubscriberFoo::setData(const int& t_value)
	{
		m_value = t_value;
	}

}
