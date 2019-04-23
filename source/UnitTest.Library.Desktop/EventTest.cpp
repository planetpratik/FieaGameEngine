#include "pch.h"
#include "Event.h"
#include "IEventSubscriber.h"
#include "IEventPublisher.h"
#include "EventQueue.h"
#include "SubscriberFoo.h"
#include "UnsubscriberFoo.h"
#include "SubscriberEnqueue.h"
#include "Factory.h"
#include "Foo.h"
#include "GameTime.h"
#include "World.h"
#include <fstream>
#include <string>
#include "CppUnitTest.h"
#include <vector>


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

		TEST_METHOD(RTTI_TEST)
		{
			Foo foo;
			Event<Foo> event_foo(foo);
			RTTI* rtti = &event_foo;
			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsTrue(rtti->Is("IEventPublisher"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
			Assert::IsTrue(rtti->Is(IEventPublisher::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Bar* b = rtti->As<Bar>();
			Assert::IsNull(b);

			IEventPublisher* pra = rtti->As<IEventPublisher>();
			Assert::IsNotNull(pra);
			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(IEventPublisher::TypeIdClass());
			Assert::IsNotNull(r);
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
			Event<Foo>::unsubscribeAll();
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
			Event<Foo>::unsubscribeAll();
		}

		TEST_METHOD(EventQueueUpdateTest)
		{
			SubscriberFoo sub_foo_one, sub_foo_two;
			Event<Foo>::subscribe(sub_foo_one);
			Event<Foo>::subscribe(sub_foo_two);
			Foo foo(50);
			Foo another_foo(50);
			EventQueue event_queue;

			GameTime game_time;
			game_time.SetCurrentTime(std::chrono::steady_clock::now());
			std::shared_ptr<Event<Foo>> eventOne = std::make_shared<Event<Foo>>(foo);
			std::shared_ptr<Event<Foo>> eventTwo = std::make_shared<Event<Foo>>(another_foo);
			event_queue.enqueue(eventOne, game_time, MilliSeconds(500));
			event_queue.enqueue(eventTwo, game_time, MilliSeconds(1500));
			event_queue.update(game_time);
			Assert::AreEqual(100, sub_foo_one.data());
			Assert::AreEqual(100, sub_foo_two.data());
			
			game_time.SetCurrentTime(game_time.CurrentTime() + MilliSeconds(501));
			event_queue.update(game_time);
			Assert::AreEqual(50, sub_foo_one.data());
			Assert::AreEqual(50, sub_foo_two.data());

			Event<Foo>::unsubscribeAll();

		}

		TEST_METHOD(AsyncSubscribeUnsubscribeTest)
		{
			SubscriberFoo sub_foos[1000];
			UnsubscriberFoo unsub_foos[500];
			for (int i = 0; i < 500; ++i)
			{
				sub_foos[i].setData(i);
				Event<Foo>::subscribe(sub_foos[i]);
				unsub_foos[i].setData(i);
				Event<Foo>::subscribe(unsub_foos[i]);
			}
			for (int i = 500; i < 1000; ++i)
			{
				sub_foos[i].setData(i);
				Event<Foo>::subscribe(sub_foos[i]);
			}
			Foo foo(200);
			Event<Foo> foo_event(foo);
			foo_event.deliver();
			for (int i = 0; i < 500; ++i)
			{
				Assert::AreEqual(unsub_foos[i].data(), 200);
			}
			for (int i = 0; i < 1000; ++i)
			{
				Assert::AreEqual(sub_foos[i].data(), 200);
			}
			Event<Foo>::unsubscribeAll();
		}

		TEST_METHOD(AsyncEnqueueUpdateTest)
		{
			EventQueue event_queue;
			GameTime game_time;
			SubscriberFoo sub_foos[20];
			vector<shared_ptr<SubscriberEnqueue>> sub_enqueue;
			std::shared_ptr<SubscriberEnqueue> sub_enqueue_ptr[20];
			for (int32_t i = 0; i < 20; ++i)
			{
				SubscriberEnqueue subscriber(event_queue, game_time);
				sub_enqueue_ptr[i] = std::make_shared<SubscriberEnqueue>(subscriber);
				sub_enqueue_ptr[i]->setData(i);
				sub_enqueue.push_back(sub_enqueue_ptr[i]);
			}

			// Subscribe all subscribers
			for (int32_t i = 0; i < 20; ++i)
			{
				sub_foos[i].setData(i);
				Event<Foo>::subscribe(sub_foos[i]);
				Event<Foo>::subscribe(*sub_enqueue[i]);
			}

			// Create and Enqueue Events in Event Queue.
			for (int32_t i = 0; i < 20; ++i)
			{
				Foo temp(2000);
				std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(temp);
				event_queue.enqueue(event, game_time, std::chrono::milliseconds(0));
			}
			game_time.SetCurrentTime(game_time.CurrentTime() + MilliSeconds(500));

			// Call Update to check queued subscribers

			event_queue.update(game_time);
			for (int32_t i = 0; i < 20; ++i)
			{
				Assert::AreEqual(sub_enqueue[i]->data(), 2000);
			}

			game_time.SetCurrentTime(game_time.CurrentTime() + MilliSeconds(500));
			event_queue.update(game_time);

			for (int32_t i = 0; i < 20; ++i)
			{
				Assert::AreEqual(sub_enqueue[i]->data(), 200);
			}
			Event<Foo>::unsubscribeAll();
		}


	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState EventTest::s_start_mem_state;
}