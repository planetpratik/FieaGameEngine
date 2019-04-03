#include "pch.h"
#include "JsonTableParseHelper.h"
#include "Sector.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonTableParseHelper::SharedData)

#pragma region SharedData

	JsonTableParseHelper::SharedData::SharedData(Scope& t_scope) : shared_scope(&t_scope)
	{
	}

	void JsonTableParseHelper::SharedData::initialize()
	{
	}

	gsl::owner<JsonTableParseHelper::SharedData*> JsonTableParseHelper::SharedData::create() const
	{
		Scope* scope = new Scope();
		return new SharedData(*scope);
	}

	Scope* JsonTableParseHelper::SharedData::getSharedData()
	{
		return shared_scope;
	}

	void JsonTableParseHelper::SharedData::setSharedData(Scope& t_rhs)
	{
		shared_scope = &t_rhs;
	}

#pragma endregion

#pragma region JsonTableParseHelper

	void JsonTableParseHelper::initialize()
	{
		IJsonParseHelper::initialize();
	}


	bool JsonTableParseHelper::startHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key, const Json::Value& t_values, bool t_is_array_element, uint32_t t_index, uint32_t t_array_size)
	{
		JsonTableParseHelper::SharedData* shared_data = t_shared_data.As<JsonTableParseHelper::SharedData>();
		if (shared_data == nullptr)
		{
			return false;
		}

		if (t_values.isObject())
		{
			if (t_is_array_element)
			{
				if (!m_is_array_size_already_assigned)
				{
					m_is_array_size_already_assigned = true;
					m_array_elements_to_parse = t_array_size;
				}
				if (m_is_array_size_already_assigned)
				{
					element.key = key_stack.top();
				}
			}
			if (t_key == "Value")
			{
				return true;
			}
			element.key = t_key;
			key_stack.push(t_key);
			return true;
		}

		if (t_key == "Class")
		{
			element.class_name = t_values.asString();
		}

		if (t_key == "Type")
		{
			element.type = t_values.asString();
			if (!element.class_name.empty())
			{
				Scope* nested_scope = Factory<Scope>::create(element.class_name)->As<Scope>();
				assert(nested_scope != nullptr);
				shared_data->shared_scope->adopt(element.key, *nested_scope);
				element.class_name = "";
				return true;
			}
			if (element.class_name.empty() && element.type == "table")
			{
				shared_data->shared_scope = &shared_data->shared_scope->appendScope(element.key);
				return true;
			}
			Datum& datum = shared_data->shared_scope->append(element.key);
			if (element.type == "integer")
			{
				datum.setType(Datum::DatumType::INTEGER);
			}
			else if (element.type == "float")
			{
				datum.setType(Datum::DatumType::FLOAT);
			}
			else if (element.type == "vector4")
			{
				datum.setType(Datum::DatumType::VECTOR4);
			}
			else if (element.type == "matrix4x4")
			{
				datum.setType(Datum::DatumType::MATRIX4X4);
			}
			else if (element.type == "string")
			{
				datum.setType(Datum::DatumType::STRING);
			}

			if (!datum.isExternal())
			{
				datum.setSize(datum.size() + 1);
			}
		}

		if (t_key == "Value")
		{
			element.value = t_values;
			Datum& datum = shared_data->shared_scope->append(element.key);

			if (t_is_array_element)
			{
				if (!datum.isExternal() && (0 == t_index))
				{
					datum.setSize(element.value.size());
					m_array_elements_to_parse = t_array_size;
				}
				datum.setFromString(t_values.asString(), t_index);
				--m_array_elements_to_parse;
			}
			else
			{
				datum.setFromString(t_values.asString(), (datum.size() - 1));
			}
		}
		return true;
	}






	//bool JsonTableParseHelper::startHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key, const Json::Value& t_values, bool t_is_array_element, uint32_t t_index, uint32_t t_array_size)
	//{
	//	JsonTableParseHelper::SharedData* shared_data = t_shared_data.As<JsonTableParseHelper::SharedData>();
	//	if (shared_data == nullptr)
	//	{
	//		return false;
	//	}

	//	if (t_values.isObject())
	//	{
	//		if (t_is_array_element)
	//		{
	//			if (!m_is_array_size_already_assigned)
	//			{
	//				m_is_array_size_already_assigned = true;
	//				m_array_elements_to_parse = t_array_size;
	//			}
	//		}
	//		element.key = t_key;
	//		key_stack.push(t_key);
	//		return true;
	//	}

	//	if (t_key == "Class")
	//	{
	//		element.class_name = t_values.asString();
	//		if (element.class_name == "Sector")
	//		{
	//			//shared_data->shared_scope = shared_data->shared_scope->getParent()->As<World>()->createSector(element.class_name);
	//			auto sector_scope = Factory<Scope>::create();
	//			shared_data->shared_scope = Factory<Scope>::create();
	//			element.class_name = "";
	//			element.attribute_name = "";
	//		}
	//	}

	//	if (t_key == "Type")
	//	{
	//		element.type = t_values.asString();
	//		
	//		if (element.type == "table")
	//		{
	//			if (m_array_elements_to_parse != 0)
	//			{
	//				auto stack_top = key_stack.top();
	//				if (stack_top == "Value" && element.key == "Value")
	//				{
	//					shared_data->shared_scope = shared_data->shared_scope->getParent();
	//				}
	//			}
	//			shared_data->shared_scope = &shared_data->shared_scope->appendScope(element.key);
	//			return true;
	//		}
	//		Datum& datum = shared_data->shared_scope->append(element.key);
	//		if (element.type == "integer")
	//		{
	//			datum.setType(Datum::DatumType::INTEGER);
	//		}
	//		else if (element.type == "float")
	//		{
	//			datum.setType(Datum::DatumType::FLOAT);
	//		}
	//		else if (element.type == "vector4")
	//		{
	//			datum.setType(Datum::DatumType::VECTOR4);
	//		}
	//		else if (element.type == "matrix4x4")
	//		{
	//			datum.setType(Datum::DatumType::MATRIX4X4);
	//		}
	//		else if (element.type == "string")
	//		{
	//			datum.setType(Datum::DatumType::STRING);
	//		}

	//		if (!datum.isExternal())
	//		{
	//			datum.setSize(datum.size() + 1);
	//		}
	//	}

	//	if (t_key == "Value")
	//	{
	//		element.value = t_values;
	//		std::string stack_top = key_stack.top();
	//		if (!element.class_name.empty() && stack_top == "name")
	//		{
	//			if(!element.attribute_name.empty())
	//			{
	//				shared_data->shared_scope = shared_data->shared_scope->As<Sector>()->createEntity(element.class_name, element.attribute_name);
	//				element.attribute_name = "";
	//				element.class_name = "";
	//			}
	//		}

	//		Datum& datum = shared_data->shared_scope->append(element.key);
	//		if (m_is_array_size_already_assigned)
	//		{
	//			--m_array_elements_to_parse;
	//		}
	//		if (t_is_array_element)
	//		{
	//			if (!datum.isExternal() && (0 == t_index))
	//			{
	//				datum.setSize(element.value.size());
	//				m_array_elements_to_parse = t_array_size;
	//			}
	//			datum.setFromString(t_values.asString(), t_index);
	//			--m_array_elements_to_parse;
	//		}
	//		else
	//		{
	//			datum.setFromString(t_values.asString(), (datum.size() - 1));
	//		}
	//	}

	//	return true;
	//}

	bool JsonTableParseHelper::endHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key)
	{
		JsonTableParseHelper::SharedData* shared_data = t_shared_data.As<JsonTableParseHelper::SharedData>();
		if (shared_data == nullptr)
		{
			return false;
		}
		if (t_key == "Value")
		{
			return true;
		}
		if (!key_stack.isEmpty() && (key_stack.top() == t_key))
		{
			key_stack.pop();
			if (m_array_elements_to_parse > 0)
			{
				--m_array_elements_to_parse;
				return true;
			}
			else
			{
				if (m_array_elements_to_parse == 0)
				{
					m_is_array_size_already_assigned = false;
				}
				if (nullptr != shared_data->shared_scope->getParent())
				{
					shared_data->shared_scope = shared_data->shared_scope->getParent();
				}
			}
		}
		return true;
	}

	gsl::owner<IJsonParseHelper*> JsonTableParseHelper::create() const
	{
		return new JsonTableParseHelper();
	}

	size_t JsonTableParseHelper::stackSize() const
	{
		return key_stack.size();
	}

#pragma endregion
}