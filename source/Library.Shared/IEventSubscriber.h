#pragma once
namespace FieaGameEngine
{
	/// <summary>
	/// Forward Reference to IEventPublisher Class.
	/// </summary>
	class IEventPublisher;

	/// <summary>
	/// Abstract Base class for EventSubscriber.
	/// </summary>
	class IEventSubscriber
	{
	public:
		/// <summary>Constructor for IEventSubscriber (Default)</summary>
		IEventSubscriber() = default;

		/// <summary>Destructor for IEventSubscriber (Virtual)</summary>
		virtual ~IEventSubscriber() = default;

		/// <summary>Copy constructor for IEventSubscriber (Default)</summary>
		/// <param name="t_rhs">Const reference to passed IEventSubscriber</param>
		IEventSubscriber(const IEventSubscriber& t_rhs) = default;

		/// <summary>Move Constructor for IEventSubscriber (Default)</summary>
		/// <param name="t_rhs">R-Value reference to IEventSubscriber which is to be moved.</param>
		IEventSubscriber(IEventSubscriber&& t_rhs) = default;

		/// <summary>Copy Assignment operator for IEventPublisher (Default).</summary>
		/// <param name="t_rhs">Const reference to passed IEventSubscriber.</param>
		/// <returns>Returns current IEventSubscriber.</returns>
		IEventSubscriber& operator=(const IEventSubscriber& t_rhs) = default;

		/// <summary>Move Assignment operator for IEventPublisher (Default).</summary>
		/// <param name="t_rhs">R-Value reference to IEventSubscriber which is to be moved.</param>
		/// <returns>Returns current IEventSubscriber.</returns>
		IEventSubscriber& operator=(IEventSubscriber&& t_rhs) = default;

		/// <summary>This method is called by deliver() method of IEventPublisher. (Pure Virtual Method)</summary>
		/// <param name="t_event">Const reference to IEventPublisher. </param>
		virtual void notify(const IEventPublisher& t_event) = 0;
	};

}