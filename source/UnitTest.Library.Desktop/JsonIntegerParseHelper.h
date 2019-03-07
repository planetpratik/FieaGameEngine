#pragma once
#include "RTTI.h"
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"

namespace UnitTestLibraryDesktop
{
	class JsonIntegerParseHelper final : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonIntegerParseHelper, FieaGameEngine::IJsonParseHelper)
	public:
		class SharedData final : public FieaGameEngine::JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedData, FieaGameEngine::JsonParseMaster::SharedData)
		public:
			virtual void initialize() override;
			virtual gsl::owner<SharedData*> create() const override;

			uint32_t max_depth = 0;
			int32_t int_value = 0;
			FieaGameEngine::Vector<int32_t> data;
		};

		virtual gsl::owner<IJsonParseHelper*> JsonIntegerParseHelper::create() const override;
		virtual void JsonIntegerParseHelper::initialize() override;
		virtual bool JsonIntegerParseHelper::startHandler(FieaGameEngine::JsonParseMaster::SharedData& t_shared_data, const std::string& t_key, const Json::Value& t_values, bool t_is_array_element, size_t t_index) override;
		virtual bool JsonIntegerParseHelper::endHandler(FieaGameEngine::JsonParseMaster::SharedData& t_shared_data, const std::string& t_key) override;

		bool is_initialized_called = false;
		uint32_t start_handler_count = 0;
		uint32_t end_handler_count = 0;
		uint32_t array_element_count = 0;

	private:
		inline static const std::string integerElementName = "integer";
		bool m_parsing_data = false;
	};

}
