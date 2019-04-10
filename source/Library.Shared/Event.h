#pragma once
#include "IEventPublisher.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Templated Event Class which Derives from EventPublisher Abstract Interface.
	/// Template Argument is Concrete Message ( Payload ) class.
	/// </summary>
	template<typename T>
	class Event final : public IEventPublisher
	{
		RTTI_DECLARATIONS(Event, IEventPublisher);

	public:

		/// <summary>Constructor for Event Class (Explicit)</summary>
		/// <param name="t_payload">Reference to the Message Object (Payload)</param>
		explicit Event(T& t_payload);

		/// <summary>Copy Constructor for Event Class (Default).</summary>
		/// <param name="t_rhs">Const reference to passed Event.</param>
		Event(const Event& t_rhs) = default;

		/// <summary>Move Constructor for Event Class (Default).</summary>
		/// <param name="t_rhs">R-Value reference to passed Event which is to be moved.</param>
		Event(Event&& t_rhs) = default;

		/// <summary>Copy Assignment Operator Overload for Event Class (Default).</summary>
		/// <param name="t_rhs">Const reference to passed Event.</param>
		/// <returns>Reference to Current Event.</returns>
		Event& operator=(const Event& t_rhs) = default;

		/// <summary>Move Assignment Operator overload for Event Class (Default).</summary>
		/// <param name="t_rhs">R-Value reference to passed Event which is to be moved.</param>
		/// <returns>Reference to current Event.</returns>
		Event& operator=(Event&& t_rhs) = default;

		/// <summary>Destructor for Event Class. (Default)</summary>
		~Event() = default;

		/// <summary>Given the address of an EventSubscriber, add it to the list of subscribers for this event type (Static).</summary>
		/// <param name="t_subscriber">Reference to an IEventSubscriber.</param>
		static void subscribe(IEventSubscriber& t_subscriber);

		/// <summary>Given the address of an EventSubscriber, remove it from the list of subscribers for this event type (Static).</summary>
		/// <param name="t_subscriber">Reference to an IEventSubscriber.</param>
		static void unsubscribe(IEventSubscriber& t_subscriber);

		/// <summary>Unsubscribe all subscribers to this event type. (Static).</summary>
		static void unsubscribeAll();
		const T& message() const;
	private:
		T payload;
		static Vector<IEventSubscriber*> m_subscribers_list;
	};
}
#include "Event.inl"