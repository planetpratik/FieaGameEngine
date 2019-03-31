#pragma once
#include "IJsonParseHelper.h"
#include "Scope.h"
#include "Stack.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Table Parse Helper class - Helper for parsing Scopes and Attributed objects from JSON.
	/// Use of Stack to store keys provides pushdown automata sort of solution to populate Scope.
	/// </summary>
	class JsonTableParseHelper : public IJsonParseHelper
	{

	public:
		/// <summary>
		/// Embedded SharedData class which holds Scope pointer (root) to store parsed data.
		/// </summary>
		class SharedData : public JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedData, JsonParseMaster::SharedData)

			/// <summary>Allows JsonTableParseHelper to access private members of SharedData.</summary>
			friend class JsonTableParseHelper;

		public:
			/// <summary>Constructor for Shared Data - Constructs SharedData from existing scope object (Explicit).</summary>
			/// <param name="t_scope">Reference to passed Scope.</param>
			explicit SharedData(Scope& t_scope);

			/// <summary>Destructor for SharedData (Default).</summary>
			~SharedData() = default;

			/// <summary>Initializer for SharedData (Overrided).</summary>
			virtual void initialize() override;

			/// <summary>Duplicate SharedData object (Clone).</summary>
			/// <returns>Returns Non-Null pointer to JsonParseMaster ( new instance ).</returns>
			virtual gsl::owner<SharedData*> create() const override;

			/// <summary>Getter for SharedData.</summary>
			/// <returns>Returns Scope pointer stored in SharedData.</returns>
			Scope* getSharedData();

			/// <summary>Setter for SharedData.</summary>
			/// <param name="t_rhs">Reference to Scope which is to be set.</param>
			void setSharedData(Scope& t_rhs);

		private:
			Scope* shared_scope = nullptr;
		};

		/// <summary>Default Constructor for JsonTableParseHelper - (Default).</summary>
		JsonTableParseHelper() = default;

		/// <summary>Virtual Destructor for JsonTableParseHelper - (Default).</summary>
		virtual ~JsonTableParseHelper() = default;

		/// <summary>Copy constructor for JsonTableParseHelper (Default).</summary>
		/// <param name="t_rhs">Const reference to passed JsonTableParseHelper.</param>
		JsonTableParseHelper(const JsonTableParseHelper& t_rhs) = default;

		/// <summary>Move constructor for JsonTableParseHelper (Default).</summary>
		/// <param name="t_rhs">R-Value reference to passed JsonTableParseHelper which is to be moved.</param>
		JsonTableParseHelper(JsonTableParseHelper&& t_rhs) = default;

		/// <summary>Copy Assignment operator for JsonTableParseHelper (Default).</summary>
		/// <param name="t_rhs">Const reference to passed JsonTableParseHelper.</param>
		/// <returns>Returns reference to a JsonTableParseHelper.</returns>
		JsonTableParseHelper& operator=(const JsonTableParseHelper& t_rhs) = default;

		/// <summary>Move Assignment operator for JsonTableParseHelper (Default).</summary>
		/// <param name="t_rhs">R-Value reference to JsonTableParseHelper which is to be moved.</param>
		/// <returns>Returns reference to a JsonTableParseHelper.</returns>
		JsonTableParseHelper& operator=(JsonTableParseHelper&& t_rhs) = default;

		/// <summary>Initialize method for JsonTableParseHelper. Initializes IJsonParser and clears Stack.</summary>
		virtual void initialize() override;

		/// <summary>Determines if and how to "handle" the start of a name/value pair. (Overriden)</summary>
		/// <param name="t_shared_data">Passed Shared Data Reference</param>
		/// <param name="t_key">Const reference to key of type STL std::string.</param>
		/// <param name="t_values">Json Value associated with the given key.</param>
		/// <param name="t_is_array_element">Boolean value indicating whether given value is an array element or not.</param>
		/// <param name="t_index">Index of an array if parsing Array.</param>
		/// <param name="array_size">Size of an Array if parsing Array.</param>
		/// <returns>Returns boolean value indicating whether given helper can handle the request or not.</returns>
		virtual bool startHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key, const Json::Value& t_values, bool t_is_array_element, uint32_t t_index, uint32_t array_size) override;
		
		/// <summary>Determines if and how to "handle" the end of a name/value pair.(Overriden)</summary>
		/// <param name="t_shared_data">Passed Shared Data Reference</param>
		/// <returns>Returns boolean value indicating whether given helper can handle the request or not.</returns>
		virtual bool endHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key) override;
		
		/// <summary>Create clone of an JsonTableParseHelper (Overriden)</summary>
		/// <returns>Returns IJsonParseHelper pointer.</returns>
		virtual gsl::owner<IJsonParseHelper*> create() const override;

		/// <summary>Gets size of a Key Stack.</summary>
		/// <returns>Returns Size of a Stack as a std::size_t</returns>
		size_t stackSize() const;

	private:

		struct Data
		{
			std::string key;
			std::string type;
			std::string class_name = "";
			std::string attribute_name = "";
			Json::Value value;
		};

		Data element;
		Stack<std::string> key_stack;
		Stack<std::string> type_stack;
		bool m_parsing_data = false;
		bool m_is_array_size_already_assigned = false;
		int32_t m_array_elements_to_parse = 0;

	};

}