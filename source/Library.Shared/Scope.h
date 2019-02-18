#pragma once
#include "RTTI.h"
#include <cstdint>
#include "HashMap.h"
#include "Datum.h"
#include "Vector.h"
namespace FieaGameEngine
{
	/// <summary>Scope - Dynamic Hierarchical Database for Datum. Implements RTTI interface.</summary>
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI);

	public:
		using LookupTable = HashMap<std::string, Datum>;
		using TableEntry = LookupTable::PairType;
		using PointersList = Vector<TableEntry*>;

		/// <summary>Parameterised Constructor for Scope - Initializes Hashmap & Order Vector.</summary>
		/// <param name="t_default_size">Size for the Hashmap.</param>
		explicit Scope(const uint32_t& t_default_size = DEFAULT_SIZE);

		/// <summary>Copy Constructor for Scope. Makes Deep copy of elements.</summary>
		/// <param name="t_rhs">Const reference to passed Scope.</param>
		Scope(const Scope& t_rhs);

		/// <summary>Move Constructor for Scope.</summary>
		/// <param name="t_rhs">R-Value reference to passed Scope which is to be moved.</param>
		Scope(Scope&& t_rhs);

		/// <summary>Copy Assignment Operator for Scope. Makes deep copy of elements.</summary>
		/// <param name="t_rhs">Const reference to passed Scope.</param>
		/// <returns>Reference to current Scope.</returns>
		Scope& operator=(const Scope& t_rhs);

		/// <summary>Move Assignment Operator for Scope.</summary>
		/// <param name="t_rhs">R-Value reference to passed Scope which is to be moved.</param>
		/// <returns>Returns reference to current Scope.</returns>
		//Scope& operator=(Scope&& t_rhs);

		/// <summary>Recursively delete contents of this scope.</summary>
		virtual void clear();

		/// <summary>Destructor for Scope. Calls clear() for recursive destruction.</summary>
		virtual ~Scope();

		/// <summary>Find a Datum stored using given name in current Scope.</summary>
		/// <param name="t_name">Const reference to a name of type STL std::string</param>
		/// <returns>Returns pointer to the datum if name found. Else returns nullptr.</returns>
		Datum* find(const std::string& t_name);

		/// <summary>Const version of find() - Find a Datum stored using given name in current Scope.</summary>
		/// <param name="t_name">Const reference to a name of type STL std::string to be searched.</param>
		/// <returns>Returns const pointer to the datum if name found. Else returns nullptr.</returns>
		const Datum* find(const std::string& t_name) const;

		/// <summary>Search for most closely nested datum to given owner scope.</summary>
		/// <param name="t_name">Const reference to a name of type STL std::string to be searched.</param>
		/// <param name="t_owner">(Out param) Pointer to scope if element is found.</param>
		/// <returns>Returns address of most closely nested datum. Else returns nullptr.</returns>
		Datum* search(const std::string& t_name, Scope** t_owner = nullptr);

		/// <summary>Adds datum of the given name to the Scope if doesn't exist. If already exists, reuses it.</summary>
		/// <param name="t_name">Const reference to a name of datum to be added of type STL std::string.</param>
		/// <returns>Returns reference to a Datum in the scope which is stored with given name.</returns>
		Datum& append(const std::string& t_name);

		/// <summary>Special case of append() - Add scope to the datum stored with the given name.</summary>
		/// <param name="t_name">Const reference to a passed name of Type STL std::string for a datum which is to be added.</param>
		/// <returns>Returns reference to a Scope with associated name of added scope.</returns>
		Scope& appendScope(const std::string& t_name);

		/// <summary>Adopts the given scope and stores it in the Datum represented by given name.</summary>
		/// <param name="t_name">Const reference to name of Type STL std::string of the datum to which scope will be added</param>
		/// <param name="t_child">Reference to a scope ( Child to be adopted ).</param>
		void adopt(const std::string& t_name, Scope& t_child);

		/// <summary>Getter for Parent of this scope.</summary>
		/// <returns>Returns pointer to Scope ( parent )</returns>
		Scope* getParent() const;

		/// <summary>Wrapper for append() method - </summary>
		/// <param name="t_name">Const reference to name of the datum to be added.</param>
		/// <returns>Returns reference of the datum in this scope.Throws exception if passed string is empty.</returns>
		Datum& operator[](const std::string& t_name);

		/// <summary>Const version of operator==[string] overload - Wrapper for at() method - </summary>
		/// <param name="t_name">Const reference to name of the datum to be added.</param>
		/// <returns>Returns reference of the datum in this scope.Throws exception if passed string is empty.</returns>
		const Datum& operator[](const std::string& t_name) const;

		/// <summary>Getter and Setter for Datum added at given index in Scope.</summary>
		/// <param name="t_index">Index of the element in the scope ( as per order of insertion )</param>
		/// <returns>Reference to the datum in the scope stored at the given order index.</returns>
		Datum& operator[](const uint32_t& t_index);

		/// <summary>Const version of operator==[int] Getter and Setter for Datum added at given index in Scope.</summary>
		/// <param name="t_index">Index of the element in the scope ( as per order of insertion )</param>
		/// <returns>Reference to the datum in the scope stored at the given order index.</returns>
		const Datum& operator[](const uint32_t& t_index) const;

		/// <summary>Wrapper for find() method.</summary>
		/// <param name="t_name">Const reference to std::string of name which is to be searched.</param>
		/// <returns>Returns pointer to the datum if found. Throws exception if element isn't found.</returns>
		Datum& at(const std::string& t_name);

		/// <summary>Const version of at() method - Wrapper for find() method.</summary>
		/// <param name="t_name">Const reference to std::string of name which is to be searched.</param>
		/// <returns>Returns const reference to the datum if found. Throws exception if element isn't found.</returns>
		const Datum& at(const std::string& t_name) const;

		/// <summary>Comparison operator overload (==) for Scope. ( Deep checking of nested Scopes and children ).</summary>
		/// <param name="t_rhs">Const reference to the passed Scope which is to be compared.</param>
		/// <returns>Returns boolean value indicating whether 2 Scopes are equal or not.</returns>
		bool operator==(const Scope& t_rhs);

		/// <summary>Not Equal operator overload (!=) for Scope. ( Deep checking of nested Scopes and children ).</summary>
		/// <param name="t_rhs">Const reference to the passed Scope which is to be compared.</param>
		/// <returns>Returns boolean value indicating whether 2 Scopes are equal or not.</returns>
		bool operator!=(const Scope& t_rhs);

		/// <summary>Finds the name of the Datum the given Scope is stored in.</summary>
		/// <param name="t_child">Const reference to the Scope child which is to be searched.</param>
		/// <returns>Returns name of the Datum if found. Returns empty STL std::string otherwise.</returns>
		std::string findName(const Scope& t_child) const;

		/// <summary>Compares whether given RTTI is Scope or not. ( Overriden from RTTI Interface ).</summary>
		/// <param name="t_rhs">Const pointer to RTTI object.</param>
		/// <returns>Returns true if given RTTI is Scope. Returns false otherwise.</returns>
		virtual bool Equals(const RTTI* t_rhs);

		/// <summary>Gets name of the class ( Overriden from RTTI Interface ).</summary>
		/// <returns>Returns name of the class as Type STL std::string</returns>
		virtual std::string ToString() const;

	private:

		void doRecursiveChildrenCopy(const Scope& t_rhs);
		void fixParentPointer(Scope& t_rhs);
		void makeChildOrphan();

		static const uint32_t DEFAULT_SIZE = 15;
		Scope* m_parent;
		LookupTable m_lookup_table;
		PointersList m_pointers_list;
	};
}
