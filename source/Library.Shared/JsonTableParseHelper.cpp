#include "pch.h"
#include "JsonTableParseHelper.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonTableParseHelper::SharedData)

#pragma region SharedData
	
	JsonTableParseHelper::SharedData::SharedData(Scope& t_scope) :
		shared_data(&t_scope)
	{
	}

	Scope* JsonTableParseHelper::SharedData::getSharedData()
	{
		return shared_data;
	}

	void JsonTableParseHelper::SharedData::setSharedData(Scope& t_rhs)
	{
		shared_data = &t_rhs;
	}

#pragma endregion

#pragma region JsonTableParseHelper

	void JsonTableParseHelper::initialize()
	{
	}

	bool JsonTableParseHelper::startHandler(JsonParseMaster::SharedData & t_shared_data, const std::string & t_key, Json::Value & t_values)
	{
		t_shared_data;
		t_key;
		t_values;
		return false;
	}

	bool JsonTableParseHelper::endHandler(JsonParseMaster::SharedData & t_shared_data, const std::string & t_key)
	{
		t_shared_data;
		t_key;
		return false;
	}

	gsl::owner<IJsonParseHelper*> JsonTableParseHelper::create()
	{
		return gsl::owner<IJsonParseHelper*>();
	}

#pragma endregion

}