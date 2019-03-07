#pragma once
#include "RTTI.h"
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"

namespace UnitTestLibraryDesktop
{
	class JsonTestParseHelper final : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTestParseHelper, FieaGameEngine::IJsonParseHelper)

	public:
	
		class SharedData final : public FieaGameEngine::JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedData, FieaGameEngine::JsonParseMaster::SharedData)
		public:
			virtual void initialize() override;
			virtual gsl::owner<SharedData*> create() const override;

			uint32_t max_depth = 0;
		};

		virtual gsl::owner<IJsonParseHelper*> JsonTestParseHelper::create() const override;
		virtual void JsonTestParseHelper::initialize() override;
		virtual bool JsonTestParseHelper::startHandler(FieaGameEngine::JsonParseMaster::SharedData& t_shared_data, const std::string& t_key, const Json::Value& t_values, bool t_is_array_element, size_t t_index) override;
		virtual bool JsonTestParseHelper::endHandler(FieaGameEngine::JsonParseMaster::SharedData& t_shared_data, const std::string& t_key) override;

		bool is_initialized_called = false;
		uint32_t start_handler_count = 0;
		uint32_t end_handler_count = 0;
		uint32_t array_element_count = 0;
	};
}