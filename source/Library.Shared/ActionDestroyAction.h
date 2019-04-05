#pragma once
#include "Action.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class ActionDestroyAction : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action)

	public:
		/// <summary>Default Constructor for ActionDestroyAction Class.</summary>
		ActionDestroyAction();

		/// <summary>Parameterised Constructor for ActionDestroyAction ( Explicit )</summary>
		/// <param name="t_name">Const reference to string name with which Action is to be initialized.</param>
		explicit ActionDestroyAction(const std::string& t_name);

		/// <summary>Copy Constructor for ActionDestroyAction (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to passed ActionDestroyAction.</param>
		ActionDestroyAction(const ActionDestroyAction& t_rhs) = default;

		/// <summary>Move Constructor for ActionDestroyAction (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed ActionDestroyAction which is to be moved.</param>
		ActionDestroyAction(ActionDestroyAction&& t_rhs) = default;

		/// <summary>Copy Assignment operator for ActionDestroyAction (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to ActionDestroyAction which is to be copied.</param>
		/// <returns>Returns Reference to current ActionDestroyAction.</returns>
		ActionDestroyAction& operator=(const ActionDestroyAction& t_rhs) = default;

		/// <summary>Move Assignment operator for ActionDestroyAction (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed ActionDestroyAction which is to be moved.</param>
		/// <returns>Returns Reference to current ActionDestroyAction.</returns>
		ActionDestroyAction& operator=(ActionDestroyAction&& t_rhs) = default;

		/// <summary>Update method for ActionCreateAction (Overriden from Action).</summary>
		/// <param name="t_world_state">Passed WorldState reference parameter.</param>
		virtual void update(WorldState& t_world_state) override;

		/// <summary>Set Action Instance Name (Setter)</summary>
		/// <param name="t_instance_name"></param>
		void setActionInstanceName(const std::string& t_instance_name);

		/// <summary>Create clone of an ActionDestroyAction (Overriden from Action)</summary>
		/// <returns>Returns new ActionDestroyAction clone.</returns>
		virtual gsl::owner<Scope*> clone() const override;

		/// <summary>Defaulted Destructor for ActionDestroyAction (Virtual)</summary>
		virtual ~ActionDestroyAction() = default;

		/// <summary>Static Method to get Signatures which are passed to TypeManager.</summary>
		/// <returns>Returns Vector of Signatures.</returns>
		static Vector<Signature> Signatures();
	private:
		std::string m_action_instance_name;
	};

	ConcreteFactory(ActionDestroyAction, Action)
}