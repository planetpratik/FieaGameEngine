#include "pch.h"
#include "Event.h"
#include "IEventSubscriber.h"
#include "IEventPublisher.h"
#include "EventQueue.h"
#include "SubscriberFoo.h"
#include "Factory.h"
#include "Foo.h"
#include "GameTime.h"
#include <fstream>
#include <string>
#include "CppUnitTest.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Foo>(const Foo& t_temp)
	{
		RETURN_WIDE_STRING(t_temp.ToString().c_str());
	}
}


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventTest)
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

		TEST_METHOD(EventSubscribeUnsubscribeTest)
		{
			Foo foo(50);
			SubscriberFoo sub_foo_one, sub_foo_two;
			Event<Foo> event(foo);
			Event<Foo>::subscribe(sub_foo_one);
			
			Assert::AreEqual(100, sub_foo_one.data());
			Assert::AreEqual(100, sub_foo_two.data());
			event.deliver();
			Assert::AreEqual(50, sub_foo_one.data());
			Assert::AreEqual(100, sub_foo_two.data());
			Event<Foo>::unsubscribe(sub_foo_one);
			Assert::AreEqual(50, sub_foo_one.data());
			Event<Foo>::subscribe(sub_foo_two);
			event.deliver();
			Assert::AreEqual(50, sub_foo_one.data());
			Assert::AreEqual(50, sub_foo_two.data());
			Event<Foo>::unsubscribe(sub_foo_two);
		}

		TEST_METHOD(EventUnsubscribeAllTest)
		{
			Foo foo(50);
			SubscriberFoo sub_foo_one, sub_foo_two;
			Event<Foo>::subscribe(sub_foo_one);
			Event<Foo>::subscribe(sub_foo_two);
			Event<Foo> event(foo);

			Assert::AreEqual(100, sub_foo_one.data());
			Assert::AreEqual(100, sub_foo_two.data());
			event.deliver();
			Assert::AreEqual(50, sub_foo_one.data());
			Assert::AreEqual(50, sub_foo_two.data());
			Event<Foo>::unsubscribe(sub_foo_one);
			Event<Foo>::unsubscribe(sub_foo_two);
			Assert::AreEqual(50, sub_foo_one.data());
			Assert::AreEqual(50, sub_foo_two.data());
		}

		TEST_METHOD(EventMessageTest)
		{
			Foo foo(50);
			SubscriberFoo sub_foo;
			Event<Foo> event(foo);
			Assert::AreEqual(event.message(), foo);
		}

		TEST_METHOD(EventPublisherTimeTest)
		{
			SubscriberFoo sub_foo;
			GameTime game_time;
			game_time.SetCurrentTime(std::chrono::steady_clock::now());
			auto current_time = game_time.CurrentTime();
			auto delay = MilliSeconds(1000);

			Foo foo(50);
			Event<Foo> event(foo);
			Assert::IsFalse(event.timeEnqueued() == current_time);
			Assert::IsFalse(delay == MilliSeconds(0));
			event.setTime(current_time, delay);
			Assert::IsTrue(event.timeEnqueued() == current_time);
			Assert::IsTrue(event.delay() == delay);
		}

		TEST_METHOD(EventPublisherTimeExpiredTest)
		{
			SubscriberFoo sub_foo;
			GameTime game_time;
			game_time.SetCurrentTime(std::chrono::steady_clock::now());
			auto current_time = game_time.CurrentTime();
			auto delay = MilliSeconds(1000);

			Foo foo(50);
			Event<Foo> event(foo);
			event.setTime(current_time, delay);
			Assert::IsFalse(event.isExpired(current_time));
			current_time = current_time += MilliSeconds(1001);
			Assert::IsTrue(event.isExpired(current_time));
		}

		TEST_METHOD(EventQueueIsEmptyTest)
		{
			SubscriberFoo sub_foo;
			Foo foo(50);
			std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(foo);
			EventQueue queue;
			GameTime game_time;
			auto delay = MilliSeconds(1000);
			Assert::IsTrue(queue.isEmpty());
			queue.enqueue(event, game_time, delay);
			Assert::IsTrue(event->delay() == MilliSeconds(1000));
			Assert::IsFalse(queue.isEmpty());
		}

		TEST_METHOD(EventQueueSizeTest)
		{
			SubscriberFoo sub_foo;
			Foo foo(50);
			std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(foo);
			EventQueue queue;
			GameTime game_time;
			auto delay = MilliSeconds(1000);
			Assert::AreEqual(queue.size(), 0U);
			queue.enqueue(event, game_time, delay);
			Assert::IsTrue(event->delay() == MilliSeconds(1000));
			Assert::AreEqual(queue.size(), 1U);
			queue.clear();
			Assert::AreEqual(queue.size(), 0U);
		}

		TEST_METHOD(EventQueueSendTest)
		{
			SubscriberFoo sub_foo_one, sub_foo_two;
			Event<Foo>::subscribe(sub_foo_one);
			Event<Foo>::subscribe(sub_foo_two);
			Foo foo(50);
			EventQueue event_queue;
			std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(foo);
			Assert::AreEqual(100, sub_foo_one.data());
			Assert::AreEqual(100, sub_foo_two.data());
			event_queue.send(event);
			Assert::AreEqual(50, sub_foo_one.data());
			Assert::AreEqual(50, sub_foo_two.data());
			Event<Foo>::unsubscribe(sub_foo_two);
			Event<Foo>::unsubscribe(sub_foo_one);
			//Event<Foo>::unsubscribeAll();
		}

		TEST_METHOD(EventQueueUpdateTest)
		{
			SubscriberFoo sub_foo_one, sub_foo_two;
			Event<Foo>::subscribe(sub_foo_one);
			Event<Foo>::subscribe(sub_foo_two);
			Foo foo(50);
			Foo another_foo(60);
			EventQueue event_queue;

			GameTime game_time;
			game_time.SetCurrentTime(std::chrono::steady_clock::now());
			std::shared_ptr<Event<Foo>> eventOne = std::make_shared<Event<Foo>>(foo);
			std::shared_ptr<Event<Foo>> eventTwo = std::make_shared<Event<Foo>>(another_foo);
			event_queue.enqueue(eventOne, game_time, MilliSeconds(500));
			event_queue.enqueue(eventTwo, game_time, MilliSeconds(1000));
			event_queue.update(game_time);
			Assert::AreEqual(50, sub_foo_one.data());
			Assert::AreEqual(50, sub_foo_two.data());
			
			game_time.SetCurrentTime(game_time.CurrentTime() + MilliSeconds(501));
			event_queue.update(game_time);
			Assert::AreEqual(60, sub_foo_one.data());
			Assert::AreEqual(60, sub_foo_two.data());

			Event<Foo>::unsubscribe(sub_foo_one);
			Event<Foo>::unsubscribe(sub_foo_two);

		}

	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState EventTest::s_start_mem_state;
}