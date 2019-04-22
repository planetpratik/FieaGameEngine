#pragma once
#include "Event.h"
#include "IEventSubscriber.h"

namespace FieaGameEngine
{
	template<typename T>
	RTTI_DEFINITIONS(Event<T>)

	template<typename T>
	Vector<IEventSubscriber*> Event<T>::m_subscribers_list;

	template<typename T>
	std::mutex Event<T>::m_mutex;

	template<typename T>
	Event<T>::Event(T& t_payload) : IEventPublisher(m_subscribers_list, m_mutex), payload(t_payload)
	{
	}

	template<typename T>
	inline void Event<T>::subscribe(IEventSubscriber& t_subscriber)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_subscribers_list.pushBack(&t_subscriber);
	}

	template<typename T>
	inline void Event<T>::unsubscribe(IEventSubscriber& t_subscriber)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_subscribers_list.remove(&t_subscriber);
	}

	template<typename T>
	inline void Event<T>::unsubscribeAll()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_subscribers_list.erase();
	}

	template<typename T>
	inline const T& Event<T>::message() const
	{
		return payload;
	}

	template<typename T>
	inline uint32_t& Event<T>::getTotalSubscribers() const
	{
		return m_subscribers_list.size();
	}

	template<typename T>
	inline T& Event<T>::getMutex() const
	{
		return m_mutex;
	}
}