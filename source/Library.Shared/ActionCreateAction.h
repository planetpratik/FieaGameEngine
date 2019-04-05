#pragma once
#include "Action.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class ActionCreateAction : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)
	public:
		/// <summary>Default Constructor for ActionCreateAction Class.</summary>
		ActionCreateAction();

		/// <summary>Parameterised Constructor for ActionCreateAction ( Explicit )</summary>
		/// <param name="t_name">Const reference to string name with which Action is to be initialized.</param>
		explicit ActionCreateAction(const std::string& t_name);

		/// <summary>Copy Constructor for ActionCreateAction (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to passed ActionCreateAction.</param>
		ActionCreateAction(const ActionCreateAction& t_rhs) = default;

		/// <summary>Move Constructor for ActionCreateAction (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed ActionCreateAction which is to be moved.</param>
		ActionCreateAction(ActionCreateAction&& t_rhs) = default;

		/// <summary>Copy Assignment operator for ActionCreateAction (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to ActionCreateAction which is to be copied.</param>
		/// <returns>Returns Reference to current ActionCreateAction.</returns>
		ActionCreateAction& operator=(const ActionCreateAction& t_rhs) = default;

		/// <summary>Move Assignment operator for ActionCreateAction (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed ActionCreateAction which is to be moved.</param>
		/// <returns>Returns Reference to current ActionCreateAction.</returns>
		ActionCreateAction& operator=(ActionCreateAction&& t_rhs) = default;

		/// <summary>Defaulted Destructor for ActionCreateAction ( Virtual )</summary>
		virtual ~ActionCreateAction() = default;

		/// <summary>Set Action class name (Setter).</summary>
		/// <param name="t_condition">Const Reference to a string which is to be assigned as the class name of Action class.</param>
		void setClassName(const std::string& t_class_name);

		/// <summary>Set Action instance name (Setter).</summary>
		/// <param name="t_condition">Const Reference to a string which is to be assigned as the instance name of Action class.</param>
		void setInstanceName(const std::string& t_instance_name);

		/// <summary>Update method for ActionCreateAction (Overriden from Action).</summary>
		/// <param name="t_world_state">Passed WorldState reference parameter.</param>
		virtual void update(WorldState& t_world_state) override;

		/// <summary>Create clone of an ActionCreateAction (Overriden from Action)</summary>
		/// <returns>Returns new ActionCreateAction clone.</returns>
		virtual gsl::owner<Scope*> clone() const override;

		/// <summary>Static Method to get Signatures which are passed to TypeManager.</summary>
		/// <returns>Returns Vector of Signatures.</returns>
		static Vector<Signature> Signatures();

	private:
		std::string m_class_name;
		std::string m_instance_name;
	};

	ConcreteFactory(ActionCreateAction, Scope)
}