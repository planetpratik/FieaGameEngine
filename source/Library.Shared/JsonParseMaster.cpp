#include "pch.h"
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"
#include <sstream>
#include <fstream>


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonParseMaster::SharedData)

#pragma region SharedData

	void JsonParseMaster::SharedData::setJsonParseMaster(JsonParseMaster& t_parse_master)
	{
		json_parse_master = &t_parse_master;
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
		if (depth_counter > 0)
		{
			--depth_counter;
		}
	}

	uint32_t FieaGameEngine::JsonParseMaster::SharedData::depth() const
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

	JsonParseMaster::JsonParseMaster(JsonParseMaster&& t_rhs) :
		m_parse_helpers(std::move(t_rhs.m_parse_helpers)), m_shared_data(t_rhs.m_shared_data),
		m_is_clone(t_rhs.m_is_clone), m_file_name(t_rhs.m_file_name)
	{
		m_shared_data->setJsonParseMaster(*this);
		t_rhs.m_shared_data = nullptr;
		t_rhs.m_is_clone = false;
	}

	JsonParseMaster& JsonParseMaster::operator=(JsonParseMaster&& t_rhs)
	{
		if (this != &t_rhs)
		{
			m_parse_helpers = std::move(t_rhs.m_parse_helpers);
			m_shared_data = t_rhs.m_shared_data;
			m_is_clone = t_rhs.m_is_clone;
			m_file_name = std::move(t_rhs.m_file_name);
			m_shared_data->setJsonParseMaster(*this);
			t_rhs.m_shared_data = nullptr;
			t_rhs.m_is_clone = false;
		}
		return *this;
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
		gsl::owner<JsonParseMaster*> clone = new JsonParseMaster(*m_shared_data->create());
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
		auto it = std::find_if(m_parse_helpers.begin(), m_parse_helpers.end(), [&t_parse_helper](const IJsonParseHelper* h)
		{
			return (&t_parse_helper == h) || (t_parse_helper.TypeIdInstance() == h->TypeIdInstance());
		});
		if (it != m_parse_helpers.end())
		{
			throw std::exception("Invalid Operation! This helper or one of the same type has already been added to JsonParseMaster");
		}
		m_parse_helpers.pushBack(&t_parse_helper);
	}

	bool JsonParseMaster::removeHelper(IJsonParseHelper& t_helper_to_remove)
	{
		bool result = false;
		result = m_parse_helpers.remove(&t_helper_to_remove);
		if (m_is_clone)
		{
			delete &t_helper_to_remove;
		}
		return result;
	}

	void JsonParseMaster::parse(const std::string& t_json_string)
	{
		std::stringstream string_stream;
		string_stream << t_json_string;
		parse(string_stream);
	}

	void JsonParseMaster::parse(std::istream& t_json_input_stream)
	{
		Json::Value root;
		t_json_input_stream >> root;
		m_shared_data->incrementDepth();
		parseValue(root);
		m_shared_data->decrementDepth();
	}

	void JsonParseMaster::parseFromFile(const std::string& t_file_name)
	{
		std::ifstream json_file(t_file_name);
		if (json_file.bad())
		{
			throw std::exception("Invalid Operation! Unable to open file.");
		}
		m_file_name = t_file_name;
		parse(json_file);
	}

	const std::string& JsonParseMaster::getFileName() const
	{
		return m_file_name;
	}

	JsonParseMaster::SharedData* JsonParseMaster::getSharedData() const
	{
		return m_shared_data;
	}

	void JsonParseMaster::setSharedData(SharedData& t_shared_data)
	{
		if (m_is_clone)
		{
			throw std::exception("Invalid Operation! Cannot change Shared Data on Cloned JsonParseMaster.");
		}
		m_shared_data = &t_shared_data;
		m_shared_data->setJsonParseMaster(*this);
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

	void JsonParseMaster::parseValue(const Json::Value& t_value)
	{
		const std::vector<std::string> keys = t_value.getMemberNames();
		for (const std::string& key : keys)
		{
			parseKeyValuePair(key, t_value[key]);
		}
	}

	void JsonParseMaster::parseKeyValuePair(const std::string& t_key, const Json::Value& t_value, bool t_is_array_element, uint32_t t_index, uint32_t t_array_size)
	{
		if (t_value.isObject())
		{
			m_shared_data->incrementDepth();
			for (IJsonParseHelper* helper : m_parse_helpers)
			{
				if (helper->startHandler(*m_shared_data, t_key, t_value, t_is_array_element, t_index, t_array_size))
				{
					parseValue(t_value);
					helper->endHandler(*m_shared_data, t_key);
					break;
				}
			}
			m_shared_data->decrementDepth();
		}
		else if (t_value.isArray())
		{
			uint32_t i = 0;
			for (const auto& element : t_value)
			{
				parseKeyValuePair(t_key, element, true, i, t_value.size());
				++i;
			}
		}
		else
		{
			for (IJsonParseHelper* helper : m_parse_helpers)
			{
				if (helper->startHandler(*m_shared_data, t_key, t_value, t_is_array_element, t_index, t_array_size))
				{
					helper->endHandler(*m_shared_data, t_key);
					break;
				}
			}
		}
	}
#pragma endregion	
}