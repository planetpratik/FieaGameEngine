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
	Event<T>::Event(T& t_payload) : IEventPublisher(m_subscribers_list), payload(t_payload)
	{
	}

	template<typename T>
	inline void Event<T>::subscribe(IEventSubscriber& t_subscriber)
	{
		m_subscribers_list.pushBack(&t_subscriber);
	}

	template<typename T>
	inline void Event<T>::unsubscribe(IEventSubscriber& t_subscriber)
	{
		m_subscribers_list.remove(&t_subscriber);
	}

	template<typename T>
	inline void Event<T>::unsubscribeAll()
	{
		m_subscribers_list.erase();
	}

	template<typename T>
	inline const T& Event<T>::message() const
	{
		return payload;
	}
}