#include "pch.h"
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"
#include <sstream>
#include <fstream>


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonParseMaster::SharedData)

#pragma region SharedData

	void JsonParseMaster::SharedData::setJsonParseMaster(const JsonParseMaster& t_parse_master)
	{
		json_parse_master = const_cast<JsonParseMaster*>(&t_parse_master);
	}

	JsonParseMaster* JsonParseMaster::SharedData::getJsonParseMaster()
	{
		return json_parse_master;
	}

	void JsonParseMaster::SharedData::incrementDepth()
	{
		++depth_counter;
	}

	void FieaGameEngine::JsonParseMaster::SharedData::decrementDepth()
	{
		--depth_counter;
	}

	uint32_t FieaGameEngine::JsonParseMaster::SharedData::depth()
	{
		return depth_counter;
	}

#pragma endregion

#pragma region JsonParseMaster

	FieaGameEngine::JsonParseMaster::JsonParseMaster(SharedData& t_data) :
		m_shared_data(&t_data)
	{
		t_data.setJsonParseMaster(*this);
	}

	JsonParseMaster::~JsonParseMaster()
	{
		if (m_is_clone)
		{
			for (auto& helper : m_parse_helpers)
			{
				delete helper;
			}
			delete m_shared_data;
		}
	}

	gsl::owner<JsonParseMaster*> JsonParseMaster::clone()
	{
		gsl::owner<JsonParseMaster*> clone = new JsonParseMaster();
		if (m_shared_data)
		{
			const auto& shared_data = m_shared_data->create();
			clone->m_shared_data = shared_data;
		}
		for (auto& helper : m_parse_helpers)
		{
			clone->m_parse_helpers.pushBack(helper->create());
		}
		clone->m_is_clone = true;
		return clone;
	}

	void JsonParseMaster::addHelper(IJsonParseHelper& t_parse_helper)
	{
		if (m_is_clone)
		{
			throw std::exception("Invalid Operation! Cannot add helper to clone");
		}
		m_parse_helpers.pushBack(&t_parse_helper);
	}

	bool JsonParseMaster::removeHelper(IJsonParseHelper & t_helper_to_remove)
	{
		auto itr = m_parse_helpers.find(&t_helper_to_remove);
		return m_parse_helpers.remove(itr);
	}

	void JsonParseMaster::parse(const std::string& t_json_string)
	{
		Json::Value root;
		std::stringstream string_stream;
		string_stream << t_json_string;
		string_stream >> root;

		parseMembers("", root);
	}

	void JsonParseMaster::parse(std::stringstream& t_json_input_stream)
	{
		Json::Value root;
		t_json_input_stream >> root;

		parseMembers("", root);
	}

	void JsonParseMaster::parseFromFile(const std::string& t_file_name)
	{
		std::ifstream json_file("Content/" + t_file_name);
		
		if (json_file.bad())
		{
			throw std::exception("Invalid Operation! Unable to open file.");
		}
		std::stringstream stream;
		stream << json_file.rdbuf();
		m_file_name = t_file_name;
		parse(stream);
	}

	const std::string& JsonParseMaster::getFileName() const
	{
		return m_file_name;
	}

	JsonParseMaster::SharedData* JsonParseMaster::getSharedData() const
	{
		return m_shared_data;
	}

	void JsonParseMaster::initialize()
	{
		m_shared_data->initialize();
		for (auto& helper : m_parse_helpers)
		{
			helper->initialize();
		}
		m_file_name.clear();
	}

	bool JsonParseMaster::isClone() const
	{
		return m_is_clone;
	}

	const Vector<IJsonParseHelper*>& JsonParseMaster::getHelpers() const
	{
		return m_parse_helpers;
	}

	IJsonParseHelper* JsonParseMaster::parseMembers(const std::string& t_key, Json::Value& t_json_value)
	{
		if (t_json_value.isObject() || t_json_value.isArray())
		{
			IJsonParseHelper* temp = nullptr;
			if (t_key != "")
			{
				m_shared_data->incrementDepth();
				for (auto& helper : m_parse_helpers)
				{
					if (helper->startHandler(*m_shared_data, t_key, t_json_value))
					{
						temp = helper;
						break;
					}
				}
			}
			for (auto& member_name : t_json_value.getMemberNames())
			{
				temp = parseMembers(member_name, t_json_value[member_name]);
			}
			if (temp != nullptr)
			{
				temp->endHandler(*m_shared_data, t_key);
				m_shared_data->decrementDepth();
			}
		}
		else
		{
			for (auto& helper : m_parse_helpers)
			{
				if (helper->startHandler(*m_shared_data, t_key, t_json_value))
				{
					helper->endHandler(*m_shared_data, t_key);
					return helper;
				}
			}
		}
		return nullptr;
	}
#pragma endregion	
}