#include "pch.h"
#include <string>
#include "JsonTableParseHelper.h"
#include "CppUnitTest.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<FieaGameEngine::Scope>(FieaGameEngine::Scope* t_temp)
	{
		RETURN_WIDE_STRING(t_temp);
	}

	template<>
	std::wstring ToString<FieaGameEngine::JsonParseMaster>(FieaGameEngine::JsonParseMaster* t_temp)
	{
		RETURN_WIDE_STRING(t_temp);
	}

	template<>
	std::wstring ToString<FieaGameEngine::JsonTableParseHelper::SharedData>(FieaGameEngine::JsonTableParseHelper::SharedData* t_temp)
	{
		RETURN_WIDE_STRING(t_temp);
	}
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
			const string input = R"({ "speed": { "Type": "table", "Value": [ { "Ferrari": { "Type": "integer", "Value": 100 } }, { "Porsche": { "Type": "integer", "Value": 150 } }, { "Lamborghini": { "Type": "integer", "Value": 200 } } ] }, "food": { "Type": "string", "Value": [ "kitkat", "StarBurst", "Reeses" ] } })";
			JsonTableParseHelper::SharedData shared_data(scope);
			JsonTableParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			parse_master.initialize();

			parse_master.parse(input);
			Scope* new_scope = scope["speed"].get<Scope*>(0);
			Scope temp = *new_scope;
			Assert::AreEqual(100, temp["Ferrari"].get<int32_t>(0));
			Assert::AreEqual(150, temp["Porsche"].get<int32_t>(0));
			Assert::AreEqual(200, temp["Lamborghini"].get<int32_t>(0));

			Assert::AreEqual("kitkat"s, scope["food"].get<std::string>(0));
			Assert::AreEqual("StarBurst"s, scope["food"].get<std::string>(1));
			Assert::AreEqual("Reeses"s, scope["food"].get<std::string>(2));
		}
		
		TEST_METHOD(ParseScopeFromFileTest)
		{
			const std::string file_name = "Content\\ScopeTest.json";
			std::ifstream json_file(file_name);
			Assert::IsFalse(json_file.bad());
			json_file.close();

			Scope scope;
			JsonTableParseHelper::SharedData shared_data(scope);
			JsonTableParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			parse_master.initialize();

			parse_master.parseFromFile(file_name);

			Assert::IsTrue(glm::vec4(1, 0, 0, 1) == scope["directions"].get<glm::vec4>(0));

			Assert::AreEqual("kitkat"s, scope["food"].get<std::string>(0));
			Assert::AreEqual("StarBurst"s, scope["food"].get<std::string>(1));
			Assert::AreEqual("Reeses"s, scope["food"].get<std::string>(2));

			Assert::AreEqual(100, scope["health"].get<int32_t>(0));

			Scope* new_scope = scope["inventory"].get<Scope*>(0);
			Scope temp = *new_scope;
			Scope* another_scope = temp["food"].get<Scope*>(0);
			Scope temp_two = *another_scope;
			Assert::AreEqual("Almond"s, temp_two["protein"].get<std::string>(0));

			new_scope = scope["maxspeed"].get<Scope*>(0);
			temp = *new_scope;
			Assert::AreEqual(100, temp["Ferrari"].get<int32_t>(0));
			Assert::AreEqual(150, temp["Porsche"].get<int32_t>(0));
			Assert::AreEqual(200, temp["Lamborghini"].get<int32_t>(0));
			
			Assert::AreEqual(74.2f, scope["power"].get<float_t>(0));
			Assert::AreEqual(51.5f, scope["power"].get<float_t>(1));
			Assert::AreEqual(92.0f, scope["power"].get<float_t>(2));

			std::string temp_string = "mat4x4((1, 0, 0, 0), (1, 0, 0, 0), (1, 1, 0, 0), (0, 0, 1, 1))"s;
			glm::mat4x4 temp_mat;
			sscanf_s(temp_string.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
				&temp_mat[0][0], &temp_mat[0][1], &temp_mat[0][2], &temp_mat[0][3],
				&temp_mat[1][0], &temp_mat[1][1], &temp_mat[1][2], &temp_mat[1][3],
				&temp_mat[2][0], &temp_mat[2][1], &temp_mat[2][2], &temp_mat[2][3],
				&temp_mat[3][0], &temp_mat[3][1], &temp_mat[3][2], &temp_mat[3][3]);

			Assert::IsTrue(temp_mat == scope["transform"].get<glm::mat4x4>(0));

			Assert::AreEqual(24.3f, scope["speed"].get<float_t>(0));
		}

		TEST_METHOD(InitializeTest)
		{
			Scope scope;
			const string input = R"({ "health": { "Type": "integer", "Value": 100 }, "speed": { "Type": "float", "Value": 24.3 } })";
			JsonTableParseHelper::SharedData shared_data(scope);
			JsonTableParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			parse_master.initialize();

			parse_master.parse(input);
			parse_helper.initialize();
			Assert::AreEqual(0, static_cast<int32_t>(parse_helper.stackSize()));
		}

		TEST_METHOD(SharedDataGetterSetterTest)
		{
			Scope scope;
			Scope scope_two;
			const string input = R"({ "health": { "Type": "integer", "Value": 100 }, "speed": { "Type": "float", "Value": 24.3 } })";
			JsonTableParseHelper::SharedData shared_data(scope);
			JsonTableParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			parse_master.initialize();
			parse_master.parse(input);
			Assert::AreEqual(&shared_data, parse_master.getSharedData()->As<JsonTableParseHelper::SharedData>());
			Assert::AreEqual(&scope, shared_data.getSharedData());
			shared_data.setSharedData(scope_two);
			Assert::AreEqual(&scope_two, shared_data.getSharedData());
		}

		TEST_METHOD(CloneTest)
		{
			Scope scope;
			JsonTableParseHelper::SharedData shared_data(scope);
			JsonTableParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			auto clone = parse_master.clone();
			Assert::AreNotEqual(clone, &parse_master);
			delete clone->getSharedData()->As<JsonTableParseHelper::SharedData>()->getSharedData();
			delete clone;
		}


	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState JsonTableParseHelperTest::s_start_mem_state;
}