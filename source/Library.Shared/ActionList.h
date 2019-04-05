#pragma once
#include "Action.h"
#include "Factory.h"
namespace FieaGameEngine
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList,Action)

	public:
		/// <summary>Default Constructor for ActionList Class.</summary>
		ActionList();

		/// <summary>Parameterised Constructor for ActionList ( Explicit )</summary>
		/// <param name="t_name">Const reference to string name with which ActionList is to be initialized.</param>
		explicit ActionList(const std::string& t_name);

		/// <summary>Copy Constructor for ActionList (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to passed ActionList.</param>
		ActionList(const ActionList& t_rhs) = default;

		/// <summary>Move Constructor for ActionList (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed ActionList which is to be moved.</param>
		ActionList(ActionList&& t_rhs) = default;

		/// <summary>Copy Assignment operator for ActionList (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to ActionList which is to be copied.</param>
		/// <returns>Returns Reference to current ActionList.</returns>
		ActionList& operator=(const ActionList& t_rhs) = default;

		/// <summary>Move Assignment operator for ActionList (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed ActionList which is to be moved.</param>
		/// <returns>Returns Reference to current ActionList.</returns>
		ActionList& operator=(ActionList&& t_rhs) = default;

		/// <summary>Defaulted Destructor for ActionList ( Virtual )</summary>
		virtual ~ActionList() = default;

		/// <summary>Update method for ActionList (Overriden from Action).</summary>
		/// <param name="t_world_state">Passed WorldState reference parameter.</param>
		virtual void update(WorldState& t_world_state) override;

		/// <summary>
		/// Takes an Action-derived class name and its instance name, and 
		/// uses the factory to make a new object of the given type,
		/// And adopts the Action into the Actions.
		/// </summary>
		/// <param name="t_class_name">Const reference to Class name in STL std::string.</param>
		/// <param name="t_instance_name">Const reference to desired Instance name in STL std::string.</param>
		/// <returns>Retuns address of newly created Action.</returns>
		Action* createAction(const std::string& t_class_name, const std::string& t_instance_name);

		/// <summary>To get contained Actions</summary>
		/// <returns> Returns the Datum reference that stores the contained entities.</returns>
		Datum& actions();

		/// <summary>Static Method to get Signatures which are passed to TypeManager.</summary>
		/// <returns>Vector of Signatures.</returns>
		static Vector<Signature> Signatures();

		/// <summary>Create clone of an ActionList (Overriden from Action)</summary>
		/// <returns>Returns new ActionList clone.</returns>
		virtual gsl::owner<Scope*> clone() const override;

	protected:
		ActionList(const uint64_t& t_type_id, const std::string& t_name);
	};

	ConcreteFactory(ActionList, Scope)
}