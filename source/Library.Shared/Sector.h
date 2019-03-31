#pragma once
#include "Attributed.h"
#include "Entity.h"
#include "World.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Sector is purely an artifact of finite resources.
	/// Sector is loosely comparable to the idea of a package or library, and exists to partition
	/// code & data into pieces that we can manage in isolation. A Sector is another container
	/// but it is one which does not introduce a namespace.
	/// We should be able to move data and processes between Sectors without affecting anything about the data itself.
	/// Sectors are, essentially, just files that contain descriptions of data.
	/// </summary>
	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed)

	public:

		/// <summary>Default Constructor for Sector</summary>
		Sector();

		/// <summary>Parameterised Constructor for Sector ( Explicit )</summary>
		/// <param name="t_name">Const reference to string name with which Sector is to be initialized.</param>
		explicit Sector(const std::string& t_name);

		/// <summary>Copy Constructor for Sector.</summary>
		/// <param name="t_rhs">Const reference to passed Sector.</param>
		Sector(const Sector& t_rhs);

		/// <summary>Move Constructor for Sector.</summary>
		/// <param name="t_rhs">R-Value reference to passed Sector which is to be moved.</param>
		Sector(Sector&& t_rhs);

		/// <summary>Copy Assignment operator overload for Sector.</summary>
		/// <param name="t_rhs">Const reference to passed Sector.</param>
		/// <returns>Returns reference to current Sector.</returns>
		Sector& operator=(const Sector& t_rhs);

		/// <summary>Move Assignment operator overload for Sector.</summary>
		/// <param name="t_rhs">R-Value reference to passed Sector which is to be moved.</param>
		/// <returns>Returns reference to current Sector.</returns>
		Sector& operator=(Sector&& t_rhs);

		/// <summary>Defaulted Destructor for Sector ( Virtual )</summary>
		virtual ~Sector() = default;

		/// <summary>To Get name of the Sector. (Getter)</summary>
		/// <returns>Returns name of the Sector in STL std::string.</returns>
		const std::string& name() const;

		/// <summary>To set name of the Sector (Setter).</summary>
		/// <param name="t_name">Const reference to Name of the Sector of type STL std::string.</param>
		void setName(const std::string& t_name);

		/// <summary>To get contained entities in the Sector.</summary>
		/// <returns> Returns the Datum reference that stores the contained entities in the sector.</returns>
		Datum& entities();

		/// <summary>
		/// Takes an entity class name and its instance name, and 
		/// uses the entity factory to make a new object of the given type,
		/// And adopts the entity into the sector.
		/// </summary>
		/// <param name="t_class_name">Const reference to Class name in STL std::string.</param>
		/// <param name="t_instance_name">Const reference to desired Instance name in STL std::string.</param>
		/// <returns>Retuns address of newly created Entity.</returns>
		Entity* createEntity(const std::string& t_class_name, const std::string& t_instance_name);

		/// <summary>To get World associated with given Sector. (Getter)</summary>
		/// <returns>Parent World associated with given Sector.</returns>
		World* getWorld() const;

		/// <summary>To set World associated with given Sector. (Setter)</summary>
		/// <param name="t_sector">Pointer to World which is to be assigned as parent to given Sector.</param>
		void setWorld(World* t_world);

		/// <summary>Takes a WorldState reference and Calls Update to contained Entities.</summary>
		/// <param name="t_world_state">Passed WorldState reference parameter.</param>
		void update(WorldState& t_world_state);

		/// <summary>To Populate Sector during construction with default values.</summary>
		void populate();

		/// <summary>To update Storage of name parameter to External.</summary>
		void updateExternalStorage();

		/// <summary>Create clone of a Sector (Overriden)</summary>
		/// <returns>Returns new Sector clone.</returns>
		virtual gsl::owner<Scope*> clone() const override;

		/// <summary>Static Method to get Signatures which are passed to TypeManager.</summary>
		/// <returns>Vector of Signatures.</returns>
		static Vector<Signature> Signatures();
	private:
		std::string m_sector_name;
		Datum* m_entities_table;
	};

}
