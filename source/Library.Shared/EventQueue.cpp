#include "pch.h"
#include "EventQueue.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	void EventQueue::enqueue(std::shared_ptr<IEventPublisher> t_event, const GameTime& t_game_time, const MilliSeconds& t_delay = MilliSeconds(0))
	{
		t_event->setTime(t_game_time.CurrentTime(), t_delay);
		m_event_queue.pushBack(t_event);
	}

	void EventQueue::send(std::shared_ptr<IEventPublisher> t_event)
	{
		t_event->deliver();
	}

	void EventQueue::update(const GameTime& t_game_time)
	{
		if (!m_event_queue.isEmpty())
		{
			// Predicate to pass into std::partition()
			auto expression = [&t_game_time](std::shared_ptr<IEventPublisher> t_event)
			{
				return t_event->isExpired(t_game_time.CurrentTime());
			};

			// Do partition as per predicate and Get first element of 2nd Partition
			// ( first partition represents all elements with condition true, 
			// whereas second partition represents all elements with condition failed.
			// Order may be different than original ).

			auto partition_element = std::partition(m_event_queue.begin(), m_event_queue.end(), expression);

			// Now iterate through expired events and call deliver() on them. Then remove them from queue.

			for (auto it = m_event_queue.begin(); it != partition_element; ++it)
			{
				(*it)->deliver();
			}

			m_event_queue.remove(m_event_queue.begin(), partition_element);
		}
		
	}

	void EventQueue::clear()
	{
		m_event_queue.clear();
	}

	bool EventQueue::isEmpty() const
	{
		return m_event_queue.isEmpty();
	}

	uint32_t EventQueue::size() const
	{
		return m_event_queue.size();
	}
}