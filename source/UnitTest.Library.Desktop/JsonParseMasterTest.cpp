#include "pch.h"
#include <string>
#include <json/json.h>
#include "JsonTestParseHelper.h"
#include "JsonIntegerParseHelper.h"
#include "CppUnitTest.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<UnitTestLibraryDesktop::JsonTestParseHelper::SharedData>(UnitTestLibraryDesktop::JsonTestParseHelper::SharedData* t_temp)
	{
		RETURN_WIDE_STRING(t_temp);
	}

	template<>
	std::wstring ToString<UnitTestLibraryDesktop::JsonTestParseHelper>(UnitTestLibraryDesktop::JsonTestParseHelper* t_temp)
	{
		RETURN_WIDE_STRING(t_temp);
	}

	template<>
	std::wstring ToString<FieaGameEngine::IJsonParseHelper>(FieaGameEngine::IJsonParseHelper* t_temp)
	{
		RETURN_WIDE_STRING(t_temp);
	}

	template<>
	std::wstring ToString<FieaGameEngine::JsonParseMaster>(FieaGameEngine::JsonParseMaster* t_temp)
	{
		RETURN_WIDE_STRING(t_temp);
	}

	template<>
	std::wstring ToString<FieaGameEngine::JsonParseMaster::SharedData>(FieaGameEngine::JsonParseMaster::SharedData* t_temp)
	{
		RETURN_WIDE_STRING(t_temp);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonParseMasterTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&s_start_mem_state);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState end_mem_state, diff_mem_state;
			_CrtMemCheckpoint(&end_mem_state);
			if (_CrtMemDifference(&diff_mem_state, &s_start_mem_state, &end_mem_state))
			{
				_CrtMemDumpStatistics(&diff_mem_state);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(JsonParseMasterTestConstructor)
		{
			JsonTestParseHelper::SharedData shared_data;
			JsonTestParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			Assert::AreEqual(&shared_data, parse_master.getSharedData()->As<JsonTestParseHelper::SharedData>());
		}

		TEST_METHOD(JsonParseMasterTestMoveSemantics)
		{
			JsonTestParseHelper::SharedData shared_data;
			JsonTestParseHelper parse_helper;
			{
				JsonParseMaster parse_master(shared_data);
				parse_master.addHelper(parse_helper);
				// Move Constructor Test
				JsonParseMaster another_parse_master(std::move(parse_master));
				Assert::IsFalse(another_parse_master.isClone());
				Assert::AreEqual(1U, another_parse_master.getHelpers().size());
				Assert::AreEqual(&shared_data, another_parse_master.getSharedData()->As<JsonTestParseHelper::SharedData>());
				Assert::AreEqual(&another_parse_master, shared_data.getJsonParseMaster());
			}
			{
				JsonParseMaster parse_master(shared_data);
				parse_master.addHelper(parse_helper);
				parse_master.initialize();
				// Move Assignment operator Test
				JsonTestParseHelper::SharedData another_shared_data;
				JsonParseMaster another_parse_master(another_shared_data);
				another_parse_master = std::move(parse_master);
				Assert::IsFalse(another_parse_master.isClone());
				Assert::AreEqual(1U, another_parse_master.getHelpers().size());
				Assert::AreEqual(&shared_data, another_parse_master.getSharedData()->As<JsonTestParseHelper::SharedData>());
				Assert::AreEqual(&another_parse_master, shared_data.getJsonParseMaster());

			}
		}

		TEST_METHOD(JsonParseMasterTestClone)
		{
			JsonTestParseHelper::SharedData shared_data;
			JsonTestParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			Assert::IsFalse(parse_master.isClone());
			auto clone = parse_master.clone();
			Assert::AreNotEqual(&parse_master, clone);
			Assert::IsTrue(clone->isClone());
			auto helper_clone = clone->getHelpers()[0]->As<JsonTestParseHelper>();
			Assert::IsNotNull(helper_clone);
			Assert::AreNotEqual(helper_clone, parse_master.getHelpers()[0]->As<JsonTestParseHelper>());
			Assert::AreNotSame(clone->getSharedData(), parse_master.getSharedData());
			delete clone;
		}
		TEST_METHOD(JsonParseMasterTestIJsonParseHelperRTTI)
		{
			JsonTestParseHelper helper;
			RTTI* rtti = &helper;
			Assert::IsTrue(rtti->Is("IJsonParseHelper"));
			Assert::IsTrue(rtti->Is(JsonTestParseHelper::TypeIdClass()));
			Assert::IsTrue(rtti->TypeIdInstance());

			IJsonParseHelper* IJson_helper_rtti = rtti->As<IJsonParseHelper>();
			Assert::IsNotNull(IJson_helper_rtti);
			Assert::AreEqual(static_cast<IJsonParseHelper*>(&helper), IJson_helper_rtti);

			JsonTestParseHelper* helper_rtti = rtti->As<JsonTestParseHelper>();
			Assert::IsNotNull(helper_rtti);
			Assert::AreEqual(&helper, helper_rtti);

			RTTI* r = rtti->QueryInterface(Attributed::TypeIdClass());
			Assert::IsTrue(r == nullptr);
			r = rtti->QueryInterface(JsonTestParseHelper::TypeIdClass());
			Assert::IsTrue(r != nullptr);
		}

		TEST_METHOD(JsonParseMasterTestSharedDataRTTI)
		{
			JsonTestParseHelper::SharedData shared_data;
			RTTI* rtti = &shared_data;
			Assert::IsTrue(rtti->Is("SharedData"));
			Assert::IsTrue(rtti->Is(JsonTestParseHelper::SharedData::TypeIdClass()));
			Assert::IsTrue(rtti->TypeIdInstance());

			JsonTestParseHelper::SharedData* shared_data_rtti = rtti->As<JsonTestParseHelper::SharedData>();
			Assert::IsNotNull(shared_data_rtti);
			Assert::AreEqual(&shared_data, shared_data_rtti);

			RTTI* r = rtti->QueryInterface(Attributed::TypeIdClass());
			Assert::IsTrue(r == nullptr);
			r = rtti->QueryInterface(JsonTestParseHelper::SharedData::TypeIdClass());
			Assert::IsTrue(r != nullptr);
		}

		TEST_METHOD(JsonParseMasterTestParseString)
		{
			const string input = R"({ "integer": { "name": "Health", "value": 100 } })";
			JsonTestParseHelper::SharedData shared_data;
			JsonTestParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);

			Assert::IsFalse(parse_helper.is_initialized_called);
			parse_master.initialize();
			Assert::IsTrue(parse_helper.is_initialized_called);
			parse_master.parse(input);
			Assert::AreEqual(3U, parse_helper.start_handler_count);
			Assert::AreEqual(3U, parse_helper.end_handler_count);
			Assert::AreEqual(0U, shared_data.depth());
			Assert::AreEqual(2U, shared_data.max_depth);
		}

		TEST_METHOD(JsonParseMasterTestParseStream)
		{
			stringstream input;
			input << R"({ "integer": { "name": "Health", "value": 100 } })"s;
			JsonTestParseHelper::SharedData shared_data;
			JsonTestParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);

			Assert::IsFalse(parse_helper.is_initialized_called);
			parse_master.initialize();
			Assert::IsTrue(parse_helper.is_initialized_called);
			parse_master.parse(input);
			Assert::AreEqual(3U, parse_helper.start_handler_count);
			Assert::AreEqual(3U, parse_helper.end_handler_count);
			Assert::AreEqual(0U, shared_data.depth());
			Assert::AreEqual(2U, shared_data.max_depth);
		}

		TEST_METHOD(JsonParseMasterTestParseFile)
		{
			const std::string file_name = "Content\\TestData.json";
			const std::string input_string = R"({ "integer": { "name": "Health", "value": 100 } })";
			std::ifstream json_file(file_name);
			Assert::IsFalse(json_file.bad());
			json_file.close();
			JsonTestParseHelper::SharedData shared_data;
			JsonTestParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			parse_master.initialize();
			parse_master.parseFromFile(file_name);
			Assert::AreEqual("Content\\TestData.json"s, parse_master.getFileName());
			Assert::AreEqual(3U, parse_helper.start_handler_count);
			Assert::AreEqual(3U, parse_helper.end_handler_count);
			Assert::AreEqual(0U, shared_data.depth());
			Assert::AreEqual(2U, shared_data.max_depth);
		}

		TEST_METHOD(JsonParseMasterTestSimpleIntegerParsing)
		{
			std::string input = R"({ "integer": 100 })";
			JsonIntegerParseHelper::SharedData shared_data;
			JsonIntegerParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			parse_master.initialize();
			parse_master.parse(input);
			Assert::AreEqual(100, shared_data.data[0]);
		}

		TEST_METHOD(JsonParseMasterTestIntegerArrayParsing)
		{
			std::string input = R"({ "integer": [ 10, 20, 30, 40 ] })";
			JsonIntegerParseHelper::SharedData shared_data;
			JsonIntegerParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			parse_master.initialize();
			parse_master.parse(input);
			Assert::AreEqual(4U, shared_data.data.size());
			Assert::AreEqual(10, shared_data.data[0]);
			Assert::AreEqual(20, shared_data.data[1]);
			Assert::AreEqual(30, shared_data.data[2]);
			Assert::AreEqual(40, shared_data.data[3]);
		}

		TEST_METHOD(JsonParseMasterTestAddHelper)
		{
			JsonTestParseHelper::SharedData shared_data;
			JsonTestParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			Assert::AreEqual(1U, parse_master.getHelpers().size());
			JsonIntegerParseHelper integer_parse_helper;
			parse_master.addHelper(integer_parse_helper);
			Assert::AreEqual(2U, parse_master.getHelpers().size());

			auto clone = parse_master.clone();
			Assert::AreEqual(2U, clone->getHelpers().size());
			auto expression = [&clone, &parse_helper] { clone->addHelper(parse_helper); };
			Assert::ExpectException<std::exception>(expression);

			delete clone;
		}

		TEST_METHOD(JsonParseMasterTestRemoveHelper)
		{
			JsonTestParseHelper::SharedData shared_data;
			JsonTestParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			JsonIntegerParseHelper integer_parse_helper;
			Assert::IsFalse(parse_master.removeHelper(integer_parse_helper));
			Assert::AreEqual(1U, parse_master.getHelpers().size());
			parse_master.removeHelper(parse_helper);
			Assert::AreEqual(0U, parse_master.getHelpers().size());
		}

	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState JsonParseMasterTest::s_start_mem_state;
}