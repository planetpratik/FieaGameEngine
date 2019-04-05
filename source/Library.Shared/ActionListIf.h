#pragma once
#include "ActionList.h"

namespace FieaGameEngine
{
	class ActionListIf : public ActionList
	{
		RTTI_DECLARATIONS(ActionListIf, ActionList)
	public:
		/// <summary>Default Constructor for ActionListIf Class.</summary>
		ActionListIf();

		/// <summary>Parameterised Constructor for ActionListIf ( Explicit )</summary>
		/// <param name="t_name">Const reference to string name with which Action is to be initialized.</param>
		explicit ActionListIf(const std::string& t_name);

		/// <summary>Defaulted Destructor for ActionListIf ( Virtual )</summary>
		virtual ~ActionListIf() = default;

		/// <summary>Copy Constructor for ActionListIf (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to passed ActionListIf.</param>
		ActionListIf(const ActionListIf& t_rhs) = default;

		/// <summary>Move Constructor for ActionListIf (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed ActionListIf which is to be moved.</param>
		ActionListIf(ActionListIf&& t_rhs) = default;

		/// <summary>Copy Assignment operator for ActionListIf (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to ActionListIf which is to be copied.</param>
		/// <returns>Returns Reference to current ActionListIf.</returns>
		ActionListIf& operator=(const ActionListIf& t_rhs) = default;

		/// <summary>Move Assignment operator for ActionListIf (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed ActionListIf which is to be moved.</param>
		/// <returns>Returns Reference to current ActionListIf.</returns>
		ActionListIf& operator=(ActionListIf&& t_rhs) = default;

		/// <summary>Update method for ActionListIf (Overriden from Action).</summary>
		/// <param name="t_world_state">Passed WorldState reference parameter.</param>
		virtual void update(WorldState& t_world_state) override;

		/// <summary>Set Condition Value. (Setter).</summary>
		/// <param name="t_condition">Reference to an Integer which is to be assigned for if-else condition</param>
		void setCondition(const int32_t & t_condition);

		/// <summary>Set If Block Action (Setter).</summary>
		/// <param name="t_if_action">Reference to an Action which is to be assigned for "if" condition block.</param>
		void setIfBlock(Action& t_if_action);

		/// <summary>Set Else Block Action (Setter).</summary>
		/// <param name="t_else_action">Reference to an Action which is to be assigned for "else" condition block.</param>
		void setElseBlock(Action& t_else_action);

		/// <summary>Static Method to get Signatures which are passed to TypeManager.</summary>
		/// <returns>Returns Vector of Signatures.</returns>
		static Vector<Signature> Signatures();

		/// <summary>Create clone of an ActionListIf (Overriden from Action)</summary>
		/// <returns>Returns new ActionListIf clone.</returns>
		virtual gsl::owner<Scope*> clone() const override;

	private:
		int32_t m_condition_value;
	};

	ConcreteFactory(ActionListIf, Action)
}