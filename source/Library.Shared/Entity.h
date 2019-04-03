#pragma once
#include "Attributed.h"
#include "TypeManager.h"
#include "WorldState.h"
#include "Factory.h"

namespace FieaGameEngine
{
	/// <summary>Forward Declaration of Sector Class</summary>
	class Sector;

	/// <summary>
	/// Entity is a Wrapper for Attributed Scope.
	/// Entity objects automatically populate their tables based on information known at compile time.
	/// <Summary>
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)

	public:
		/// <summary>Default Constructor for Entity</summary>
		Entity();

		/// <summary>Parameterised Constructor for Entity ( Explicit )</summary>
		/// <param name="t_name">Const reference to string name with which Entity is to be initialized.</param>
		explicit Entity(const std::string& t_name);

		/// <summary>Copy Constructor for Entity.</summary>
		/// <param name="t_rhs">Const reference to passed Entity.</param>
		Entity(const Entity& t_rhs);

		/// <summary>Move Constructor for Entity.</summary>
		/// <param name="t_rhs">R-Value reference to passed Entity which is to be moved.</param>
		Entity(Entity&& t_rhs);

		/// <summary>Copy Assignment operator overload for Entity.</summary>
		/// <param name="t_rhs">Const reference to passed Entity.</param>
		/// <returns>Returns reference to current Entity.</returns>
		Entity& operator=(const Entity& t_rhs);

		/// <summary>Move Assignment operator overload for Entity.</summary>
		/// <param name="t_rhs">R-Value reference to passed Entity which is to be moved.</param>
		/// <returns>Returns reference to current Entity.</returns>
		Entity& operator=(Entity&& t_rhs);

		/// <summary>Defaulted Destructor for Entity ( Virtual )</summary>
		virtual ~Entity() = default;

		/// <summary>To Get name of the Entity. (Getter)</summary>
		/// <returns>Returns name of the Entity in STL std::string.</returns>
		const std::string& name() const;

		/// <summary>To set name of the Entity (Setter).</summary>
		/// <param name="t_name">Const reference to Name of the Entity of type STL std::string.</param>
		void setName(const std::string& t_name);

		/// <summary>To get Sector associated with given Entity. (Getter)</summary>
		/// <returns>Parent Sector associated with given Entity.</returns>
		Sector* getSector() const;

		/// <summary>To set Sector associated with given Entity. (Setter)</summary>
		/// <param name="t_sector">Pointer to Sector which is to be assigned as parent to given entity.</param>
		void setSector(Sector* t_sector);

		/// <summary>Takes a WorldState reference and Calls Update to contained Actions.</summary>
		/// <param name="t_world_state">Passed WorldState reference parameter.</param>
		void update(WorldState& t_world_state);

		/// <summary>To Populate Entity during construction with default values.</summary>
		void populate();

		/// <summary>To update Storage of name parameter to External.</summary>
		void updateExternalStorage();

		/// <summary>Create clone of an Entity (Overriden)</summary>
		/// <returns>Returns new Entity clone.</returns>
		virtual gsl::owner<Scope*> clone() const override;

		/// <summary>Static Method to get Signatures which are passed to TypeManager.</summary>
		/// <returns>Vector of Signatures.</returns>
		static Vector<Signature> Signatures();
	private:
		std::string m_entity_name;
	};

	ConcreteFactory(Entity, Scope);
}

