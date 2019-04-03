#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include "Factory.h"
#include "JsonTableParseHelper.h"
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
	std::wstring ToString<Entity>(const Entity& t_temp)
	{
		RETURN_WIDE_STRING(t_temp.ToString().c_str());
	}
	template<>
	std::wstring ToString<Sector>(const Sector& t_temp)
	{
		RETURN_WIDE_STRING(t_temp.ToString().c_str());
	}
	template<>
	std::wstring ToString<World>(const World& t_temp)
	{
		RETURN_WIDE_STRING(t_temp.ToString().c_str());
	}
}


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(WorldSectorEntityTest)
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
			{
				string name = "TestEntity";
				Entity entity(name);
				Assert::IsTrue(entity.isPrescribedAttribute("name"s));
				Assert::AreEqual(name, entity.name());
			}
			{
				string name = "TestSector";
				Sector sector(name);
				Assert::IsTrue(sector.isPrescribedAttribute("name"s));
				Assert::AreEqual(name, sector.name());
			}
			{
				string name = "TestWorld";
				World world(name);
				Assert::IsTrue(world.isPrescribedAttribute("name"s));
				Assert::AreEqual(name, world.name());
			}
		}

		TEST_METHOD(NameTest)
		{
			{
				Entity entity("");
				string name = "TestEntity";
				Assert::IsTrue(entity.name().empty());
				entity.setName(name);
				Assert::AreEqual(name, entity.name());
			}
			{
				Sector sector("");
				string name = "TestSector";
				Assert::IsTrue(sector.name().empty());
				sector.setName(name);
				Assert::AreEqual(name, sector.name());
			}
			{
				World world("");
				string name = "TestWorld";
				Assert::IsTrue(world.name().empty());
				world.setName(name);
				Assert::AreEqual(name, world.name());
			}
		}

		TEST_METHOD(CloneTest)
		{
			{
				Entity entity("TestEntity");
				auto clone = entity.clone();
				Entity* entity_clone = clone->As<Entity>();
				Assert::IsNotNull(entity_clone);
				Assert::AreNotSame(entity, *entity_clone);
				delete clone;
			}
			{
				Sector sector("TestSector");
				auto clone = sector.clone();
				Sector* sector_clone = clone->As<Sector>();
				Assert::IsNotNull(sector_clone);
				Assert::AreNotSame(sector, *sector_clone);
				delete clone;
			}
			{
				World world("TestEntity");
				auto clone = world.clone();
				World* world_clone = clone->As<World>();
				Assert::IsNotNull(world_clone);
				Assert::AreNotSame(world, *world_clone);
				delete clone;
			}
		}

		TEST_METHOD(UpdateTest)
		{
			{
				Entity entity("TestEntity");
				GameTime game_time;
				WorldState world_state;
				world_state.setGameTime(game_time);
				entity.update(world_state);
			}

			{
				Sector sector("TestSector");
				GameTime game_time;
				WorldState world_state;
				world_state.setGameTime(game_time);
				sector.update(world_state);
			}

			{
				World world("World");
				GameTime game_time;
				WorldState world_state;
				world_state.setGameTime(game_time);
				world.update(world_state);
			}
		}

		TEST_METHOD(SectorGetterAndSetterTest)
		{
			EntityFactory factory;
			Sector sector("TestSector");
			Sector sector2("TestSector2");
			Entity* entity = sector.createEntity("Entity", "MyAwesomeEntity");
			Assert::IsTrue(&sector == entity->getSector());
			entity->setSector(&sector2);
			Assert::IsTrue(&sector2 == entity->getSector());

		}

		TEST_METHOD(WorldGetterAndSetterTest)
		{
			World world("TestWorld");
			World world2("AnotherTestWorld");
			Sector* sector = world.createSector("TestSector");
			Assert::IsTrue(&world == sector->getWorld());
			sector->setWorld(&world2);
			Assert::IsTrue(&world2 == sector->getWorld());
		}

		TEST_METHOD(ClockTests)
		{
			// Temporary Testing. Needs Rework
			World world("TestWorld");
			GameTime game_time;
			GameTime another_game_time;
			WorldState world_state;
			world_state.setGameTime(game_time);
			world.update(world_state);
			GameTime gameTime = world_state.getGameTime();
			gameTime.SetElapsedGameTime(std::chrono::milliseconds(100));
			gameTime.SetTotalGameTime(std::chrono::milliseconds(200));
			Assert::IsFalse(gameTime.ElapsedGameTime() == gameTime.TotalGameTime());
			Assert::IsFalse(gameTime.ElapsedGameTimeSeconds() == gameTime.TotalGameTimeSeconds());
			GameClock gameClock;
			Assert::IsTrue(gameTime.CurrentTime() != gameClock.CurrentTime());
			gameTime.SetCurrentTime(std::chrono::steady_clock::time_point());
			gameClock.StartTime();
			gameClock.Reset();
			gameClock.LastTime();
			gameClock.UpdateGameTime(another_game_time);
		}

		TEST_METHOD(SectorsAndEntitiesTest)
		{
			EntityFactory factory;
			Assert::IsTrue(factory.find("Entity") == &factory);
			World world("TestWorld");
			Sector* sector = world.createSector("TestSector");
			world.createSector("TestSector2");
			sector->createEntity("Entity"s, "Entity1"s);
			sector->createEntity("Entity"s, "Entity2"s);
			Datum& sectors = world.sectors();
			Assert::AreEqual(sectors.size(), 2U);
			Assert::IsTrue(sector == sectors.get<Scope*>(0));
			Datum& entities = sector->entities();
			Assert::AreEqual(entities.size(), 2U);
		}

		TEST_METHOD(CopySemanticsTest)
		{
			EntityFactory factory;
			Assert::IsTrue(factory.find("Entity") == &factory);
			World world("TestWorld");
			Sector* sector = world.createSector("TestSector"s);
			World world2(world);
			Datum& sectors = world2.sectors();
			Assert::AreEqual(sectors.size(), 1U);
			World world3;
			world3 = world2;
			world3.createSector("TestSector2"s);
			Datum& sectors2 = world3.sectors();
			Assert::AreEqual(sectors2.size(), 2U);
			//Assert::IsTrue(world3.sectors().get<Scope*>(0)->Is(Sector::TypeIdClass()));

			Sector* sector1 = new Sector(*world.createSector("Sector1"s));
			Sector* sector2 = new Sector(*world.createSector("Sector2"s));
			*sector2 = *sector1;
			Assert::AreEqual(sector2->name(), sector1->name());

			Entity* entity1 = new Entity(*sector1->createEntity("Entity", "Entity1"));
			Entity* entity2 = new Entity(*sector1->createEntity("Entity", "Entity2"));
			*entity2 = *entity1;
			Assert::AreEqual(entity2->name(), entity1->name());

			GameTime game_time;
			WorldState world_state;
			world_state.setGameTime(game_time);
			world.update(world_state);
			sector1->update(world_state);

			delete sector1;
			delete sector2;
			delete entity1;
			delete entity2;
		}

		TEST_METHOD(MoveSemanticsTest)
		{
			EntityFactory factory;
			Assert::IsTrue(factory.find("Entity") == &factory);
			World world("TestWorld");
			Sector* sector = world.createSector("TestSector"s);
			World world2(std::move(world));
			Datum& sectors = world2.sectors();
			Assert::AreEqual(sectors.size(), 1U);
			World world3;
			world3 = std::move(world2);
			world3.createSector("TestSector2"s);
			Datum& sectors2 = world3.sectors();
			Assert::AreEqual(sectors2.size(), 2U);

			Sector* sector1 = world3.createSector("Sector1"s);
			Sector* sector2 = new Sector(std::move(*sector1));
			Assert::AreEqual(sector2->name(), "Sector1"s);
			Sector* sector3 = new Sector();
			*sector3 = std::move(*sector2);
			Assert::AreEqual(sector3->name(), "Sector1"s);

			Entity* entity1 = sector3->createEntity("Entity", "Entity1");
			Entity* entity2 = new Entity(std::move(*entity1));
			Assert::AreEqual(entity2->name(), "Entity1"s);
			Entity* entity3 = new Entity();
			*entity3 = std::move(*entity2);
			Assert::AreEqual(entity3->name(), "Entity1"s);

			delete sector1;
			delete sector2;
			delete entity1;
			delete entity2;

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

			Assert::IsTrue("World" == world["name"].get<std::string>(0));

			Scope* sectors = world["sectors"].get<Scope*>(0);
			Assert::IsTrue("Sector1" == (*sectors)["name"].get<std::string>(0));
			Assert::IsTrue("Sector2" == (*sectors)["name"].get<std::string>(1));

			/*Scope* sector2 = temp["sectors"].get<Scope*>(0);
			Scope temp2 = *sector2;
			Assert::IsTrue("Sector2" == temp2["name"].get<std::string>(0));*/


		}

		

	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState WorldSectorEntityTest::s_start_mem_state;
}