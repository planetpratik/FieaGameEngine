#pragma once
#include "Attributed.h"
#include "TypeManager.h"
#include "WorldState.h"


namespace FieaGameEngine
{
	/// <summary>Forward declaration of Entity Class.</summary>
	class Entity;

	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed)

	public:
		/// <summary>Default Constructor for Action Class.</summary>
		Action();

		/// <summary>Parameterised Constructor for Action ( Explicit )</summary>
		/// <param name="t_name">Const reference to string name with which Action is to be initialized.</param>
		explicit Action(const std::string& t_name);

		/// <summary>Defaulted Destructor for Action ( Virtual )</summary>
		virtual ~Action() = default;

		/// <summary>Copy Constructor for Action (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to passed Action.</param>
		Action(const Action& t_rhs) = default;

		/// <summary>Move Constructor for Action (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed Action which is to be moved.</param>
		Action(Action&& t_rhs) = default;

		/// <summary>Copy Assignment operator for Action (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to Action which is to be copied.</param>
		/// <returns>Returns Reference to current Action.</returns>
		Action& operator=(const Action& t_rhs) = default;

		/// <summary>Move Assignment operator for Action (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed Action which is to be moved.</param>
		/// <returns>Returns Reference to current Action.</returns>
		Action& operator=(Action&& t_rhs) = default;

		/// <summary>To Get name of the Action ( Getter ).</summary>
		/// <returns>Const reference to STL std::string representing name of the Action.</returns>
		const std::string& name() const;

		/// <summary>To Set name of the Action ( Setter ).</summary>
		/// <param name="t_name">Const reference to STL std::string representing name of the Action.</param>
		void setName(const std::string& t_name);

		/// <summary>Update method for Actions (Pure Virtual Method).</summary>
		/// <param name="t_world_state">Passed WorldState reference parameter.</param>
		virtual void update(WorldState& t_world_state) = 0;

		/// <summary>To get Entity associated with given Action. (Getter)</summary>
		/// <returns>Parent Entity pointer associated with given Action.</returns>
		Entity* getEntity() const;

		/// <summary>To set Entity associated with given Action. (Setter)</summary>
		/// <param name="t_sector">Pointer to Entity which is to be assigned as parent to given Action.</param>
		void setEntity(Entity* t_entity);

		/// <summary>Static Method to get Signatures which are passed to TypeManager.</summary>
		/// <returns>Vector of Signatures.</returns>
		static Vector<Signature> Signatures();

		/// <summary>Create clone of an Action (Pure Virtual function)</summary>
		/// <returns>Returns new Action clone.</returns>
		virtual gsl::owner<Scope*> clone() const = 0;

	protected:
		Action(const uint64_t& t_type_id, const std::string& t_name);

	private:
		std::string m_action_name;
	};
}