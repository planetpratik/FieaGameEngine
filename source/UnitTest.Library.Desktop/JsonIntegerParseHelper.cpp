#include "pch.h"
#include "JsonIntegerParseHelper.h"

using namespace std;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
#pragma region SharedData
	RTTI_DEFINITIONS(JsonIntegerParseHelper::SharedData)

	void JsonIntegerParseHelper::SharedData::initialize()
	{
		max_depth = 0;
	}

	gsl::owner<JsonIntegerParseHelper::SharedData*> JsonIntegerParseHelper::SharedData::create() const
	{
		return new SharedData();
	}
#pragma endregion

#pragma region JsonTestParseHelper

	RTTI_DEFINITIONS(JsonIntegerParseHelper)

	gsl::owner<IJsonParseHelper*> JsonIntegerParseHelper::create() const
	{
		return new JsonIntegerParseHelper();
	}

	void JsonIntegerParseHelper::initialize()
	{
		IJsonParseHelper::initialize();
		is_initialized_called = true;
		start_handler_count = 0;
		end_handler_count = 0;
	}

	bool JsonIntegerParseHelper::startHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key, Json::Value& t_values)
	{
		JsonIntegerParseHelper::SharedData* shared_data = t_shared_data.As<JsonIntegerParseHelper::SharedData>();
		if (shared_data == nullptr)
		{
			return false;
		}
		if (t_key == "integer")
		{
			++start_handler_count;
			Json::Value value = t_values;
			Json::ValueType value_type = value.type();
			if (value_type == Json::ValueType::intValue)
			{
				shared_data->int_value = value.asInt();
				return true;
			}
			return false;
		}
		return false;
	}


	bool JsonIntegerParseHelper::endHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key)
	{
		t_key;
		JsonIntegerParseHelper::SharedData* shared_data = t_shared_data.As<JsonIntegerParseHelper::SharedData>();
		if (shared_data == nullptr)
		{
			return false;
		}
		++end_handler_count;
		return true;
	}

#pragma endregion

}