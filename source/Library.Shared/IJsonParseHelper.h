#pragma once
#include "JsonParseMaster.h"

namespace FieaGameEngine
{
	/// <summary>Abstract Helper class through which all helpers are derived.</summary>
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI)

	public:

		/// <summary>Constructor for IJsonParseHelper - (Default).</summary>
		IJsonParseHelper() = default;

		/// <summary>Copy Constructor for IJsonParseHelper - (Default).</summary>
		/// <param name="t_rhs">Const reference to IJsonParseHelper.</param>
		IJsonParseHelper(const IJsonParseHelper& t_rhs) = default;

		/// <summary>Move Constructor for IJsonParseHelper - (Default).</summary>
		/// <param name="t_rhs">R-Value reference to passed IJsonParseHelper which is to be moved.</param>
		IJsonParseHelper(IJsonParseHelper&& t_rhs) = default;

		/// <summary>Copy Assignment operator for IJsonParseHelper - (Default).</summary>
		/// <param name="t_rhs">Const Reference to an IJsonParseHelper.</param>
		/// <returns>Reference to current IJsonParseHelper.</returns>
		IJsonParseHelper& operator=(const IJsonParseHelper& t_rhs) = default;

		/// <summary>Move Assignment operator for IJsonParseHelper - (Default).</summary>
		/// <param name="t_rhs">R-Value reference to a passed IJsonParseHelper which is to be moved.</param>
		/// <returns>Reference to current IJsonParseHelper.</returns>
		IJsonParseHelper& operator=(IJsonParseHelper&& t_rhs) = default;

		/// <summary>Destructor of IJsonParseHelper.</summary>
		~IJsonParseHelper() = default;

		/// <summary>Initialize IJsonParsehelper.</summary>
		virtual void initialize();

		/// <summary>Checks if given helper can handle the key-value pair ( Pure Virtual function ).</summary>
		/// <param name="t_shared_data">Passed Shared Data Reference</param>
		/// <param name="t_key">Const reference to key of type STL std::string.</param>
		/// <param name="t_values">Json Value associated with the given key.</param>
		/// <param name="t_is_array_element">Boolean value indicating whether given value is an array element or not.</param>
		/// <param name="t_index">Index of an array</param>
		/// <param name="array_size">Size of an Array if parsing Array.</param>
		/// <returns>Returns boolean value indicating whether given helper can handle the request or not.</returns>
		virtual bool startHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key, const Json::Value& t_values, bool t_is_array_element, uint32_t t_index, uint32_t array_size) = 0;
		
		/// <summary>Attempts to complete handling of given key-value pair.</summary>
		/// <param name="t_shared_data">Passed Shared Data Reference</param>
		/// <param name="t_key">Const reference to key of type STL std::string.</param>
		/// <returns>Returns boolean value indicating whether given helper can handle the request or not.</returns>
		virtual bool endHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key) = 0;
		
		/// <summary>Overridden implementation will create an instance of the helper.</summary>
		/// <returns>Returns Non-Null pointer to IJsonParseHelper ( new instance ).</returns>
		virtual gsl::owner<IJsonParseHelper*> create() const = 0;
	};

}
