#pragma once
#include "Attributed.h"
#include "TypeManager.h"
#include "EventQueue.h"
#include "Reaction.h"
namespace FieaGameEngine
{
	/// <summary>Forward Declaration of WorldState Class</summary>
	class Sector;
	class WorldState;
	/// <summary>
	/// World class is Yet Another Container – a container of Sectors.
	/// A World defines a global Scope – the Scope that all other objects in the simulation share.
	/// </summary>
	class World final: public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed)

	public:

		/// <summary>Default Constructor for World</summary>
		World();

		/// <summary>Parameterised Constructor for World ( Explicit )</summary>
		/// <param name="t_name">Const reference to string name with which World is to be initialized.</param>
		explicit World(const std::string& t_name);

		World(const std::string& t_name, EventQueue& t_event_queue);

		/// <summary>Copy Constructor for World (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to passed World.</param>
		World(const World& t_rhs) = default;

		/// <summary>Move Constructor for World (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed World which is to be moved.</param>
		World(World&& t_rhs) = default;

		/// <summary>Copy Assignment operator overload for World (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to passed World.</param>
		/// <returns>Returns reference to current World.</returns>
		World& operator=(const World& t_rhs) = default;

		/// <summary>Move Assignment operator overload for World (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed World which is to be moved.</param>
		/// <returns>Returns reference to current World.</returns>
		World& operator=(World&& t_rhs) = default;

		/// <summary>Defaulted Destructor for World ( Virtual )</summary>
		virtual ~World() = default;

		/// <summary>To Get name of the World. (Getter)</summary>
		/// <returns>Returns name of the World in STL std::string.</returns>
		const std::string& name() const;

		/// <summary>To set name of the World (Setter).</summary>
		/// <param name="t_name">Const reference to Name of the World of type STL std::string.</param>
		void setName(const std::string& t_name);

		/// <summary>To get contained sectors in the World.</summary>
		/// <returns> Returns the Datum reference that stores the contained sectors in the World.</returns>
		Datum& sectors();

		/// <summary>
		/// Takes a Sector name and instantiate new Sector,
		/// And adopts the Sector into the World.
		/// </summary>
		/// <param name="t_sector_name">Const reference to Sector name in STL std::string.</param>
		/// <returns>Retuns address of newly created Sector.</returns>
		Sector* createSector(const std::string& t_sector_name);

		/// <summary>Takes a WorldState reference and Calls Update to contained Sectors.</summary>
		/// <param name="t_world_state">Passed WorldState reference parameter.</param>
		void update(WorldState& t_world_state);

		/// <summary>Create clone of a World (Overriden)</summary>
		/// <returns>Returns new World clone.</returns>
		virtual gsl::owner<Scope*> clone() const override;

		/// <summary>Static Method to get Signatures which are passed to TypeManager.</summary>
		/// <returns>Vector of Signatures.</returns>
		static Vector<Signature> Signatures();

		WorldState& getWorldState();

		EventQueue& getEventQueue();

		Datum& reactions();

		Reaction* createReaction(const std::string& t_class_name);

	private:
		std::string m_world_name;
		WorldState* m_world_state = nullptr;
		EventQueue* m_event_queue = nullptr;
		const static inline uint32_t sectors_index = 2;
	};
}
