#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<> inline
		std::wstring ToString<Foo>(const Foo& t_rhs)
	{
		RETURN_WIDE_STRING(t_rhs.data());
	}

	template<> inline
		std::wstring ToString<Foo>(Foo* t_rhs)
	{
		RETURN_WIDE_STRING((*t_rhs).data());
	}
}

namespace UnitTestLibraryDesktop
{		
	TEST_CLASS(FooTest)
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

#pragma region RTTITest

		TEST_METHOD(RTTI_TESTS)
		{
			Foo a(10);
			RTTI* rtti = &a;
			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsTrue(rtti->Is("Foo"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(Foo::TypeIdClass(), rtti->TypeIdInstance());

			Bar* b = rtti->As<Bar>();
			Assert::IsNull(b);

			Foo* f = rtti->As<Foo>();
			Assert::IsNotNull(f);
			Assert::AreEqual(&a, f);
			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNotNull(r);

			Foo otherFoo(10);
			Assert::IsFalse(rtti->Equals(&otherFoo));

			Bar otherBar(10);
			Assert::IsFalse(rtti->Equals(&otherBar));
		}

#pragma endregion

		TEST_METHOD(TestConstructor)
		{
			// TODO: Your test code here
			Foo a;
			Assert::AreEqual(0, a.data());

			const int data = 10;
			Foo b(data);
			Assert::AreEqual(data, b.data());
		}


	private:
		static _CrtMemState s_start_mem_state;
	};

	_CrtMemState FooTest::s_start_mem_state;
}