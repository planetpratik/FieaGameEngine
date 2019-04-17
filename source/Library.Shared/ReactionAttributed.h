#pragma once
#include "Reaction.h"
#include "Factory.h"
namespace FieaGameEngine
{
	class ReactionAttributed : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction)
	public:

		/// <summary>Default constructor for ReactionAttributed Class.</summary>
		ReactionAttributed();

		/// <summary>Parameterised Constructor for ReactionAttributed Class (Explicit).</summary>
		/// <param name="t_name">Const reference to std::string name with which action is to be initialised.</param>
		explicit ReactionAttributed(const std::string& t_name);

		/// <summary>Copy Constructor for ReactionAttributed. (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to passed ReactionAttributed.</param>
		ReactionAttributed(const ReactionAttributed& t_rhs) = default;

		/// <summary>Move Constructor for ReactionAttributed. (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed ReactionAttributed which is to be moved.</param>
		ReactionAttributed(ReactionAttributed&& t_rhs) = default;

		/// <summary>Copy Assignment operator overload for ReactionAttributed (Defaulted).</summary>
		/// <param name="t_rhs">Const reference to passed ReactionAttributed.</param>
		/// <returns>Returns reference to current ReactionAttributed.</returns>
		ReactionAttributed& operator=(const ReactionAttributed& t_rhs) = default;

		/// <summary>Move Assignment operator overload for ReactionAttributed (Defaulted).</summary>
		/// <param name="t_rhs">R-Value reference to passed ReactionAttributed which is to be moved.</param>
		/// <returns>Returns reference to current ReactionAttributed.</returns>
		ReactionAttributed& operator=(ReactionAttributed&& t_rhs) = default;

		/// <summary>Virtual Destructor for ReactionAttributed class (Defaulted).</summary>
		virtual ~ReactionAttributed();

		/// <summary>This method is called by deliver() method of IEventPublisher. (Overriden)</summary>
		/// <param name="t_event">Const reference to IEventPublisher. </param>
		void notify(const IEventPublisher& t_event) override;

		/// <summary>Static Method to get Signatures which are passed to TypeManager.</summary>
		/// <returns>Vector of Signatures.</returns>
		static Vector<Signature> Signatures();

		/// <summary>Create clone of an ReactionAttributed (Overriden)</summary>
		/// <returns>Returns new ReactionAttributed clone.</returns>
		virtual gsl::owner<Scope*> clone() const override;

		/// <summary>Set ReactionAttributed's SubType string value. (Setter).</summary>
		/// <param name="t_subtype">Const reference to STL std::string representating SubType.</param>
		void setSubtype(const std::string& t_subtype);

		/// <summary>Get ReactionAttributed's SubType.</summary>
		/// <returns>Returns Const reference to STL std::string representing SubType (Const).</returns>
		const std::string& getSubtype() const ;
	private:
		std::string m_subtype;
	};

	ConcreteFactory(ReactionAttributed, Scope)
}
