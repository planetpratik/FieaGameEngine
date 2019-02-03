#include "pch.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<> inline
		std::wstring ToString<Foo>(const Foo& t_rhs)
	{
		RETURN_WIDE_STRING(t_rhs.data());
	}

	template<> inline
		std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator& t_rhs)
	{
		RETURN_WIDE_STRING((*t_rhs).data());
	}

	template<> inline
		std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator& t_rhs)
	{
		RETURN_WIDE_STRING((*t_rhs).data());
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(VectorTest)
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

#pragma region VectorTests
		TEST_METHOD(VectorTestConstructor)
		{
			Vector<Foo> vector_of_foos;
			Assert::IsTrue(vector_of_foos.isEmpty());
			Assert::AreEqual(5U, vector_of_foos.capacity());
		}

		TEST_METHOD(VectorTestReserve)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{a, b};
			Assert::AreEqual(5U, vector_of_foos.capacity());
			vector_of_foos.reserve(10);
			Assert::AreEqual(10U, vector_of_foos.capacity());
			Assert::IsTrue(a == vector_of_foos[0]);
			Assert::IsTrue(b == vector_of_foos[1]);
		}

		TEST_METHOD(VectorTestCopyConstructor)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{a, b};
			Vector<Foo> second_vector_of_foos(vector_of_foos);
			Assert::IsTrue(a == second_vector_of_foos[0]);
			Assert::IsTrue(b == second_vector_of_foos[1]);
		}

		TEST_METHOD(VectorTestAssignmentOperator)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{a, b};
			Foo c(30);
			Foo d(40);
			Vector<Foo> second_vector_of_foos{c, d};
			second_vector_of_foos = vector_of_foos;
			Assert::IsTrue(a == second_vector_of_foos[0]);
			Assert::IsTrue(b == second_vector_of_foos[1]);
			Assert::AreEqual(2U, second_vector_of_foos.size());
		}

		TEST_METHOD(VectorTestSubscriptOperator)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{a, b};
			Assert::IsTrue(b == vector_of_foos[1]);
			Foo c;
			auto expression = [&] { c = vector_of_foos[4]; };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(VectorTestSubscriptOperatorConst)
		{
			Foo a(20);
			Foo b(40);
			Vector<Foo> vector_of_foos{ a, b };
			const Vector<Foo> const_vector_of_foos = vector_of_foos;
			auto expression = [const_vector_of_foos] { Foo c(const_vector_of_foos[3U]); };
			Assert::ExpectException<std::exception>(expression);
			Assert::IsTrue(a == const_vector_of_foos[0]);
		}

		TEST_METHOD(VectorTestPushBack)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			Foo d(40);
			Foo e(50);
			Foo f(60);
			Vector<Foo> vector_of_foos{ a,b,c,d,e };
			Assert::AreEqual(5U, vector_of_foos.size());
			vector_of_foos.pushBack(f);
			Assert::AreEqual(10U, vector_of_foos.capacity());
		}

		TEST_METHOD(VectorTestPopBack)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{a, b};
			vector_of_foos.popBack();
			Assert::AreEqual(1U, vector_of_foos.size());
			vector_of_foos.popBack();
			Assert::AreEqual(0U, vector_of_foos.size());
			auto expression = [&] {vector_of_foos.popBack();};
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(VectorTestFront)
		{
			Foo a(50);
			Foo b(40);
			Vector<Foo> vector_of_foos{a, b};
			Assert::IsTrue(a == vector_of_foos.front());
			vector_of_foos.popBack();
			vector_of_foos.popBack();
			auto expression = [&] {vector_of_foos.front(); };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(VectorTestFrontConst)
		{
			Foo a(50);
			Foo b(40);
			Vector<Foo> vector_of_foos{a, b};
			const Vector<Foo> const_vector_of_foos = vector_of_foos;
			Assert::IsTrue(a == const_vector_of_foos.front());
			const Vector<Foo> another_const_vector_of_foo;
			auto expression = [&] {another_const_vector_of_foo.front(); };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(VectorTestBack)
		{
			Foo a(20);
			Foo b(100);
			Vector<Foo> vector_of_foos{a, b};
			Assert::IsTrue(b == vector_of_foos.back());
			vector_of_foos.popBack();
			vector_of_foos.popBack();
			auto expression = [&] {vector_of_foos.back(); };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(VectorTestBackConst)
		{
			Foo a(20);
			Foo b(100);
			Vector<Foo> vector_of_foos{a, b};
			const Vector<Foo> const_vector_of_foos = vector_of_foos;
			Assert::IsTrue(b == const_vector_of_foos.back());
			const Vector<Foo> another_const_vector_of_foo;
			auto expression = [&] {another_const_vector_of_foo.back(); };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(VectorTestBegin)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{a, b};
			Vector<Foo>::Iterator it = vector_of_foos.begin();
			Assert::AreEqual(a, *it);
		}

		TEST_METHOD(VectorTestEnd)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{a, b};
			Vector<Foo>::Iterator it = vector_of_foos.end();
			Assert::AreEqual(b, vector_of_foos.back());
		}

		TEST_METHOD(VectorTestCBegin)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{a, b};
			Vector<Foo>::Iterator it = vector_of_foos.begin();
			Assert::IsTrue(a == *it);
			Vector<Foo>::ConstIterator c_it;
			c_it = vector_of_foos.cbegin();
			Assert::IsTrue(a == *c_it);
		}

		TEST_METHOD(VectorTestCEnd)
		{
			Foo a(100);
			Foo b(200);
			Foo c(300);
			Vector<Foo>::ConstIterator c_it;
			Vector<Foo> vector_of_foos{a, b, c};
			Assert::AreEqual(b, vector_of_foos[1]);
			c_it = vector_of_foos.begin();
			c_it++;
			c_it++;
			c_it++;
			//c_it = vector_of_foos.cend();
			//Assert::AreNotEqual(c, *c_it);
			Assert::IsTrue(vector_of_foos.cend() == c_it);
		}

		TEST_METHOD(VectorTestIsEmpty)
		{
			Vector<Foo> vector_of_foos;
			Assert::IsTrue(vector_of_foos.isEmpty());
			Foo a(10);
			Foo b(20);
			vector_of_foos.pushBack(a);
			vector_of_foos.pushBack(b);
			vector_of_foos.remove(a);
			vector_of_foos.popBack();
			Assert::IsTrue(vector_of_foos.isEmpty());
		}

		TEST_METHOD(VectorTestAt)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{a, b};
			Foo c = vector_of_foos.at(1);
			Assert::AreEqual(c, b);
			Foo d = vector_of_foos.at(0);
			Assert::AreEqual(d, a);
			auto expression = [&] {Foo e = vector_of_foos.at(2); };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(VectorTestAtConst)
		{
			Foo a(10);
			Foo b(20);
			const Vector<Foo> vector_of_foos{a, b};
			const Foo c = vector_of_foos.at(1);
			Assert::AreEqual(c, b);
			auto expression = [vector_of_foos] {Foo d = vector_of_foos.at(2); };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(VectorTestFind)
		{
			Foo a(20);
			Foo b(40);
			Foo c(60);
			Foo d(80);
			Vector<Foo> vector_of_foos{a, b, c};
			Vector<Foo>::Iterator it = vector_of_foos.find(b);
			Assert::AreEqual(b, *it);
			it = vector_of_foos.find(c);
			Assert::AreEqual(c, *it);
			Vector<Foo>::Iterator another_it = vector_of_foos.find(d);
			Assert::IsTrue(vector_of_foos.end() == another_it);
		}

		TEST_METHOD(VectorTestRemove)
		{
			Foo a(20);
			Foo b(40);
			Foo c(60);
			Vector<Foo> vector_of_foos{a, b, c};
			vector_of_foos.remove(c);
			Assert::AreEqual(2U, vector_of_foos.size());
			Vector<Foo>::Iterator it = vector_of_foos.begin();
			vector_of_foos.remove(it);
			Assert::AreEqual(1U, vector_of_foos.size());
			Assert::AreEqual(b, *(vector_of_foos.begin()));
			vector_of_foos.remove(a);
			Assert::AreEqual(1U, vector_of_foos.size());
			vector_of_foos.popBack();
			Foo d(80);
			Foo e(100);
			Foo f(120);
			Vector<Foo> another_vector_of_foos{a ,b, c, d, e, f};
			Vector<Foo>::Iterator t_it_start = another_vector_of_foos.begin();
			++t_it_start;
			++t_it_start;
			Vector<Foo>::Iterator t_it_end = another_vector_of_foos.begin();
			t_it_end++;
			t_it_end++;
			t_it_end++;
			t_it_end++;
			another_vector_of_foos.remove(t_it_start, t_it_end);
			Assert::AreEqual(3U, another_vector_of_foos.size());
			Assert::AreEqual(a, *(another_vector_of_foos.begin()));
			Foo g = another_vector_of_foos.at(1);
			Foo h = another_vector_of_foos.at(2);
			Assert::AreEqual(g, b);
			Assert::AreEqual(h, f);

			// Test Exceptions
			Vector<Foo> yet_another_foo{a, b};
			vector_of_foos.pushBack(a);
			Vector<Foo>::Iterator it_one = vector_of_foos.begin();
			Vector<Foo>::Iterator it_two = yet_another_foo.begin();
			auto expression = [&] {another_vector_of_foos.remove(it_one, it_two); };
			Assert::ExpectException<std::exception>(expression);
			Vector<Foo>::Iterator it_three = vector_of_foos.end();
			auto expression_two = [&] {vector_of_foos.remove(it_three, it_one); };
			Assert::ExpectException<std::exception>(expression_two);
			auto expression_three = [&] {vector_of_foos.remove(it_two); };
			Assert::ExpectException<std::exception>(expression_three);
		}
#pragma endregion

#pragma region IteratorTests

		TEST_METHOD(VectorIteratorTestConstructor)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{a,b};
			Vector<Foo>::Iterator it;
			it = vector_of_foos.begin();
			Assert::AreEqual(a, *it);
		}

		TEST_METHOD(VectorIteratorTestCopyConstructor)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{ a,b };
			Vector<Foo>::Iterator it = vector_of_foos.begin();
			Vector<Foo>::Iterator it_two(it);
			Assert::AreEqual(a, *it_two);
		}

		TEST_METHOD(VectorIteratorTestAssignmentOperator)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{a, b};
			Vector<Foo>::Iterator it = vector_of_foos.begin();
			Vector<Foo>::Iterator it_two;
			it_two = it;
			Assert::AreEqual(a, *it_two);
		}

		TEST_METHOD(VectorIteratorTestEqualityOperator)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			Foo d(40);
			Vector<Foo> vector_of_foos{a, b, c, d};
			Vector<Foo>::Iterator it_one = vector_of_foos.begin();
			Vector<Foo>::Iterator it_two = vector_of_foos.begin();
			Assert::IsTrue(it_one == it_two);
			++it_one;
			++it_two;
			Assert::AreEqual(b, *it_one);
			Assert::AreEqual(b, *it_two);
			++it_one;
			++it_two;
			Assert::AreEqual(c, *it_one);
			Assert::AreEqual(c, *it_two);
			Vector<Foo> another_vector_of_foos(vector_of_foos);
			Vector<Foo>::Iterator it_three = another_vector_of_foos.begin();
			Assert::IsTrue(it_one != it_three);
		}

		TEST_METHOD(VectorIteratorPrefixIncrementOperator)
		{
			Vector<Foo> vector_of_foos;
			Vector<Foo>::Iterator it;
			auto expression_one = [&] { ++it; };
			Assert::ExpectException<std::exception>(expression_one);
			it = vector_of_foos.begin();
			auto expression_two = [&] { ++it; };
			Assert::ExpectException<std::exception>(expression_two);
			Foo a(10);
			Foo b(20);
			vector_of_foos.pushBack(a);
			vector_of_foos.pushBack(b);
			it = vector_of_foos.begin();
			Assert::AreEqual(a, *it);
			++it;
			Assert::AreEqual(b, *it);
			++it;
			auto expression_three = [&] { ++it; };
			Assert::ExpectException<std::exception>(expression_three);
		}

		TEST_METHOD(VectorIteratorPostfixIncrementOperator)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos;
			Vector<Foo>::Iterator it;
			auto expression_one = [&] { it++; };
			Assert::ExpectException<std::exception>(expression_one);
			it = vector_of_foos.begin();
			auto expression_two = [&] { it++; };
			Assert::ExpectException<std::exception>(expression_two);
			vector_of_foos.pushBack(a);
			vector_of_foos.pushBack(b);
			it = vector_of_foos.begin();
			Assert::AreEqual(a, *it);
			Vector<Foo>::Iterator it_two = ++it;
			Assert::AreEqual(b, *it);
			Foo c = *(it_two);
			Assert::IsTrue(b == c);
			it++;
			auto expression_three = [&] { it++; };
			Assert::ExpectException<std::exception>(expression_three);
		}

		TEST_METHOD(VectorIteratorDereferenceOperator)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos;
			Vector<Foo>::Iterator it;
			auto expression_one = [&] { Foo c = *(it); };
			Assert::ExpectException<std::exception>(expression_one);
			it = vector_of_foos.begin();
			vector_of_foos.pushBack(a);
			vector_of_foos.pushBack(b);
			it = vector_of_foos.begin();
			Assert::IsTrue(a == *it);
			++it;
			Assert::IsTrue(b == *it);
			++it;
			auto expression = [&] {Foo e = *it; };
			Assert::ExpectException<std::exception>(expression);
		}
