#pragma once
#include "Attributed.h"
#include "TypeManager.h"
namespace FieaGameEngine
{
	// Forward Declaration of World class.
	class World;

	class EventMessageAttributed final : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed)
	public:

		/// <summary>Default constructor for EventMessageAttributed Class.</summary>
		EventMessageAttributed();

		/// <summary>Parameterised Constructor for EventMessageAttributed Class (Explicit).</summary>
		/// <param name="t_name">Const reference to std::string name with which SubType is to be initialised.</param>
		explicit EventMessageAttributed(const std::string& t_name);

		/// <summary>Copy Constructor for EventMessageAttributed. (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to passed EventMessageAttributed.</param>
		EventMessageAttributed(const EventMessageAttributed& t_rhs) = default;

		/// <summary>Move Constructor for EventMessageAttributed. (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed EventMessageAttributed which is to be moved.</param>
		EventMessageAttributed(EventMessageAttributed&& t_rhs) = default;

		/// <summary>Copy Assignment operator overload for EventMessageAttributed (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to passed EventMessageAttributed.</param>
		/// <returns>Returns reference to current EventMessageAttributed.</returns>
		EventMessageAttributed& operator=(const EventMessageAttributed& t_rhs) = default;

		/// <summary>Move Assignment operator overload for EventMessageAttributed (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed EventMessageAttributed which is to be moved.</param>
		/// <returns>Returns reference to current EventMessageAttributed.</returns>
		EventMessageAttributed& operator=(EventMessageAttributed&& t_rhs) = default;

		/// <summary>Virtual Destructor for EventMessageAttributed class (Defaulted).</summary>
		virtual ~EventMessageAttributed() = default;

		/// <summary>Set EventMessageAttributed's SubType string value. (Setter).</summary>
		/// <param name="t_subtype">Const reference to STL std::string representating SubType.</param>
		void setSubType(const std::string& t_subtype);

		/// <summary>Set EventMessageAttributed's World pointer. (Setter).</summary>
		/// <param name="t_subtype">Reference to World.</param>
		void setWorld(World& t_world);

		/// <summary>Get EventMessageAttributed's SubType. (Const)</summary>
		/// <returns>Returns Const reference to STL std::string representing SubType.</returns>
		const std::string& getSubType() const;

		/// <summary>Get EventMessageAttributed's World pointer. (Const)</summary>
		/// <returns>Returns pointer to World</returns>
		World* getWorld() const;

		/// <summary>Static Method to get Signatures which are passed to TypeManager.</summary>
		/// <returns>Vector of Signatures.</returns>
		static Vector<Signature> Signatures();

		/// <summary>Create clone of an EventMessageAttributed (Overriden)</summary>
		/// <returns>Returns new EventMessageAttributed clone.</returns>
		virtual gsl::owner<Scope*> clone() const override;
	private:
		std::string m_subtype;
		World* m_world;
	};

}
