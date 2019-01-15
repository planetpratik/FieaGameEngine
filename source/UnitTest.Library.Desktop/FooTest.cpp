#include "pch.h"
#include "CppUnitTest.h"
//#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

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