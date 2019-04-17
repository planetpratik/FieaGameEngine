#include "pch.h"
#include "Event.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "IEventSubscriber.h"
#include "IEventPublisher.h"
#include "EventQueue.h"
#include "Factory.h"
#include "GameTime.h"
#include "Reaction.h"
#include "ReactionAttributed.h"
#include "EventMessageAttributed.h"
#include "JsonTableParseHelper.h"
#include "ActionEvent.h"
#include "ActionIncrement.h"
#include <fstream>
#include <string>
#include "CppUnitTest.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	/*template<>
	std::wstring ToString<Foo>(const Foo& t_temp)
	{
		RETURN_WIDE_STRING(t_temp.ToString().c_str());
	}*/
	template<>
	std::wstring ToString<ReactionAttributed>(ReactionAttributed* t_temp)
	{
		RETURN_WIDE_STRING(t_temp->ToString().c_str());
	}

	template<>
	std::wstring ToString<ActionEvent>(ActionEvent* t_temp)
	{
		RETURN_WIDE_STRING(t_temp->ToString().c_str());
	}

	template<>
	std::wstring ToString<EventMessageAttributed>(EventMessageAttributed* t_temp)
	{
		RETURN_WIDE_STRING(t_temp->ToString().c_str());
	}
}


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ReactionTest)
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

		TEST_METHOD(RTTI_Tests)
		{
			// ReactionAttributed RTTI Test

			{
				ReactionAttributed ra;
				RTTI* rtti = &ra;
				Assert::IsFalse(rtti->Is("Bar"s));
				Assert::IsTrue(rtti->Is("Reaction"s));
				Assert::IsTrue(rtti->Is("ActionList"s));
				Assert::IsTrue(rtti->Is("Attributed"s));
				Assert::IsTrue(rtti->Is("Scope"s));
				Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
				Assert::IsTrue(rtti->Is(ReactionAttributed::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(ReactionAttributed::TypeIdClass(), rtti->TypeIdInstance());

				Bar* b = rtti->As<Bar>();
				Assert::IsNull(b);

				ReactionAttributed* pra = rtti->As<ReactionAttributed>();
				Assert::IsNotNull(pra);
				Assert::AreEqual(&ra, pra);
				RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
				Assert::IsNull(r);

				r = rtti->QueryInterface(ReactionAttributed::TypeIdClass());
				Assert::IsNotNull(r);
			}

			// ActionEvent RTTI Test
			{
				ActionEvent aa;
				RTTI* rtti = &aa;
				Assert::IsFalse(rtti->Is("Bar"s));
				Assert::IsTrue(rtti->Is("ActionEvent"s));
				Assert::IsTrue(rtti->Is("Action"s));
				Assert::IsTrue(rtti->Is("Attributed"s));
				Assert::IsTrue(rtti->Is("Scope"s));
				Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
				Assert::IsTrue(rtti->Is(ActionEvent::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(ActionEvent::TypeIdClass(), rtti->TypeIdInstance());

				Bar* b = rtti->As<Bar>();
				Assert::IsNull(b);

				ActionEvent* paa = rtti->As<ActionEvent>();
				Assert::IsNotNull(paa);
				Assert::AreEqual(&aa, paa);
				RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
				Assert::IsNull(r);

				r = rtti->QueryInterface(ActionEvent::TypeIdClass());
				Assert::IsNotNull(r);
			}

			// EventMessageAttributed RTTI Test
			{
				EventMessageAttributed ema;
				RTTI* rtti = &ema;
				Assert::IsFalse(rtti->Is("Bar"s));
				Assert::IsTrue(rtti->Is("EventMessageAttributed"s));
				Assert::IsTrue(rtti->Is("Attributed"s));
				Assert::IsTrue(rtti->Is("Scope"s));
				Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
				Assert::IsTrue(rtti->Is(EventMessageAttributed::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(EventMessageAttributed::TypeIdClass(), rtti->TypeIdInstance());

				Bar* b = rtti->As<Bar>();
				Assert::IsNull(b);

				EventMessageAttributed* pema = rtti->As<EventMessageAttributed>();
				Assert::IsNotNull(pema);
				Assert::AreEqual(&ema, pema);
				RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
				Assert::IsNull(r);

				r = rtti->QueryInterface(EventMessageAttributed::TypeIdClass());
				Assert::IsNotNull(r);
			}

		}


		TEST_METHOD(WorldCreateReactionTest)
		{
			ReactionAttributedFactory reactions_attributed_factory;
			World test_world("TestWorld");
			ReactionAttributed* reaction = test_world.createReaction("ReactionAttributed")->As<ReactionAttributed>();
			auto reactions = &test_world.reactions();
			Assert::IsTrue(reaction == &test_world["Reactions"][0]);
		}

		TEST_METHOD(ReactionAttributedConstructorTest)
		{
			ReactionAttributed reaction_attributed;
			Assert::IsTrue(reaction_attributed.isPrescribedAttribute("this"s));
			Assert::IsTrue(reaction_attributed.isPrescribedAttribute("SubType"s));
		}

		TEST_METHOD(ActionEventConstructorTest)
		{
			ActionEvent action_event;
			Assert::IsTrue(action_event.isPrescribedAttribute("this"s));
			Assert::IsTrue(action_event.isPrescribedAttribute("SubType"s));
			Assert::IsTrue(action_event.isPrescribedAttribute("Delay"s));
		}

		TEST_METHOD(EventMessageAttributedConstructorTest)
		{
			EventMessageAttributed event_message_attributed;
			Assert::IsTrue(event_message_attributed.isPrescribedAttribute("this"s));
			Assert::IsTrue(event_message_attributed.isPrescribedAttribute("SubType"s));
		}

		TEST_METHOD(CloneTest)
		{
			// ReactionAttributed Clone Test
			{
				ReactionAttributedFactory reactions_attributed_factory;
				World test_world("TestWorld");
				ReactionAttributed* reaction = test_world.createReaction("ReactionAttributed")->As<ReactionAttributed>();
				reaction->setSubtype("Integer"s);

				auto clone = reaction->clone();
				ReactionAttributed* reaction_attributed_clone = clone->As<ReactionAttributed>();
				Assert::IsNotNull(reaction_attributed_clone);
				Assert::AreNotSame(reaction, reaction_attributed_clone);
				Assert::AreEqual("Integer"s, reaction_attributed_clone->getSubtype());
				delete clone;
			}

			// ActionEvent Clone Test
			{
				ReactionAttributedFactory reactions_attributed_factory;
				ActionEventFactory action_event_factory;
				EntityFactory entity_factory;
				World test_world("TestWorld");
				Entity* test_entity = test_world.createSector("TestSector"s)->createEntity("Entity"s, "TestEntity"s);
				ActionEvent* test_action = test_entity->createAction("ActionEvent", "TestAction")->As<ActionEvent>();
				test_action->appendAuxillaryAttribute("Number") = 5;
				test_action->setSubtype("Integer"s);

				auto clone = test_action->clone();
				ActionEvent* action_event_clone = clone->As<ActionEvent>();
				Assert::IsNotNull(action_event_clone);
				Assert::AreNotSame(test_action, action_event_clone);
				Assert::AreEqual("Integer"s, action_event_clone->getSubtype());
				Assert::AreEqual(5, (*action_event_clone)["Number"].get<int32_t>());
				delete clone;
			}

			// EventMessageAttributed Clone Test
			{
				ReactionAttributedFactory reactions_attributed_factory;
				ActionEventFactory action_event_factory;
				EntityFactory entity_factory;
				World test_world("TestWorld");
				Entity* test_entity = test_world.createSector("TestSector"s)->createEntity("Entity"s, "TestEntity"s);
				EventMessageAttributed event_message_attributed;
				event_message_attributed.setSubType("Integer");
				event_message_attributed.setWorld(test_world);

				auto clone = event_message_attributed.clone();
				EventMessageAttributed* ema_clone = clone->As<EventMessageAttributed>();
				Assert::IsNotNull(ema_clone);
				Assert::AreNotSame(&event_message_attributed, ema_clone);
				Assert::AreEqual("Integer"s, ema_clone->getSubType());
				Assert::IsTrue(&test_world == ema_clone->getWorld());
				delete clone;
			}
		}

		TEST_METHOD(ActionEventUpdateTest)
		{

			GameTime game_time;
			game_time.SetCurrentTime(std::chrono::steady_clock::now());
			WorldState world_state;
			world_state.setGameTime(game_time);

			ReactionAttributedFactory reactions_attributed_factory;
			EntityFactory entity_factory;
			ActionEventFactory action_event_factory;
			ActionIncrementFactory action_increment_factory;
			World test_world("TestWorld");
			ReactionAttributed* reaction = test_world.createReaction("ReactionAttributed")->As<ReactionAttributed>();
			reaction->setSubtype("Integer"s);
			Assert::AreEqual("Integer"s, reaction->getSubtype());

			ActionIncrement* action_increment = reaction->createAction("ActionIncrement", "action_increment")->As<ActionIncrement>();
			(*action_increment)["Number"] = 10;
			(*action_increment)["Target"] = "Number";

			Entity* test_entity = test_world.createSector("TestSector"s)->createEntity("Entity"s, "TestEntity"s);
			ActionEvent* test_action = test_entity->createAction("ActionEvent", "TestAction")->As<ActionEvent>();
			test_action->appendAuxillaryAttribute("Number") = 5;

			Datum& datum = test_action->appendAuxillaryAttribute("TableAttribute");
			datum.setType(Datum::DatumType::TABLE);
			test_action->setSubtype("Integer");
			test_action->setDelay(MilliSeconds(0));
			Assert::AreEqual("Integer"s, test_action->getSubtype());
			Assert::IsTrue(MilliSeconds(0) == test_action->getDelay());

			test_world.update(world_state);
			Assert::AreEqual(1U, test_world.getEventQueue().size());
			test_world.update(world_state);
			Assert::AreEqual(11, (*action_increment)["Number"].get<int32_t>());

		}

		TEST_METHOD(ParsingFromFileTest)
		{
			GameTime game_time;
			game_time.SetCurrentTime(std::chrono::steady_clock::now());
			WorldState world_state;
			world_state.setGameTime(game_time);

			SectorFactory sectorFactory;
			EntityFactory factory;
			ActionIncrementFactory actionIncrementFactory;
			ReactionAttributedFactory reaction_attributed_factory;

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

			Assert::AreEqual("World"s, world.name());

			Sector* sector1 = world["Sectors"].get<Scope*>(0)->As<Sector>();
			Assert::AreEqual("Sector1"s, sector1->name());

			Sector* sector2 = world["Sectors"].get<Scope*>(1)->As<Sector>();
			Assert::AreEqual("Sector2"s, sector2->name());

			Entity* entity1 = (*sector1)["Entities"].get<Scope*>(0)->As<Entity>();
			Assert::AreEqual("Entity1"s, entity1->name());
			Entity* entity2 = (*sector1)["Entities"].get<Scope*>(1)->As<Entity>();
			Assert::AreEqual("Entity2"s, entity2->name());
			Entity* entity3 = (*sector2)["Entities"].get<Scope*>(0)->As<Entity>();
			Assert::AreEqual("Entity3"s, entity3->name());

			ActionIncrement* action_increment_one = (*entity2)["Actions"].get<Scope*>(0)->As<ActionIncrement>();
			Assert::AreEqual("Number"s, (*action_increment_one)["Target"].get<std::string>(0));
			Assert::AreEqual(100, (*action_increment_one)["Number"].get<int32_t>(0));

			ReactionAttributed* reaction_attributed = (*entity2)["Actions"].get<Scope*>(1)->As<ReactionAttributed>();
			Assert::AreEqual("integer"s, (*reaction_attributed)["SubType"].get<std::string>(0));
			/*ActionIncrement* reaction_action_increment = (*reaction_attributed)["ActionIncrement"].get<Scope*>(0)->As<ActionIncrement>();
			Assert::AreEqual("Number"s, (*reaction_action_increment)["Target"].get<std::string>(0));
			Assert::AreEqual(200, (*reaction_action_increment)["Number"].get<int32_t>(0));*/
			world.update(world_state);
			/*Assert::AreEqual(201, (*reaction_action_increment)["Number"].get<int32_t>(0));*/

			ActionIncrement* action_increment_two = (*entity3)["Actions"].get<Scope*>(0)->As<ActionIncrement>();
			Assert::AreEqual("Number"s, (*action_increment_two)["Target"].get<std::string>(0));
			Assert::AreEqual(201, (*action_increment_two)["Number"].get<int32_t>(0));
			world.update(world_state);
			Assert::AreEqual(202, (*action_increment_two)["Number"].get<int32_t>(0));

		}

	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState ReactionTest::s_start_mem_state;
}