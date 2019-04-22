#include "pch.h"
#include "IEventPublisher.h"

using TimePoint = std::chrono::high_resolution_clock::time_point;
using MilliSeconds = std::chrono::milliseconds;
using namespace std;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(IEventPublisher)

	IEventPublisher::IEventPublisher(Vector<IEventSubscriber*>& t_subscribers, std::mutex& t_mutex) : m_subscribers_list(&t_subscribers), m_mutex(&t_mutex)
	{
	}

	void IEventPublisher::setTime(const TimePoint& t_enqueued_time, MilliSeconds t_delay)
	{
		lock_guard<std::mutex> lock(*m_mutex);
		m_enqueued_time = t_enqueued_time;
		m_delay = t_delay;
	}

	const TimePoint& IEventPublisher::timeEnqueued() const
	{
		lock_guard<std::mutex> lock(*m_mutex);
		return m_enqueued_time;
	}

	const MilliSeconds& IEventPublisher::delay() const
	{
		lock_guard<std::mutex> lock(*m_mutex);
		return m_delay;
	}

	bool IEventPublisher::isExpired(const TimePoint& t_current_time) const
	{
		lock_guard<std::mutex> lock(*m_mutex);
		return (t_current_time > (m_enqueued_time + m_delay));
	}

	void IEventPublisher::deliver() const
	{
		vector<future<void>> futures;
		{
			lock_guard<std::mutex> lock(*m_mutex);
			for (auto& subscriber : *m_subscribers_list)
			{
				futures.emplace_back(async(launch::async, [subscriber, this]()
				{
					subscriber->notify(*this);
				}));
			}
		}
		
		for (auto& future : futures)
		{
			future.get();
		}
	}

	std::mutex& IEventPublisher::getMutex()
	{
		return *m_mutex;
	}
}