#include "pch.h"
#include "IEventPublisher.h"

using TimePoint = std::chrono::high_resolution_clock::time_point;
using MilliSeconds = std::chrono::milliseconds;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(IEventPublisher)

	IEventPublisher::IEventPublisher(Vector<IEventSubscriber*>& t_subscribers) : m_subscribers_list(&t_subscribers)
	{
	}

	void IEventPublisher::setTime(const TimePoint& t_enqueued_time, MilliSeconds t_delay)
	{
		m_enqueued_time = t_enqueued_time;
		m_delay = t_delay;
	}

	const TimePoint& IEventPublisher::timeEnqueued() const
	{
		return m_enqueued_time;
	}

	const MilliSeconds& IEventPublisher::delay() const
	{
		return m_delay;
	}

	bool IEventPublisher::isExpired(const TimePoint& t_current_time) const
	{
		return (t_current_time > (m_enqueued_time + m_delay));
	}

	void IEventPublisher::deliver() const
	{
		for (auto& subscriber : *m_subscribers_list)
		{
			subscriber->notify(*this);
		}
	}
}