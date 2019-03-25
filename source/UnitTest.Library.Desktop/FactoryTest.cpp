#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<> inline
		std::wstring ToString<Foo>(Foo* t_rhs)
	{
		RETURN_WIDE_STRING((*t_rhs).data());
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FactoryTest)
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

		TEST_METHOD(FactoryAddRemoveTest)
		{
			FooFactory foo_factory;
			RTTI* r = foo_factory.create();
			Assert::IsTrue(r->Is(Foo::TypeIdClass()));
			Assert::IsTrue(Factory<RTTI>::find("Foo") == &foo_factory);
			Assert::AreEqual(1U, foo_factory.getFactoryTableSize());
			delete r;
		}

		TEST_METHOD(FactoryFindTest)
		{
			FooFactory foo_factory;
			Assert::IsTrue(Factory<RTTI>::find("Foo") == &foo_factory);
			Assert::IsTrue(Factory<RTTI>::find("Bar") == nullptr);
		}

		TEST_METHOD(FactoryClassNameTest)
		{
			FooFactory foo_factory;
			Assert::IsTrue(foo_factory.className() == "Foo");
		}

	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState FactoryTest::s_start_mem_state;
}