#pragma once
#include <cstdint>
#include "Datum.h"
#include "HashMap.h"

namespace FieaGameEngine
{
	/// <summary>A structure that represents prescribed attributes.</summary>
	struct Signature final
	{
		/// <summary>String indicating key to use in the datum.</summary>
		std::string sig_name;

		/// <summary>One of the values in the datum type enumeration.</summary>
		Datum::DatumType sig_type;

		/// <summary>Number of elements in this datum.</summary>
		uint32_t sig_size;

		/// <summary>Offset to the class data member.</summary>
		size_t sig_offset;

		/// <summary>Comparison operator (==) overload for Signature.</summary>
		/// <param name="rhs">Const reference to the signature which is to be compared with.</param>
		/// <returns>Boolean result indicating whether two signatures are equal or not.</returns>
		bool operator==(const Signature& t_rhs)
		{
			return sig_name == t_rhs.sig_name &&
				sig_type == sig_type &&
				sig_size == sig_size &&
				sig_offset == sig_offset;
		}

		/// <summary>Not Equals operator overload for Signature.</summary>
		/// <param name="t_rhs">Const reference to the signature which is to be compared with.</param>
		/// <returns>Boolean result indicating whether two signatures are equal or not.</returns>
		bool operator!=(const Signature& t_rhs)
		{
			return !(operator==(t_rhs));
		}
	};

	class TypeManager
	{
	public:
		TypeManager() = delete;
		TypeManager(const TypeManager& t_rhs) = delete;
		TypeManager(TypeManager&& t_rhs) = delete;
		TypeManager& operator=(const TypeManager& t_rhs) = delete;
		TypeManager& operator=(TypeManager&& t_rhs) = delete;
		
		/// <summary>Static function to register prescribed signatures into HashMap.</summary>
		/// <param name="t_type_id">Const reference to Runtime TypeID of type Unsigned integer ( 64-bit )</param>
		/// <param name="signatures">Const reference to Vector of signatures.</param>
		static void registerType(const uint64_t& t_type_id, const Vector<Signature>& signatures);

		/// <summary>Static function to Remove Vector of Signatures associated with specific Runtime TypeID from HashMap.</summary>
		/// <param name="t_type_id">Const reference to Runtime TypeID of type Unsigned integer ( 64-bit )</param>
		static void removeType(uint64_t& t_type_id);

		/// <summary>Static function to get signatures stored in HashMap for given Runtime TypeID</summary>
		/// <param name="t_type_id">Const reference to Runtime TypeID of type Unsigned integer ( 64-bit )</param>
		/// <returns>Returns Vector of prescribed signatures.</returns>
		static Vector<Signature>& getSignatures(const uint64_t& t_type_id);

		/// <summary>Static function to clear hashmap which is used to store prescribed signatures.</summary>
		static void clear();
		
	private:
		static HashMap<uint64_t, Vector<Signature>> type_map;
	};
}