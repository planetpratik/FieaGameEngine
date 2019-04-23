#pragma once
#include "IEventSubscriber.h"

namespace FieaGameEngine
{
	class UnsubscriberFoo final : public IEventSubscriber
	{
	public:
		UnsubscriberFoo();
		~UnsubscriberFoo() = default;
		UnsubscriberFoo(const UnsubscriberFoo& t_rhs) = default;
		UnsubscriberFoo(UnsubscriberFoo&& t_rhs) = default;
		UnsubscriberFoo& operator=(const UnsubscriberFoo& t_rhs) = default;
		UnsubscriberFoo& operator=(UnsubscriberFoo&& t_rhs) = default;
		int32_t data() const;
		virtual void notify(const IEventPublisher& t_event) override;
		void setData(const int& t_value);

	private:
		int32_t m_value;
	};

}
