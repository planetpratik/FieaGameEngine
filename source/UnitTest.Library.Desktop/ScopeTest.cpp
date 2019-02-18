#include "pch.h"
#include <string>
#include <utility>
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ScopeTest)
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

		TEST_METHOD(ScopeConstructor)
		{
			Scope scope(20);
			Assert::IsNull(scope.getParent());
		}

		TEST_METHOD(ScopeCopyConstructor)
		{
			Scope zork;
			Datum& rooms = zork.append("rooms");
			rooms = 10;
			Scope& inventory = zork.appendScope("inventory");
			Datum& key = inventory.append("key");
			key = 5549;
			Scope& food = inventory.appendScope("food");
			Datum& almonds = food.append("almonds");
			almonds = 43;

			Scope another_zork(zork);
			Assert::IsTrue(another_zork == zork);
		}

		TEST_METHOD(CopyAssignmentOperator)
		{
			Scope zork;
			Datum& rooms = zork.append("rooms");
			rooms = 10;
			Scope& inventory = zork.appendScope("inventory");
			Datum& key = inventory.append("key");
			key = 5549;
			Scope& food = inventory.appendScope("food");
			Datum& almonds = food.append("almonds");
			almonds = 43;

			Scope another_zork;
			Datum& another_rooms = another_zork.append("rooms");
			another_rooms = 10;
			Scope& another_inventory = another_zork.appendScope("inventory");
			Datum& another_key = another_inventory.append("key");
			another_key = 5549;
			Scope& another_food = another_inventory.appendScope("food");
			Datum& another_almonds = another_food.append("almonds");
			another_almonds = 43;

			Assert::IsTrue(zork != another_zork);
			another_zork = zork;
			Assert::IsTrue(zork == another_zork);
		}
		TEST_METHOD(ScopeMoveConstructor)
		{
			Scope zork;
			Datum& rooms = zork.append("rooms");
			rooms = 10;
			Scope& inventory = zork.appendScope("inventory");
			Datum& key = inventory.append("key");
			key = 5549;
			Scope& food = inventory.appendScope("food");
			Datum& almonds = food.append("almonds");
			almonds = 43;

			Scope another_zork(std::move(zork));
			Assert::IsNull(another_zork.getParent());
		}

		TEST_METHOD(ScopeFind)
		{
			Scope zork;
			Assert::IsNull(zork.find(""));
			Datum& rooms = zork.append("rooms");
			Assert::IsTrue(zork.find("rooms") == &rooms);
			Assert::IsNull(zork.find("food"));
		}

		TEST_METHOD(ScopeFindConst)
		{
			const Scope zork;
			Assert::IsNull(zork.find(""));
			Scope another_zork;
			const Datum& rooms = another_zork.append("rooms");
			const Scope* just_another_zork = const_cast<Scope*>(&another_zork);
			Assert::IsTrue(just_another_zork->find("rooms") == &rooms);
			Assert::IsNull(just_another_zork->find("food"));
		}

		TEST_METHOD(ScopeSearch)
		{
			Scope zork;
			Datum& rooms = zork.append("rooms");
			Scope& inventory = zork.appendScope("inventory");
			Scope& food = inventory.appendScope("food");

			Scope* out_param;
			Assert::IsTrue(&rooms == zork.search("rooms", &out_param));
			Assert::IsTrue(&zork == out_param);
			Datum* datum = zork.search("monkey", &out_param);
			Assert::IsNull(datum);
		}

		TEST_METHOD(ScopeIndexOperator)
		{
			Scope zork;
			Datum& rooms = zork.append("rooms");
			rooms = 10;
			Scope& inventory = zork.appendScope("inventory");
			Datum& key = inventory.append("key");
			key = 5549;

			Assert::AreEqual(zork["rooms"].get<int32_t>(), 10);
			Assert::IsTrue(zork[0].type() == Datum::DatumType::INTEGER);
			Assert::IsTrue(zork[1].type() == Datum::DatumType::TABLE);
			zork["health"] = 17.2f;
			Datum* health_datum = zork.find("health");
			Assert::IsTrue(health_datum->get<float_t>() == 17.2f);
			auto expression = [&] {Datum& temp = zork[3]; };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(ScopeIndexOperatorConst)
		{
			Scope zork;
			Datum& rooms = zork.append("rooms");
			rooms = 10;
			Scope& inventory = zork.appendScope("inventory");
			Datum& key = inventory.append("key");
			key = 5549;

			const Scope another_zork = /*std::as_const(zork)*/ zork;

			Assert::AreEqual(another_zork["rooms"].get<int32_t>(), 10);
			Assert::IsTrue(another_zork[0].type() == Datum::DatumType::INTEGER);
			Assert::IsTrue(another_zork[1].type() == Datum::DatumType::TABLE);
			zork["health"] = 17.2f;
			const Scope just_another_zork = /*std::as_const(zork);*/ zork;
			const Datum* health_datum = just_another_zork.find("health");
			Assert::IsTrue(health_datum->get<float_t>() == 17.2f);
			auto expression = [&] {Datum temp = just_another_zork[3]; };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(ScopeAppend)
		{
			Scope zork;
			auto expression = [&] {zork.append(""); };
			Assert::ExpectException<std::exception>(expression);
			Datum& rooms = zork.append("rooms");
			rooms = 10;
			Datum& food = zork.append("food");
			food = "almonds";
			Assert::IsTrue(zork.append("rooms") == rooms);
			Assert::IsTrue(zork.append("food") == food);
		}

		TEST_METHOD(ScopeAppendScope)
		{
			Scope zork;
			Datum& rooms = zork.append("rooms");
			Scope& inventory = zork.appendScope("inventory");
			Assert::IsTrue(&zork == inventory.getParent());
			Scope& food = inventory.appendScope("food");
			Assert::IsTrue(&inventory == food.getParent());
			Scope& weapons = inventory.appendScope("weapons");
			Datum& automatic_gun = weapons.append("mp5");
			automatic_gun = 50;
			auto expression = [&] {weapons.appendScope("mp5"); };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(ScopeToString)
		{
			Scope zork;
			Assert::AreEqual("Scope"s, zork.ToString());
			RTTI* ptr;
			ptr = &zork;
			Assert::AreEqual("Scope"s, ptr->ToString());
		}

		TEST_METHOD(ScopeFindName)
		{
			Scope zork;
			Datum& rooms = zork.append("rooms");
			rooms = 10;
			Scope& inventory = zork.appendScope("inventory");
			Datum& key = inventory.append("key");
			key = 5549;
			Scope& another_inventory = zork.appendScope("inventory");
			Assert::IsTrue(zork.findName(another_inventory) == "inventory");
			Scope non_existant_scope;
			Assert::IsTrue(zork.findName(non_existant_scope) == "");
		}

		TEST_METHOD(ScopeTestComparison)
		{
			Scope zork;
			Datum& rooms = zork.append("rooms");
			rooms = 10;
			Scope& inventory = zork.appendScope("inventory");
			Datum& key = inventory.append("key");
			key = 5549;
			Scope& food = inventory.appendScope("food");
			Datum& almonds = food.append("almonds");
			almonds = 43;

			Scope another_zork;
			Datum& another_rooms = another_zork.append("rooms");
			another_rooms = 10;
			Scope& another_inventory = another_zork.appendScope("inventory");
			Datum& another_key = another_inventory.append("key");
			another_key = 5549;
			Scope& another_food = another_inventory.appendScope("food");
			Datum& another_almonds = another_food.append("almonds");
			another_almonds = 43;

			Assert::IsTrue(zork == another_zork);

			Scope zork2;
			Datum& rooms2 = zork.append("rooms");
			rooms2 = 10;
			Scope& inventory2 = zork.appendScope("inventory");
			Datum& key2 = inventory.append("key");
			key2 = 5548;
			Assert::IsTrue(zork != another_zork);

		}

		TEST_METHOD(ScopeAdopt)
		{
			Scope zork;
			Datum& rooms = zork.append("rooms");
			rooms = 10;
			Scope& inventory = zork.appendScope("inventory");
			Datum& key = inventory.append("key");
			key = 5549;
			Scope food;
			Datum& almonds = food.append("almonds");
			almonds = 43;
			zork.adopt("inventory", food);
			Assert::IsTrue(&zork == food.getParent());
			Assert::AreEqual(2U, zork.find("inventory")->size());
			auto expression = [&] {inventory.adopt("Paul", zork); };
			Assert::ExpectException<std::exception>(expression);
			auto expression_two = [&] {inventory.adopt("inventory", inventory); };
			Assert::ExpectException<std::exception>(expression_two);
		}

	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState ScopeTest::s_start_mem_state;
}