#pragma once
#include "RTTI.h"
#include "Vector.h"
#include <json/json.h>
#include <gsl/gsl>
namespace FieaGameEngine
{
	/// <summary>Forward reference declaration of IJsonParseHelper.</summary>
	class IJsonParseHelper;

	/// <summary>
	/// C++ Wrapper for JsonCpp functionality.
	/// Implements chain of responsibility pattern to handle parsed data.
	/// </summary>
	class JsonParseMaster final
	{
	public:
		/// <summary>Embedded SharedData class which contains Intermediate data while parsing</summary>
		class SharedData : public RTTI
		{
			/// <summary>Allows JsonParseMaster to access private members of SharedData.</summary>
			friend class JsonParseMaster;

			RTTI_DECLARATIONS(JsonParseMaster::SharedData, RTTI)

		public:
			/// <summary>Default constructor for SharedData (Default).</summary>
			SharedData() = default;

			/// <summary>Virtual destructor for SharedData (Default).</summary>
			virtual ~SharedData() = default;

			/// <summary>Get JsonParseMaster associated with this address.</summary>
			/// <returns>Returns pointer to JsonParseMaster associated with this address.</returns>
			JsonParseMaster* getJsonParseMaster();

			/// <summary>Increment nesting-depth counter. Gets incremented upon start of each element.</summary>
			void incrementDepth();

			/// <summary>Decrement nesting-depth counter. Gets decremented upon end of each element.</summary>
			void decrementDepth();

			/// <summary>To get current nesting depth.</summary>
			/// <returns>Returns current nesting depth in Unsigned Integer (32-bit)</returns>
			uint32_t depth() const;

			/// <summary>Initialize SharedData ( Pure Virtual Function ).</summary>
			virtual void initialize() = 0;

			/// <summary>Overridden implementation will create an instance of the SharedData ( Pure Virtual Function ).</summary>
			/// <returns>Returns Non-Null pointer to SharedData ( new instance ).</returns>
			virtual gsl::owner<SharedData*> create() const = 0;
			
		protected:
			uint32_t depth_counter = 0;
		private:
			JsonParseMaster* json_parse_master = nullptr;
			void setJsonParseMaster(JsonParseMaster& t_parse_master);
		};

		/// <summary>Given a reference to SharedData object, Initialize JsonParseMaster.</summary>
		/// <param name="t_data">Reference to passed SharedData.</param>
		explicit JsonParseMaster(SharedData& t_data);

		/// <summary>Copy Constructor for JsonParseMaster (Deleted).</summary>
		/// <param name="t_rhs">Const reference to passed JsonParseMaster.</param>
		JsonParseMaster(const JsonParseMaster& t_rhs) = delete;

		/// <summary>Move constructor for JsonParseMaster.</summary>
		/// <param name="t_rhs">R-Value reference to a passed JsonParseMaster which is to be moved.</param>
		JsonParseMaster(JsonParseMaster&& t_rhs);

		/// <summary>Copy Assignment operator for JsonParseMaster (Deleted).</summary>
		/// <param name="t_rhs">Const reference to passed JsonParseMaster.</param>
		/// <returns>Reference to current JsonParseMaster.</returns>
		JsonParseMaster& operator=(const JsonParseMaster& t_rhs) = delete;

		/// <summary>Move Assignmenht operator for JsonParseMaster.</summary>
		/// <param name="t_rhs">R-Value reference to a passed JsonParseMaster which is to be moved.</param>
		/// <returns>Reference to current JsonParseMaster.</returns>
		JsonParseMaster& operator=(JsonParseMaster&& t_rhs);

		/// <summary>Destructor for JsonParseMaster.</summary>
		~JsonParseMaster();

		/// <summary>Duplicate JsonParseMaster object (clone).</summary>
		/// <returns>Returns Non-Null pointer to JsonParseMaster ( new instance ).</returns>
		gsl::owner<JsonParseMaster*> clone();

		/// <summary>Given a reference to an IJsonParseHelper object, add it to the helpers list.</summary>
		/// <param name="t_parse_helper">Reference to IJsonParseHelper which is to be added.</param>
		void addHelper(IJsonParseHelper& t_parse_helper);

		/// <summary>Given a reference to an IJsonParseHelper object, remove it from helpers list.</summary>
		/// <param name="t_helper_to_remove">Reference to IJsonParseHelper which is to be removed from list.</param>
		/// <returns>Returns boolean value indicating whether helper is removed from the list or not.</returns>
		bool removeHelper(IJsonParseHelper& t_helper_to_remove);

		/// <summary>Parse a given string of JSON data.</summary>
		/// <param name="t_json_string">Const reference to a STL std::string representing JSON data.</param>
		void parse(const std::string& t_json_string);

		/// <summary>Parse a given input-stream of JSON data.</summary>
		/// <param name="t_json_input_stream">istream reference representing JSON data.</param>
		void parse(std::istream& t_json_input_stream);

		/// <summary>Given a filename, Read the file and parse JSON data.</summary>
		/// <param name="t_file_name">Const reference to a STL std::string indicating a name of file from which JSON data is to be parsed.</param>
		void parseFromFile(const std::string& t_file_name);
		
		/// <summary>Get path of the file being parsed. (Useful for errors encountered during parsing).</summary>
		/// <returns>Returns Const reference to STL std::string representing name of file being parsed.</returns>
		const std::string& getFileName() const;

		/// <summary>Gets SharedData associated with this object.</summary>
		/// <returns>Returns address of a SharedData associated with this object.<returns>
		SharedData* getSharedData() const;

		/// <summary>Sets SharedData associated to this object.</summary>
		/// <param name="t_shared_data">Passed Shared Data reference.</param>
		void setSharedData(SharedData& t_shared_data);

		/// <summary>Initialize JsonParseMaster. Initializes SharedData as well as helpers.</summary>
		void initialize();

		/// <summary>To check whether given JsonParseMaster is clone or not.</summary>
		/// <returns>Returns boolean value indicating whether given JsonParseMaster is clone or not.</returns>
		bool isClone() const;

		/// <summary>Getter for List of helpers.</summary>
		/// <returns>Returns const reference to list of IJsonParseHelper pointers ( helpers ).</returns>
		const Vector<IJsonParseHelper*>& getHelpers() const;
	private:
		SharedData* m_shared_data = nullptr;
		bool m_is_clone = false;
		Vector<IJsonParseHelper*> m_parse_helpers;
		std::string m_file_name;

		void parseValue(const Json::Value& t_value);
		void parseKeyValuePair(const std::string& t_key, const Json::Value& t_value, bool t_is_array_element = false, std::size_t t_index = 0);
	};

}
