#pragma once
#include "IEventSubscriber.h"
#include "IEventPublisher.h"

namespace FieaGameEngine
{
	class SubscriberFoo final : public IEventSubscriber
	{
	public:
		SubscriberFoo();
		~SubscriberFoo() = default;
		SubscriberFoo(const SubscriberFoo& t_rhs) = default;
		SubscriberFoo(SubscriberFoo&& t_rhs) = default;
		SubscriberFoo& operator=(const SubscriberFoo& t_rhs) = default;
		SubscriberFoo& operator=(SubscriberFoo&& t_rhs) = default;
		int32_t data() const;
		virtual void notify(const IEventPublisher& t_event) override;
		void setData(const int& t_value);
	private:
		int32_t m_value;
	};


}