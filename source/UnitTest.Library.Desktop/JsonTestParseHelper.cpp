#include "pch.h"
#include "JsonTestParseHelper.h"

using namespace std;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
#pragma region SharedData
	RTTI_DEFINITIONS(JsonTestParseHelper::SharedData)

	void JsonTestParseHelper::SharedData::initialize()
	{
		max_depth = 0;
	}

	gsl::owner<JsonTestParseHelper::SharedData*> JsonTestParseHelper::SharedData::create() const
	{
		return new SharedData();
	}
#pragma endregion

#pragma region JsonTestParseHelper

	RTTI_DEFINITIONS(JsonTestParseHelper)

	gsl::owner<IJsonParseHelper*> JsonTestParseHelper::create() const
	{
		return new JsonTestParseHelper();
	}

	void JsonTestParseHelper::initialize()
	{
		IJsonParseHelper::initialize();
		is_initialized_called = true;
		start_handler_count = 0;
		end_handler_count = 0;
		array_element_count = 0;
	}

	bool JsonTestParseHelper::startHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key, const Json::Value& t_values, bool t_is_array_element, size_t t_index)
	{
		t_key;
		t_values;
		t_index;
		JsonTestParseHelper::SharedData* shared_data = t_shared_data.As<JsonTestParseHelper::SharedData>();
		if (shared_data == nullptr)
		{
			return false;
		}
		++start_handler_count;

		if (t_is_array_element)
		{
			++array_element_count;
		}

		if (shared_data->depth() > shared_data->max_depth)
		{
			shared_data->max_depth = shared_data->depth();
		}

		return true;
	}


	bool JsonTestParseHelper::endHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key)
	{
		t_key;
		JsonTestParseHelper::SharedData* shared_data = t_shared_data.As<JsonTestParseHelper::SharedData>();
		if (shared_data == nullptr)
		{
			return false;
		}
		++end_handler_count;
		return true;
	}

#pragma endregion

}