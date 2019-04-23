#pragma once
#include "IEventSubscriber.h"
#include "GameTime.h"
#include "EventQueue.h"
namespace FieaGameEngine
{
	class SubscriberEnqueue final: public IEventSubscriber
	{
	public:
		SubscriberEnqueue(EventQueue& t_event_queue, GameTime& t_game_time);
		~SubscriberEnqueue() = default;
		SubscriberEnqueue(const SubscriberEnqueue& t_rhs) = default;
		SubscriberEnqueue(SubscriberEnqueue&& t_rhs) = default;
		SubscriberEnqueue& operator=(const SubscriberEnqueue& t_rhs) = default;
		SubscriberEnqueue& operator=(SubscriberEnqueue&& t_rhs) = default;
		virtual void notify(const IEventPublisher& t_event) override;
		int32_t data() const;
		void setData(const int & t_value);
	private:
		int32_t m_value;
		EventQueue* m_event_queue;
		GameTime* m_game_time;
	};

}
