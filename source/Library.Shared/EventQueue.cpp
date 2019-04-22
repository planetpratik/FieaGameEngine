#include "pch.h"
#include "EventQueue.h"
#include "GameTime.h"
#include <future>

namespace FieaGameEngine
{
	void EventQueue::enqueue(std::shared_ptr<IEventPublisher> t_event, const GameTime& t_game_time, const MilliSeconds& t_delay = MilliSeconds(0))
	{
		t_event->setTime(t_game_time.CurrentTime(), t_delay);
		std::lock_guard<std::mutex> lock(m_mutex);
		m_event_queue.push_back(t_event);
	}

	void EventQueue::send(std::shared_ptr<IEventPublisher> t_event)
	{
		t_event->deliver();
	}

	void EventQueue::update(const GameTime& t_game_time)
	{
		if (!m_event_queue.empty())
		{
			// Predicate to pass into std::partition()
			auto expression = [&t_game_time](std::shared_ptr<IEventPublisher> t_event)
			{
				return !t_event->isExpired(t_game_time.CurrentTime());
			};

			// Do partition as per predicate and Get first element of 2nd Partition
			// ( first partition represents all elements with condition true, 
			// whereas second partition represents all elements with condition failed.
			// Order may be different than original ).

			auto partition_element = std::partition(m_event_queue.begin(), m_event_queue.end(), expression);

			std::move(partition_element, m_event_queue.end(), std::back_inserter(m_expired_event_queue));
			m_event_queue.erase(partition_element, m_event_queue.end());
			std::vector<std::future<void>> futures;

			// Now iterate through expired events and call deliver() on them. Then remove them from queue.
			for (auto& expired_event : m_expired_event_queue)
			{
				futures.emplace_back(std::async(std::launch::async, [expired_event]()
				{
					expired_event->deliver();
				}));
			}

			for (auto& future : futures)
			{
				future.get();
			}

			m_expired_event_queue.erase(m_expired_event_queue.begin(), m_expired_event_queue.end());
		}
		
	}

	void EventQueue::clear()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_event_queue.clear();
	}

	bool EventQueue::isEmpty() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_event_queue.empty();
	}

	uint32_t EventQueue::size() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return static_cast<uint32_t>(m_event_queue.size());
	}
}