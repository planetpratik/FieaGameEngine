#include "pch.h"
#include "JsonTableParseHelper.h"
#include "Sector.h"
#include "Action.h"


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
		t_array_size;

		JsonTableParseHelper::SharedData* shared_data = t_shared_data.As<JsonTableParseHelper::SharedData>();
		if (shared_data == nullptr)
		{
			return false;
		}

		if (t_key == "Type")
		{
			assert(call_stack.isEmpty() == false);
			StackFrame& stackFrame = call_stack.top();
			stackFrame.type = Datum::StringDatumTypeMap.at(t_values.asString());
			Datum& datum = stackFrame.scope->append(stackFrame.key);
			datum.setType(stackFrame.type);
		}

		else if (t_key == "Class")
		{
			assert(call_stack.isEmpty() == false);
			StackFrame& stackFrame = call_stack.top();
			stackFrame.class_name = t_values.asString();
		}

		else if (t_key == "Value")
		{
			assert(call_stack.isEmpty() == false);
			StackFrame& stackFrame = call_stack.top();
			if (stackFrame.type == Datum::DatumType::TABLE)
			{
				if (t_values.size() > 0)
				{
					Datum* nestedScopeDatum = stackFrame.scope->find(stackFrame.key);
					Scope* nestedScope;
					if (nullptr != nestedScopeDatum && nestedScopeDatum->size() > t_index)
					{
						nestedScope = &(*nestedScopeDatum)[t_index];
					}
					else if (stackFrame.class_name.empty() == false)
					{
						nestedScope = Factory<Scope>::create(stackFrame.class_name);
						assert(nestedScope != nullptr);
						stackFrame.scope->adopt(stackFrame.key, *nestedScope);
					}
					else
					{
						nestedScope = &(stackFrame.scope->appendScope(stackFrame.key));
					}
					if (t_is_array_element)
					{
						call_stack.push({ t_key, Datum::DatumType::TABLE, "", nestedScope });
					}
					else
					{
						stackFrame.scope = nestedScope;
					}
				}
			}
			else
			{
				Datum& datum = stackFrame.scope->at(stackFrame.key);
				switch (datum.type())
				{
				case Datum::DatumType::INTEGER:
					datum.set(t_values.asInt(), t_index);
					break;
				case Datum::DatumType::FLOAT:
					datum.set(t_values.asFloat(), t_index);
					break;

				case Datum::DatumType::STRING:
					datum.set(t_values.asString(), t_index);
					break;

				case Datum::DatumType::VECTOR4:
				case Datum::DatumType::MATRIX4X4:
					datum.setFromString(t_values.asString(), t_index);
					break;
				}
			}
		}
		else
		{
			StackFrame stack_frame;
			stack_frame.key = t_key;
			stack_frame.class_name = "";
			stack_frame.scope = (call_stack.isEmpty() ? shared_data->shared_scope : call_stack.top().scope);
			assert(stack_frame.scope != nullptr);
			call_stack.push(stack_frame);
		}
		return true;
	}


	bool JsonTableParseHelper::endHandler(JsonParseMaster::SharedData& t_shared_data, const std::string& t_key)
	{
		JsonTableParseHelper::SharedData* shared_data = t_shared_data.As<JsonTableParseHelper::SharedData>();
		if (shared_data == nullptr)
		{
			return false;
		}

		auto& t_stack_frame = call_stack.top();
		if (t_key == t_stack_frame.key)
		{
			call_stack.pop();
		}

		return true;
	}

	gsl::owner<IJsonParseHelper*> JsonTableParseHelper::create() const
	{
		return new JsonTableParseHelper();
	}

	size_t JsonTableParseHelper::stackSize() const
	{
		return call_stack.size();
	}

#pragma endregion
}