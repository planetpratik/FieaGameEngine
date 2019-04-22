#pragma once
#include "IEventPublisher.h"

using MilliSeconds = std::chrono::milliseconds;

namespace FieaGameEngine
{
	/// <summary>
	/// Forward Reference to GameTime Class.
	/// </summary>
	class GameTime;

	/// <summary>
	/// EventQueue System for enqueuing and sending events to Event Subscribers. 
	/// </summary>
	class EventQueue
	{
	public:
		/// <summary>Default Constructor for EventQueue (Default).</summary>
		EventQueue() = default;

		/// <summary>Copy Constructor for EventQueue (Default).</summary>
		/// <param name="t_rhs">Const reference to passed EventQueue.</param>
		EventQueue(const EventQueue& t_rhs) = default;

		/// <summary>Move Constructor for EventQueue (Default).</summary>
		/// <param name="t_rhs">R-Value reference to passed EventQueue which is to be moved.</param>
		EventQueue(EventQueue&& t_rhs) = default;

		/// <summary>Copy Assignment operator overload for EventQueue. (Default).</summary>
		/// <param name="t_rhs">Const reference to passed EventQueue.</param>
		/// <returns>Reference to current EventQueue.</returns>
		EventQueue& operator=(const EventQueue& t_rhs) = default;

		/// <summary>Move Assignment operator overload implementation for EventQueue (Default).</summary>
		/// <param name="t_rhs">R-Value reference to passed EventQueue which is to be moved.</param>
		/// <returns>Reference to current EventQueue.</returns>
		EventQueue& operator=(EventQueue&& t_rhs) = default;

		/// <summary>Destructor for EventQueue (Default).</summary>
		~EventQueue() = default;

		/// <summary>Enqueue the event to be delivered to subscriber based on given parameters.</summary>
		/// <param name="t_event">Shared Pointer (shared_ptr) to IEventPublisher.</param>
		/// <param name="t_game_time">Const reference to GameTime (Used to retrieve current time).</param>
		/// <param name="t_delay">Const reference to Delay in MilliSeconds </param>
		void enqueue(std::shared_ptr<IEventPublisher> t_event, const GameTime& t_game_time, const MilliSeconds& t_delay);
		
		/// <summary>Given the address of an EventPublisher, send the event immediately.</summary>
		/// <param name="t_event">Shared Pointer (shared_ptr) to IEventPublisher (Event to send).</param>
		void send(std::shared_ptr<IEventPublisher> t_event);

		/// <summary>Given the a GameTime, publish any queued events that have expired.</summary>
		/// <param name="t_game_time">Const reference to GameTime (From which Current Time will be retrieved)</param>
		void update(const GameTime& t_game_time);

		/// <summary>Clear the Event Queue.</summary>
		void clear();

		/// <summary>Conveys whether EventQueue is empty or not.</summary>
		/// <returns>Returns a boolean indicating emptiness of the queue.</returns>
		bool isEmpty() const;

		/// <summary>Gets number of events in the EventQueue.</summary>
		/// <returns>Returns Number of events (Size) of the EventQueue.</returns>
		uint32_t size() const;
	private:
		mutable std::mutex m_mutex;
		std::vector<std::shared_ptr<IEventPublisher>> m_event_queue;
		std::vector<std::shared_ptr<IEventPublisher>> m_expired_event_queue;
	};

}