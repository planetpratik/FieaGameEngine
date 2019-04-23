#pragma once
#include "RTTI.h"
#include <vector>
#include <mutex>
#include <thread>
#include <future>
#include <chrono>
#include "IEventSubscriber.h"
#include "Vector.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Abstract Base class for Event Publisher.
	/// </summary>
	class IEventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(IEventPublisher, RTTI)

	public:
		using TimePoint = std::chrono::high_resolution_clock::time_point;
		using MilliSeconds = std::chrono::milliseconds;

		/// <summary>Constructor for IEventPublisher (Explicit)</summary>
		/// <param name="t_subscribers">Reference to Vector of Event Subscribers.</param>
		explicit IEventPublisher(Vector<IEventSubscriber*>& t_subscribers, std::mutex& t_mutex);

		/// <summary>Destructor for IEventPublisher (Virtual).</summary>
		virtual ~IEventPublisher() = default;

		/// <summary>Copy constructor for IEventPublisher.</summary>
		/// <param name="t_rhs">Const reference to passed IEventPublisher.</param>
		IEventPublisher(const IEventPublisher& t_rhs) = default;

		/// <summary>Move constructor for IEventPublisher.</summary>
		/// <param name="t_rhs">R-Value reference to IEventPublisher which is to be moved.</param>
		IEventPublisher(IEventPublisher&& t_rhs) = default;

		/// <summary>Copy Assignment Operator Overload for IEventPublisher.</summary>
		/// <param name="t_rhs">Const reference to passed IEventPublisher.</param>
		/// <returns>Reference to current IEventPublisher.</returns>
		IEventPublisher& operator=(const IEventPublisher& t_rhs) = default;

		/// <summary>Move Assignment Operator Overload for IEventPublisher.</summary>
		/// <param name="t_rhs">R-Value reference to passed IEventPublisher which is to be moved.</param>
		/// <returns>Reference to current IEventPublisher.</returns>
		IEventPublisher& operator=(IEventPublisher&& t_rhs) = default;

		/// <summary>Assign Current Time & Delay (Optional) to the given Event.</summary>
		/// <param name="t_enqueued_time">Current Time in Chrono::Time_Point format.</param>
		/// <param name="t_delay">Delay in MilliSeconds.</param>
		void setTime(const TimePoint& t_enqueued_time, MilliSeconds t_delay = MilliSeconds(0));

		/// <summary>Gets Time when given Event was Enqueued (Const)</summary>
		/// <returns>Returns Const reference to Enqueued Time in Chrono::Time_Point format.</returns>
		const TimePoint& timeEnqueued() const;

		/// <summary>Gets the amount of time after being enqueued that this event expires.</summary>
		/// <returns>Returns Const reference to delay time in Chrono::MilliSeconds </returns>
		const MilliSeconds& delay() const;

		/// <summary>Checks whether Current time has surpassed Enqueued Time plus Delay.</summary>
		/// <param name="t_current_time">Const reference to currentTime in Chrono::Time_Point format.</param>
		/// <returns>Boolean indicating whether Event is expired or not ( So it can be removed ).</returns>
		bool isExpired(const TimePoint& t_current_time) const;

		/// <summary>Delivers the Event to Subscriber ( Notifies all subscribers of this event ). </summary>
		void deliver() const;
	private:
		TimePoint m_enqueued_time;
		MilliSeconds m_delay;
		std::mutex* m_mutex = nullptr;
		Vector<IEventSubscriber*>* m_subscribers_list = nullptr;
	};
}
