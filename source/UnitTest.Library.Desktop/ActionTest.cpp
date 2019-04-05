#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include "Factory.h"
#include "JsonTableParseHelper.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionListIf.h"
#include "ActionIncrement.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "CppUnitTest.h"
#include <fstream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<ActionList>(const ActionList& t_temp)
	{
		RETURN_WIDE_STRING(t_temp.ToString().c_str());
	}

	template<>
	std::wstring ToString<ActionListIf>(const ActionListIf& t_temp)
	{
		RETURN_WIDE_STRING(t_temp.ToString().c_str());
	}

	template<>
	std::wstring ToString<ActionIncrement>(const ActionIncrement& t_temp)
	{
		RETURN_WIDE_STRING(t_temp.ToString().c_str());
	}

	template<>
	std::wstring ToString<ActionCreateAction>(const ActionCreateAction& t_temp)
	{
		RETURN_WIDE_STRING(t_temp.ToString().c_str());
	}

	template<>
	std::wstring ToString<ActionDestroyAction>(const ActionDestroyAction& t_temp)
	{
		RETURN_WIDE_STRING(t_temp.ToString().c_str());
	}

	template<>
	std::wstring ToString<Entity>(Entity* t_temp)
	{
		RETURN_WIDE_STRING(t_temp->ToString().c_str());
	}
}


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionTest)
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

		TEST_METHOD(ConstructorTest)
		{
			ActionIncrement action_increment;
			Assert::IsTrue(action_increment.isPrescribedAttribute("Name"));

			ActionList action_list;
			Assert::IsTrue(action_list.isPrescribedAttribute("Name"));

			ActionListIf action_list_if;
			Assert::IsTrue(action_list_if.isPrescribedAttribute("Name"));
			Assert::IsTrue(action_list_if.isPrescribedAttribute("Condition"));
			Assert::IsTrue(action_list_if.isPrescribedAttribute("Then"));
			Assert::IsTrue(action_list_if.isPrescribedAttribute("Else"));

			ActionCreateAction action_create_action;
			Assert::IsTrue(action_create_action.isPrescribedAttribute("Name"));
			Assert::IsTrue(action_create_action.isPrescribedAttribute("PrototypeName"));
			Assert::IsTrue(action_create_action.isPrescribedAttribute("InstanceName"));

			ActionDestroyAction action_destroy_action;
			Assert::IsTrue(action_destroy_action.isPrescribedAttribute("Name"));
			Assert::IsTrue(action_destroy_action.isPrescribedAttribute("ActionInstanceName"));
		}

		TEST_METHOD(UpdateTest)
		{
			GameTime game_time;
			WorldState world_state;
			world_state.setGameTime(game_time);

			// ActionIncrement Update Test
			ActionIncrement action_increment;
			action_increment["TestInteger"] = 5;
			action_increment["Target"] = "TestInteger";
			Assert::AreEqual(action_increment["TestInteger"].get<int32_t>(), 5);
			action_increment.update(world_state);
			Assert::AreEqual(action_increment["TestInteger"].get<int32_t>(), 6);

			// ActionList Update Test
			ActionList action_list;
			ActionIncrement* action_increment_first = new ActionIncrement();
			ActionIncrement* action_increment_second = new ActionIncrement();

			(*action_increment_first)["TestInteger"] = 100;
			(*action_increment_second)["TestInteger"] = 200;
			(*action_increment_first)["Target"] = "TestInteger";
			(*action_increment_second)["Target"] = "TestInteger";
			action_list["TestInteger"] = 100;
			action_list.adopt("Actions", *action_increment_first);
			action_list.adopt("Actions", *action_increment_second);

			Assert::IsTrue((*action_increment_first)["TestInteger"].get<int32_t>() == 100);
			Assert::IsTrue(action_list["TestInteger"].get<int32_t>() == 100);
			action_list.update(world_state);
			Assert::IsTrue((*action_increment_first)["TestInteger"].get<int32_t>() == 101);
			Assert::IsTrue((*action_increment_second)["TestInteger"].get<int32_t>() == 201);
			Assert::IsTrue(action_list["TestInteger"].get<int32_t>() == 100);
			Assert::AreEqual(action_list.actions().size(), 2U);

			// ActionListIf Update Test
			{
				EntityFactory entity_factory;
				ActionListFactory action_list_factory;
				ActionListIfFactory action_list_if_factory;
				World world("TestWorld");
				Entity* TestEntity = world.createSector("TestSector")->createEntity("Entity", "TestEntity");
				ActionList* TestAction = TestEntity->createAction("ActionList", "TestAction")->As<ActionList>();
				Assert::AreEqual("TestAction"s, TestAction->name());
				ActionListIf* TestActionListIf = TestAction->createAction("ActionListIf", "TestActionListIf")->As<ActionListIf>();
				Assert::AreEqual("TestActionListIf"s, TestActionListIf->name());
				TestActionListIf->setCondition(1);
				ActionIncrement* inc = new ActionIncrement();
				(*inc)["TestInteger"] = 20;
				(*inc)["Target"] = "TestInteger";
				TestActionListIf->setIfBlock(*inc);
				Assert::AreEqual(20, (*inc)["TestInteger"].get<int32_t>());
				TestEntity->update(world_state);
				Assert::AreEqual(21, (*inc)["TestInteger"].get<int32_t>());
			}
			{
				EntityFactory entity_factory;
				ActionListFactory action_list_factory;
				ActionListIfFactory action_list_if_factory;
				World world("TestWorld");
				Entity* TestEntity = world.createSector("TestSector")->createEntity("Entity", "TestEntity");
				ActionList* TestAction = TestEntity->createAction("ActionList", "TestAction")->As<ActionList>();
				Assert::AreEqual("TestAction"s, TestAction->name());
				ActionListIf* TestActionListIf = TestAction->createAction("ActionListIf", "TestActionListIf")->As<ActionListIf>();
				Assert::AreEqual("TestActionListIf"s, TestActionListIf->name());
				TestActionListIf->setCondition(0);
				ActionIncrement* inc = new ActionIncrement();
				(*inc)["TestInteger"] = 20;
				(*inc)["Target"] = "TestInteger";
				TestActionListIf->setElseBlock(*inc);
				Assert::AreEqual(20, (*inc)["TestInteger"].get<int32_t>());
				TestEntity->update(world_state);
				Assert::AreEqual(21, (*inc)["TestInteger"].get<int32_t>());
			}

			// ActionCreateAction Update Test
			{
				EntityFactory entity_factory;
				ActionListFactory action_list_factory;
				ActionCreateActionFactory action_create_action_factory;
				ActionIncrementFactory action_increment_factory;
				ActionListIfFactory action_list_if_factory;

				World world("TestWorld");
				Entity* TestEntity = world.createSector("TestSector")->createEntity("Entity", "TestEntity");
				ActionList* TestActionList = TestEntity->createAction("ActionList", "TestActionList")->As<ActionList>();
				Assert::AreEqual("TestActionList"s, TestActionList->name());
				ActionCreateAction* TestActionCreateAction = TestActionList->createAction("ActionCreateAction", "TestActionCreateAction")->As<ActionCreateAction>();
				Assert::AreEqual("TestActionCreateAction"s, TestActionCreateAction->name());
				TestActionCreateAction->setClassName("ActionIncrement");
				TestActionCreateAction->setInstanceName("ActionIncrementInstance");
				Assert::AreEqual(TestActionList->actions().size(), 1U);
				TestEntity->update(world_state);
				Assert::AreEqual(TestActionList->actions().size(), 2U);
			}

			// ActionDestroyAction Update Test
			// Not fully tested due to absence of Graveyard implementation
			{
				EntityFactory entity_factory;
				ActionListFactory action_list_factory;
				ActionDestroyActionFactory action_destroy_action_factory;
				ActionIncrementFactory action_increment_factory;
				ActionListIfFactory action_list_if_factory;

				World world("TestWorld");
				Entity* TestEntity = world.createSector("TestSector")->createEntity("Entity", "TestEntity");
				ActionList* TestActionList = TestEntity->createAction("ActionList", "TestActionList")->As<ActionList>();
				Assert::AreEqual("TestActionList"s, TestActionList->name());
				ActionDestroyAction* TestActionDestroyAction = TestActionList->createAction("ActionDestroyAction", "TestActionDestroyAction")->As<ActionDestroyAction>();
				TestActionDestroyAction->setActionInstanceName("TestActionDestroyAction");
				Assert::AreEqual("TestActionDestroyAction"s, TestActionDestroyAction->name());
				Assert::AreEqual(TestActionList->actions().size(), 1U);
				TestEntity->update(world_state);
				Assert::AreEqual(TestActionList->actions().size(), 1U);
			}
		}

		TEST_METHOD(CloneTest)
		{
			{
				ActionList action_list("action_list");
				auto clone = action_list.clone();
				ActionList* action_list_clone = clone->As<ActionList>();
				Assert::IsNotNull(action_list_clone);
				Assert::AreNotSame(action_list, *action_list_clone);
				delete clone;
			}
			
			{
				ActionListIf action_list_if("action_list_if");
				auto clone = action_list_if.clone();
				ActionListIf* action_list_if_clone = clone->As<ActionListIf>();
				Assert::IsNotNull(action_list_if_clone);
				Assert::AreNotSame(action_list_if, *action_list_if_clone);
				delete clone;
			}

			{
				ActionIncrement action_increment;
				auto clone = action_increment.clone();
				ActionIncrement* action_increment_clone = clone->As<ActionIncrement>();
				Assert::IsNotNull(action_increment_clone);
				Assert::AreNotSame(action_increment, *action_increment_clone);
				delete clone;
			}

			{
				ActionCreateAction action_create_action("action_list");
				auto clone = action_create_action.clone();
				ActionCreateAction* action_create_action_clone = clone->As<ActionCreateAction>();
				Assert::IsNotNull(action_create_action_clone);
				Assert::AreNotSame(action_create_action, *action_create_action_clone);
				delete clone;
			}

			{
				ActionDestroyAction action_destory_action("action_list");
				auto clone = action_destory_action.clone();
				ActionDestroyAction* action_destroy_action_clone = clone->As<ActionDestroyAction>();
				Assert::IsNotNull(action_destroy_action_clone);
				Assert::AreNotSame(action_destory_action, *action_destroy_action_clone);
				delete clone;
			}
		}

		TEST_METHOD(GetSetEntity)
		{
			EntityFactory entity_factory;
			World world("TestWorld");
			Entity* TestEntity = world.createSector("TestSector")->createEntity("Entity", "TestEntity");
			ActionIncrement* action_increment = new ActionIncrement();
			action_increment->setEntity(TestEntity);
			Assert::AreEqual(TestEntity, action_increment->getEntity()->As<Entity>());
		}

		TEST_METHOD(ParsingFromFileTest)
		{
			SectorFactory sectorFactory;
			EntityFactory factory;

			const std::string file_name = "Content\\WorldTest.json";
			std::ifstream json_file(file_name);
			Assert::IsFalse(json_file.bad());
			json_file.close();

			World world;
			JsonTableParseHelper::SharedData shared_data(world);
			JsonTableParseHelper parse_helper;
			JsonParseMaster parse_master(shared_data);
			parse_master.addHelper(parse_helper);
			parse_master.initialize();

			parse_master.parseFromFile(file_name);

			// TODO

		}



	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState ActionTest::s_start_mem_state;
}