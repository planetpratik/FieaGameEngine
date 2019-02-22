#pragma once
#include <glm/fwd.hpp>
#include "RTTI.h"
#include "Datum.h"
#include "Scope.h"


namespace FieaGameEngine
{
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);

	public:

		/// <summary>Copy Constructor for Attributed Class. Makes Deep copy of elements.</summary>
		/// <param name="t_rhs">Const reference to passed Attribute.</param>
		Attributed(const Attributed& t_rhs);

		/// <summary>Move Constructor for Attributed Class. </summary>
		/// <param name="t_rhs">R-Value reference to passed scope which is to be moved.</param>
		Attributed(Attributed&& t_rhs);

		/// <summary>Copy Assignment operator for Attributed Class. Makes Deep copy of elements.</summary>
		/// <param name="t_rhs">Const reference to passed Attribute.</param>
		/// <returns>Reference to current Attributed instance.</returns>
		Attributed& operator=(const Attributed& t_rhs);

		/// <summary>Move Assignment operator for Attributed Class.</summary>
		/// <param name="t_rhs">R-Value reference to passed Attributed which is to be moved.</param>
		/// <returns>Reference to current Attributed instance.</returns>
		Attributed& operator=(Attributed&& t_rhs);

		/// <summary>Clear method for Attributed Class - Clear contents of underlying Scope. ( Overrided )</summary>
		void clear() override;

		/// <summary>Destructor for Attributed Class.</summary>
		virtual ~Attributed() = default;

		/// <summary>Checks whether attribute with given name exists in the Scope or not.</summary>
		/// <param name="t_name">Const reference to name ( std::string ) to be searched as an Attribute.</param>
		/// <returns>Returns boolean result indicating whether given name exists as an attribute or not.</returns>
		bool isAttribute(const std::string& t_name);

		/// <summary>Checks whether Prescribed attribute with given name exists in the Scope or not.</summary>
		/// <param name="t_name">Const reference to name ( std::string ) to be searched as a Prescribed Attribute.</param>
		/// <returns>Returns boolean result indicating whether given name exists as a Prescribed attribute or not.</returns>
		bool isPrescribedAttribute(const std::string& t_name);

		/// <summary>Checks whether Auxillary attribute with given name exists in the Scope or not.</summary>
		/// <param name="t_name">Const reference to name ( std::string ) to be searched as an Auxillary Attribute.</param>
		/// <returns>Returns boolean result indicating whether given name exists as an Auxillary attribute or not.</returns>
		bool isAuxillaryAttribute(const std::string& t_name);

		/// <summary>Append Auxillary attribute to a Scope. If attribute with given name already exists, then return it.</summary>
		/// <param name="t_name">Const reference to name of attribute ( std::string ) to be appended.</param>
		/// <returns>Returns newly appended Auxillary Attribute else return the attribute if already exists.</returns>
		Datum& appendAuxillaryAttribute(const std::string& t_name);

		/// <summary>Append Auxillary attribute to a Scope. If attribute with given name already exists, then return it.</summary>
		/// <param name="t_name">Const reference to name of attribute ( std::string ) to be appended.</param>
		/// <param name="t_datum">Const reference to datum to be appended.</param>
		/// <returns>Returns newly appended Auxillary Attribute else return the attribute if already exists.</returns>
		Datum& appendAuxillaryAttribute(const std::string& t_name, const Datum& t_datum);

		/// <summary>Append Auxillary attribute to a Scope. If attribute with given name already exists, then return it.</summary>
		/// <param name="t_pair">Const reference to String-Datum pair</param>
		/// <returns>Returns newly appended Auxillary Attribute else return the attribute if already exists.</returns>
		Datum& appendAuxillaryAttribute(const std::pair<std::string, Datum>& t_pair);

		/// <summary>Accessor method for Attributes in underlying scope.</summary>
		/// <returns>Returns Const Vector of pointers of attributes string-datum pairs.</returns>
		const Vector<std::pair<std::string, Datum>*> getAttributes() const;

		/// <summary>Accessor method for Prescribed Atrributes in underlying scope.</summary>
		/// <returns>Returns Const Vector of pointers of prescribed attributes string-datum pairs.</returns>
		const Vector<std::pair<std::string, Datum>*> getPrescribedAttributes() const;

		/// <summary>Accessor method for Auxillary Attributes in underlying scope.</summary>
		/// <returns>Returns Const Vector of pointers of auxillary attributes string-datum pairs.</returns>
		const Vector<std::pair<std::string, Datum>*> getAuxillaryAttributes() const;

	protected:

		/// <summary>Explicit Constructor for Attributed Class.</summary>
		/// <param name="t_type_id">Runtime TypeID of Type uint64_t</param>
		explicit Attributed(uint64_t t_type_id);

		/// <summary>To Fetch data using TypeManager & populate scope using it.</summary>
		/// <param name="t_type_id">Runtime TypeID of Type uint64_t</param>
		void populate(uint64_t t_type_id);

	};
}

