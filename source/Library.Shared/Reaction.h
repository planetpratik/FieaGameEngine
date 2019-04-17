#pragma once
#include "ActionList.h"
#include "IEventSubscriber.h"
namespace FieaGameEngine
{
	/// <summary>
	/// Reaction abstract base class which Ties Event System to Rest of the Engine
	/// </summary>
	class Reaction : public ActionList, public IEventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList)
	public:

		/// <summary>Default constructor for Reaction.</summary>
		Reaction();
		/// <summary>Parameterised Constructor for Reaction (Explicit).</summary>
		/// <param name="t_name">Const reference to std::string name with which action is to be initialised.</param>
		explicit Reaction(const std::string& t_name);

		/// <summary>Copy Constructor for Reaction. (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to passed Reaction.</param>
		Reaction(const Reaction& t_rhs) = default;

		/// <summary>Move Constructor for Reaction. (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed Reaction which is to be moved.</param>
		Reaction(Reaction&& t_rhs) = default;

		/// <summary>Copy Assignment operator overload for Reaction (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to passed Reaction.</param>
		/// <returns>Returns reference to current Reaction.</returns>
		Reaction& operator=(const Reaction& t_rhs) = default;

		/// <summary>Move Assignment operator overload for Reaction (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed Reaction which is to be moved.</param>
		/// <returns>Returns reference to current Reaction.</returns>
		Reaction& operator=(Reaction&& t_rhs) = default;

		/// <summary>Virtual Destructor for Reaction class (Defaulted).</summary>
		virtual ~Reaction() = default;

		/// <summary>Static Method to get Signatures which are passed to TypeManager.</summary>
		/// <returns>Vector of Signatures.</returns>
		static Vector<Signature> Signatures();
	protected:
		Reaction(const uint64_t& t_type_id, const std::string& t_name);
	};
}