#pragma once
#include "IJsonParseHelper.h"
#include "Scope.h"
#include "Stack.h"

namespace FieaGameEngine
{

	class JsonTableParseHelper : public IJsonParseHelper
	{
	public:

		class SharedData : public JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedData, JsonParseMaster::SharedData)

			friend class JsonTableParseHelper;

		public:
			explicit SharedData(Scope& t_scope);
			~SharedData() = default;
			virtual void initialize() override;
			virtual gsl::owner<SharedData*> create() const override;
			Scope* getSharedData();
			void setSharedData(Scope& t_rhs);
		private:
			Scope* shared_scope = nullptr;
		};

		JsonTableParseHelper() = default;
		virtual ~JsonTableParseHelper() = default;
		JsonTableParseHelper(const JsonTableParseHelper& t_rhs) = default;
		JsonTableParseHelper(JsonTableParseHelper&& t_rhs) = default;
		JsonTableParseHelper& operator=(const JsonTableParseHelper& t_rhs) = default;
		JsonTableParseHelper& operator=(JsonTableParseHelper&& t_rhs) = default;
		virtual void initialize();
		virtual bool startHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key, const Json::Value& t_values, bool t_is_array_element, uint32_t t_index, uint32_t array_size);
		virtual bool endHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key);
		virtual gsl::owner<IJsonParseHelper*> create() const override;

	private:
		typedef struct
		{
			std::string key;
			std::string type;
			Json::Value value;
		}Data;

		Data element, array_element;
		Stack<std::string> key_stack;
		bool m_parsing_data = false;
		bool is_array_completely_parsed = true;

		// Unused.. Should be implemented to separate tasks in StartHandler. To make it small.
		const HashMap<std::string, Datum::DatumType> string_type_map = 
		{
			{"integer", Datum::DatumType::INTEGER},
			{"float", Datum::DatumType::FLOAT},
			{"string", Datum::DatumType::STRING},
			{"vector4", Datum::DatumType::VECTOR4},
			{"matrix4x4", Datum::DatumType::MATRIX4X4}
		};
	};

}