#pragma once
#include "Action.h"
#include "Factory.h"
namespace FieaGameEngine
{
	class ActionEvent final: public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action)
	public:

		/// <summary>Default constructor for ActionEvent Class.</summary>
		ActionEvent();

		/// <summary>Parameterised Constructor for ActionEvent Class (Explicit).</summary>
		/// <param name="t_name">Const reference to std::string name with which Action is to be initialised.</param>
		explicit ActionEvent(const std::string& t_name);

		/// <summary>Copy Constructor for ActionEvent. (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to passed EventMessageAttributed.</param>
		ActionEvent(const ActionEvent& t_rhs) = default;

		/// <summary>Move Constructor for ActionEvent. (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed ActionEvent which is to be moved.</param>
		ActionEvent(ActionEvent&& t_rhs) = default;

		/// <summary>Copy Assignment operator overload for ActionEvent (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to passed ActionEvent.</param>
		/// <returns>Returns reference to current ActionEvent.</returns>
		ActionEvent& operator=(const ActionEvent& t_rhs) = default;

		/// <summary>Move Assignment operator overload for ActionEvent (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed ActionEvent which is to be moved.</param>
		/// <returns>Returns reference to current ActionEvent.</returns>
		ActionEvent& operator=(ActionEvent&& t_rhs) = default;

		/// <summary>Virtual Destructor for ActionEvent class (Defaulted).</summary>
		virtual ~ActionEvent() = default;

		/// <summary>
		/// Update method overriden from Actions. 
		/// Create an attributed event, assign its world and subtype,
		/// copy all auxiliary parameters into the event and queue the event with the given delay (Overriden).
		/// </summary>
		/// <param name="t_world_state"></param>
		virtual void update(WorldState& t_world_state) override;

		/// <summary>Create clone of an ActionEvent (Overriden).</summary>
		/// <returns>Returns new ActionEvent clone.</returns>
		virtual gsl::owner<Scope*> clone() const override;

		/// <summary>Static Method to get Signatures which are passed to TypeManager.</summary>
		/// <returns>Vector of Signatures.</returns>
		static Vector<Signature> Signatures();

		/// <summary>Set ActionEvent's Delay in Milliseconds. (Setter).</summary>
		/// <param name="t_subtype">Const reference to milliseconds representating Delay.</param>
		void setDelay(const std::chrono::milliseconds& t_milliseconds);

		/// <summary>Set ActionEvent's SubType string value. (Setter).</summary>
		/// <param name="t_subtype">Const reference to STL std::string representating SubType.</param>
		void setSubtype(const std::string& t_subtype);

		/// <summary>Get ActionEvent's Delay. (Const)</summary>
		/// <returns>Returns reference to milliseconds representing Delay.</returns>
		const std::chrono::milliseconds getDelay() const;

		/// <summary>Get ActionEvent's SubType. (Const)</summary>
		/// <returns>Returns Const reference to STL std::string representing SubType.</returns>
		const std::string& getSubtype() const;
	private:
		std::string m_subtype;
		uint32_t m_delay;
	};

	ConcreteFactory(ActionEvent, Scope)
}
