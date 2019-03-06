#pragma once
#include "IJsonParseHelper.h"

namespace FieaGameEngine
{
	class Scope;

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
			virtual void initialize();
			virtual gsl::owner<SharedData*> create() = 0;
			Scope* getSharedData();
			void setSharedData(Scope& t_rhs);
		private:
			Scope* shared_data = nullptr;
		};

		JsonTableParseHelper() = default;
		virtual ~JsonTableParseHelper() = default;
		JsonTableParseHelper(const JsonTableParseHelper& t_rhs) = default;
		JsonTableParseHelper(JsonTableParseHelper&& t_rhs) = default;
		JsonTableParseHelper& operator=(const JsonTableParseHelper& t_rhs) = default;
		JsonTableParseHelper& operator=(JsonTableParseHelper&& t_rhs) = default;
		void initialize();
		bool startHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key, Json::Value& t_values);
		bool endHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key);
		gsl::owner<IJsonParseHelper*> create();

	private:

	};

}