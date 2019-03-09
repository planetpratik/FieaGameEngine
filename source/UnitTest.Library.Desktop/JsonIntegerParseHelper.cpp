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
		data.clear();
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
		array_element_count = 0;
	}

	bool JsonIntegerParseHelper::startHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key, const Json::Value& t_values, bool t_is_array_element, uint32_t t_index, uint32_t t_array_size)
	{
		t_is_array_element;
		t_index;
		t_array_size;
		JsonIntegerParseHelper::SharedData* shared_data = t_shared_data.As<JsonIntegerParseHelper::SharedData>();
		
		if (shared_data == nullptr)
		{
			return false;
		}

		if (t_key != integerElementName)
		{
			return false;
		}

		if (m_parsing_data)
		{
			throw std::exception("Invalid Operation! Received another start element while parsing an integer.");
		}
		
		m_parsing_data = true;
		shared_data->data.pushBack(t_values.asInt());
		return true;
	}


	bool JsonIntegerParseHelper::endHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key)
	{
		JsonIntegerParseHelper::SharedData* shared_data = t_shared_data.As<JsonIntegerParseHelper::SharedData>();
		if (nullptr == shared_data || t_key != integerElementName || false == m_parsing_data)
		{
			return false;
		}
		++end_handler_count;
		m_parsing_data = false;
		return true;
	}

#pragma endregion

}