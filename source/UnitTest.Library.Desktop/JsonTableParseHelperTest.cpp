#include "pch.h"
#include <string>
#include <json/json.h>
#include "JsonTableParseHelper.h"
#include "CppUnitTest.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	/*template<>
	std::wstring ToString<UnitTestLibraryDesktop::JsonTestParseHelper::SharedData>(UnitTestLibraryDesktop::JsonTestParseHelper::SharedData* t_temp)
	{
		RETURN_WIDE_STRING(t_temp);
	}*/

	/*template<>
	std::wstring ToString<UnitTestLibraryDesktop::JsonTestParseHelper>(UnitTestLibraryDesktop::JsonTestParseHelper* t_temp)
	{
		RETURN_WIDE_STRING(t_temp);
	}*/

	/*template<>
	std::wstring ToString<FieaGameEngine::IJsonParseHelper>(FieaGameEngine::IJsonParseHelper* t_temp)
	{
		RETURN_WIDE_STRING(t_temp);
	}*/

	/*template<>
	std::wstring ToString<FieaGameEngine::JsonParseMaster>(FieaGameEngine::JsonParseMaster* t_temp)
	{
		RETURN_WIDE_STRING(t_temp);
	}*/

	/*template<>
	std::wstring ToString<FieaGameEngine::JsonParseMaster::SharedData>(FieaGameEngine::JsonParseMaster::SharedData* t_temp)
	{
		RETURN_WIDE_STRING(t_temp);
	}*/
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonTableParseHelperTest)
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

		TEST_METHOD(ParseFromStringTest)
		{
			Scope scope;
			const string input = R"({ "health": { "Type": "integer", "Value": 100 }, "speed": { "Type": "float", "Value": 24.3 } })";
			JsonTableParseHelper::SharedData shared_data(scope);
			JsonTableParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			parse_master.initialize();
			
			parse_master.parse(input);

			Assert::AreEqual(100, scope["health"].get<int32_t>(0));
			Assert::AreEqual(24.3f, scope["speed"].get<float_t>(0));
		}

		TEST_METHOD(ParseNestedScopeFromStringTest)
		{
			Scope scope;
			const string input = R"({ "inventory": { "Type": "table", "Value": { "food": { "Type": "table", "Value": { "protein": { "Type": "string", "Value": "Almond" } } } } } })";
			JsonTableParseHelper::SharedData shared_data(scope);
			JsonTableParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			parse_master.initialize();

			parse_master.parse(input);

			Scope* new_scope = scope["inventory"].get<Scope*>(0);
			Scope temp = *new_scope;
			Scope* another_scope = temp["food"].get<Scope*>(0);
			Scope temp_two = *another_scope;
			Assert::AreEqual("Almond"s, temp_two["protein"].get<std::string>(0));
		}

		TEST_METHOD(ParseArrayFromStringTest)
		{
			Scope scope;
			const string input = R"({ "speed": { "Type": "integer", "Value": [ 100, 200, 300 ] }, "inventory": { "Type": "table", "Value": { "food": { "Type": "string", "Value": [ "kitkat", "StarBurst", "Reeses" ] } } } })";
			JsonTableParseHelper::SharedData shared_data(scope);
			JsonTableParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			parse_master.initialize();

			parse_master.parse(input);
			Assert::AreEqual(100, scope["speed"].get<int32_t>(0));
			Assert::AreEqual(200, scope["speed"].get<int32_t>(1));
			Assert::AreEqual(300, scope["speed"].get<int32_t>(2));

			Scope* new_scope = scope["inventory"].get<Scope*>(0);
			Scope temp = *new_scope;
			Assert::AreEqual("kitkat"s, temp["food"].get<std::string>(0));
			Assert::AreEqual("StarBurst"s, temp["food"].get<std::string>(1));
			Assert::AreEqual("Reeses"s, temp["food"].get<std::string>(2));
		}

		TEST_METHOD(ParseArrayofObjectsFromStringTest)
		{
			Scope scope;
			const string input = R"({ "speed": { "Type": "table", "Value": [ { "Ferrari": { "Type": "integer", "Value": 100 } }, { "Porsche": { "Type": "integer", "Value": 150 } }, { "Lamborghini": { "Type": "integer", "Value": 200 } } ] } })";
			JsonTableParseHelper::SharedData shared_data(scope);
			JsonTableParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			parse_master.initialize();

			parse_master.parse(input);
			Scope* new_scope = scope["speed"].get<Scope*>(0);
			Scope temp = *new_scope;
			Assert::AreEqual(100, temp["Ferrari"].get<int32_t>(0));
			// Wrongly getting placed. 
			Assert::AreEqual(150, scope["Porsche"].get<int32_t>(0));
			Assert::AreEqual(200, scope["Lamborghini"].get<int32_t>(0));
		}


	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState JsonTableParseHelperTest::s_start_mem_state;
}