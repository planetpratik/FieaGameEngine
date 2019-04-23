#include "pch.h"
#include "UnsubscriberFoo.h"
#include "Foo.h"
#include "Event.h"

namespace FieaGameEngine
{

	UnsubscriberFoo::UnsubscriberFoo() : m_value(0)
	{
	}

	int32_t UnsubscriberFoo::data() const
	{
		return m_value;
	}

	void UnsubscriberFoo::notify(const IEventPublisher& t_event)
	{
		Event<Foo>* foo = t_event.As<Event<Foo>>();
		if (foo)
		{
			Event<Foo>::unsubscribe(*this);
			m_value = foo->message().data();
		}
	}

	void UnsubscriberFoo::setData(const int& t_value)
	{
		m_value = t_value;
	}


}