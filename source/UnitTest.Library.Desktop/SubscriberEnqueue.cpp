#include "pch.h"
#include "SubscriberEnqueue.h"
#include "Foo.h"
#include "Event.h"

namespace FieaGameEngine
{

	SubscriberEnqueue::SubscriberEnqueue(EventQueue& t_event_queue, GameTime& t_game_time) : m_value(0), m_event_queue(&t_event_queue), m_game_time(&t_game_time)
	{
	}

	void SubscriberEnqueue::notify(const IEventPublisher& t_event)
	{
		Event<Foo>* foo = t_event.As<Event<Foo>>();
		if (foo)
		{
			Foo temp(200);
			std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(temp);
			m_event_queue->enqueue(event, *m_game_time, std::chrono::milliseconds(0));
			m_value = foo->message().data();
		}
	}

	int32_t SubscriberEnqueue::data() const
	{
		return m_value;
	}

	void SubscriberEnqueue::setData(const int& t_value)
	{
		m_value = t_value;
	}

}