#pragma endregion

#pragma region ConstIteratorTests

		TEST_METHOD(VectorConstIteratorTestDefaultConstructor)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{ a,b };
			Vector<Foo>::ConstIterator it;
			it = vector_of_foos.cbegin();
			Assert::AreEqual(a, *it);
		}
		TEST_METHOD(VectorConstIteratorImplicitConversion)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{ a,b };
			Vector<Foo>::Iterator it = vector_of_foos.begin();
			Vector<Foo>::ConstIterator c_it(it);
			Assert::AreEqual(a, *(c_it));
		}
		TEST_METHOD(VectorConstIteratorTestCopyConstructor)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{ a,b };
			Vector<Foo>::ConstIterator it = vector_of_foos.cbegin();
			Vector<Foo>::ConstIterator it_two(it);
			Assert::AreEqual(a, *it_two);
		}

		TEST_METHOD(VectorConstIteratorTestAssignmentOperator)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos{ a, b };
			Vector<Foo>::ConstIterator it = vector_of_foos.cbegin();
			Vector<Foo>::ConstIterator it_two;
			it_two = it;
			Assert::AreEqual(a, *it_two);
		}

		TEST_METHOD(VectorConstIteratorTestEqualityOperator)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			Foo d(40);
			Vector<Foo> vector_of_foos{ a, b, c, d };
			Vector<Foo>::ConstIterator it_one = vector_of_foos.cbegin();
			Vector<Foo>::ConstIterator it_two = vector_of_foos.cbegin();
			Assert::IsTrue(it_one == it_two);
			++it_one;
			++it_two;
			Assert::AreEqual(b, *it_one);
			Assert::AreEqual(b, *it_two);
			++it_one;
			++it_two;
			Assert::AreEqual(c, *it_one);
			Assert::AreEqual(c, *it_two);
			Vector<Foo> another_vector_of_foos(vector_of_foos);
			Vector<Foo>::ConstIterator it_three = another_vector_of_foos.cbegin();
			Assert::IsTrue(it_one != it_three);
		}

		TEST_METHOD(VectorConstIteratorPrefixIncrementOperator)
		{
			Vector<Foo> vector_of_foos;
			Vector<Foo>::ConstIterator it;
			auto expression_one = [&] { ++it; };
			Assert::ExpectException<std::exception>(expression_one);
			it = vector_of_foos.cbegin();
			auto expression_two = [&] { ++it; };
			Assert::ExpectException<std::exception>(expression_two);
			Foo a(10);
			Foo b(20);
			vector_of_foos.pushBack(a);
			vector_of_foos.pushBack(b);
			it = vector_of_foos.cbegin();
			Assert::AreEqual(a, *it);
			++it;
			Assert::AreEqual(b, *it);
			++it;
			auto expression_three = [&] { ++it; };
			Assert::ExpectException<std::exception>(expression_three);
		}

		TEST_METHOD(VectorConstIteratorPostfixIncrementOperator)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos;
			Vector<Foo>::ConstIterator it;
			auto expression_one = [&] { it++; };
			Assert::ExpectException<std::exception>(expression_one);
			it = vector_of_foos.begin();
			auto expression_two = [&] { it++; };
			Assert::ExpectException<std::exception>(expression_two);
			vector_of_foos.pushBack(a);
			vector_of_foos.pushBack(b);
			it = vector_of_foos.cbegin();
			Assert::AreEqual(a, *it);
			Vector<Foo>::ConstIterator it_two = ++it;
			Assert::AreEqual(b, *it);
			const Foo c = *(it_two);
			Assert::IsTrue(b == c);
			it++;
			auto expression_three = [&] { it++; };
			Assert::ExpectException<std::exception>(expression_three);
		}

		TEST_METHOD(VectorConstIteratorDereferenceOperator)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector_of_foos;
			Vector<Foo>::ConstIterator it;
			auto expression_one = [&] { Foo c = *(it); };
			Assert::ExpectException<std::exception>(expression_one);
			it = vector_of_foos.cbegin();
			auto expression_two = [&] { Foo c = *(it); };
			Assert::ExpectException<std::exception>(expression_two);
			vector_of_foos.pushBack(a);
			vector_of_foos.pushBack(b);
			it = vector_of_foos.cbegin();
			Assert::IsTrue(a == *it);
			// As returned reference is const, we can't use mutating methods like SetData from Foo class.
			//(*it).SetData(40);
			++it;
			Assert::IsTrue(b == *it);
			++it;
			auto expression = [&] {Foo e = *it; };
			Assert::ExpectException<std::exception>(expression);
		}
#pragma endregion

	private:
		static _CrtMemState s_start_mem_state;
	};

	_CrtMemState VectorTest::s_start_mem_state;
}