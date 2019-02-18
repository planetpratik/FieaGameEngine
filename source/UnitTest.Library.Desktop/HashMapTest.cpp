#include "pch.h"
#include <string>
#include "DefaultHash.h"
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

	template<> inline
		std::wstring ToString<HashMap<Foo,int>::Iterator>(const HashMap<Foo,int>::Iterator& t_rhs)
	{
		RETURN_WIDE_STRING(t_rhs->second);
	}

	template<> inline
		std::wstring ToString<HashMap<Foo, int>::ConstIterator>(const HashMap<Foo, int>::ConstIterator& t_rhs)
	{
		RETURN_WIDE_STRING(t_rhs->second);
	}
}

namespace FieaGameEngine
{
	template<>
	class DefaultHash<Foo>
	{
	public:
		uint32_t operator()(const Foo& t_key) const
		{
			return t_key.data();
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashMapTest)
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

#pragma region DefaultHashTests

		TEST_METHOD(DefaultHashTestGenericHashFunctor)
		{
			DefaultHash<Bar> default_hash;
			Bar a(10);
			Bar b(20);
			Bar c(10);
			Assert::AreEqual(default_hash(a), default_hash(c));
			Assert::AreNotEqual(default_hash(a), default_hash(b));
		}

		TEST_METHOD(DefaultHashTestIntegerHashFunctor)
		{
			DefaultHash<int> default_hash;
			int a = 10;
			int b = 20;
			int c = 10;
			Assert::AreEqual(default_hash(a), default_hash(c));
			Assert::AreNotEqual(default_hash(a), default_hash(b));
		}

		TEST_METHOD(DefaultHashTestStringHashFunctor)
		{
			DefaultHash<std::string> default_hash;
			std::string a = "Hello";
			std::string b = "World";
			std::string c = "Hello";
			Assert::AreEqual(default_hash(a), default_hash(c));
			Assert::AreNotEqual(default_hash(a), default_hash(b));
		}

		TEST_METHOD(DefaultHashTestCharPointerHashFunctor)
		{
			DefaultHash<char*> default_hash;
			char* a = "Hello";
			char* b = "World";
			char c[6];
			strncpy_s(c, b, strlen(b));
			Assert::AreNotEqual(default_hash(a), default_hash(b));
			Assert::AreEqual(default_hash(b), default_hash(c));
		}

#pragma endregion

#pragma region HashMapTests
		TEST_METHOD(HashMapTestConstructor)
		{
			HashMap<Foo, int> hashmap(3);
			Assert::AreEqual(0U, hashmap.size());
		}

		TEST_METHOD(HashMapTestInitializerList)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			HashMap<Foo, int> hashmap{ {a,10}, {b,20}, {c,30} };
			Assert::AreEqual(3U, hashmap.size());
		}

		TEST_METHOD(HashMapTestFind)
		{
			Foo a(100);
			Foo b(200);
			Foo c(300);
			HashMap<Foo, int> hashmap{ {a,100}, {b,200}, {c,300} };
			HashMap<Foo, int>::Iterator it = hashmap.find(b);
			Assert::IsTrue(b == (*it).first);
			it = hashmap.find(c);
			Assert::IsTrue(c == (*it).first);
			Foo d(400);
			it = hashmap.find(d);
			Assert::IsTrue(it == hashmap.end());
		}

		TEST_METHOD(HashMapTestFindConst)
		{
			Foo a(100);
			Foo b(200);
			Foo c(300);
			HashMap<Foo, int> hashmap{ {a,100}, {b,200}, {c,300} };
			const HashMap<Foo, int> const_hashmap = hashmap;
			HashMap<Foo, int>::ConstIterator it = const_hashmap.find(b);
			Assert::IsTrue(b == (*it).first);
			it = const_hashmap.find(c);
			Assert::IsTrue(c == (*it).first);
			Foo d(400);
			it = const_hashmap.find(d);
			Assert::IsTrue(it == const_hashmap.end());
		}

		TEST_METHOD(HashMapTestInsert)
		{
			Foo a(10);
			Foo b(20);
			HashMap<Foo, int> hashmap(5);
			HashMap<Foo, int>::Iterator it = hashmap.insert(HashMap<Foo,int>::PairType(a, 10));
			Assert::AreEqual(10, (*it).second);
			it = hashmap.insert(HashMap<Foo, int>::PairType(b, 50));
			Assert::AreEqual(50, (*it).second);
			it = hashmap.insert(HashMap<Foo, int>::PairType(a, 40));
			Assert::AreEqual(10, (*it).second);

			HashMap<Foo, int> second_hashmap(5);
			bool result = false;
			HashMap<Foo, int>::Iterator it_two = second_hashmap.insert(HashMap<Foo, int>::PairType(a, 10), result);
			Assert::AreEqual(10, (*it_two).second);
			Assert::IsTrue(result);

			it_two = second_hashmap.insert(HashMap<Foo, int>::PairType(b, 50), result);
			Assert::AreEqual(50, (*it_two).second);
			Assert::IsTrue(result);
			it_two = second_hashmap.insert(HashMap<Foo, int>::PairType(a, 40), result);
			Assert::AreEqual(10, (*it_two).second);
			Assert::IsFalse(result); 

		}

		TEST_METHOD(HashMapTestIndexOperator)
		{
			Foo a(10);
			Foo b(20);
			HashMap<Foo, int> hashmap(5);
			HashMap<Foo, int>::Iterator it = hashmap.insert(HashMap<Foo, int>::PairType(a, 10));
			it = hashmap.insert(HashMap<Foo, int>::PairType(b, 20));
			Assert::AreEqual(10, hashmap[a]);
			Assert::AreEqual(20, hashmap[b]);
			hashmap[a] = 50;
			Assert::AreEqual(50, hashmap[a]);
		}

		TEST_METHOD(HashMapTestRemove)
		{
			Foo a(100);
			Foo b(200);
			Foo c(300);
			HashMap<Foo, int> hashmap(5);
			HashMap<Foo, int>::Iterator it = hashmap.insert(HashMap<Foo, int>::PairType(a, 100));
			it = hashmap.insert(HashMap<Foo, int>::PairType(b, 200));
			hashmap.remove(a);
			it = hashmap.find(a);
			Assert::IsTrue(it == hashmap.end());
			hashmap.remove(c);
			it = hashmap.find(c);
			Assert::IsTrue(it == hashmap.end());
		}

		TEST_METHOD(HashMapTestClear)
		{
			Foo a(100);
			Foo b(200);
			HashMap<Foo, int> hashmap(5);
			HashMap<Foo, int>::Iterator it = hashmap.insert(HashMap<Foo, int>::PairType(a, 100));
			it = hashmap.insert(HashMap<Foo, int>::PairType(b, 200));
			hashmap.clear();
			Assert::AreEqual(0U, hashmap.size());
		}

		TEST_METHOD(HashMapTestContainsKey)
		{
			Foo a(100);
			Foo b(200);
			Foo c(300);
			HashMap<Foo, int> hashmap(5);
			HashMap<Foo, int>::Iterator it = hashmap.insert(HashMap<Foo, int>::PairType(a, 100));
			it = hashmap.insert(HashMap<Foo, int>::PairType(b, 200));
			Assert::IsTrue(hashmap.containsKey(a));
			Assert::IsFalse(hashmap.containsKey(c));
		}

		TEST_METHOD(HashMapTestAt)
		{
			Foo a(100);
			Foo b(200);
			Foo c(300);
			HashMap<Foo, int> hashmap(5);
			HashMap<Foo, int>::Iterator it = hashmap.insert(HashMap<Foo, int>::PairType(a, 100));
			it = hashmap.insert(HashMap<Foo, int>::PairType(b, 200));
			Assert::AreEqual(hashmap.at(b), 200);
			Assert::AreEqual(hashmap.at(a), 100);
			auto expression = [&] { int& d = hashmap.at(c); };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(HashMapTestAtConst)
		{
			Foo a(100);
			Foo b(200);
			Foo c(300);
			HashMap<Foo, int> hashmap(5);
			HashMap<Foo, int>::Iterator it = hashmap.insert(HashMap<Foo, int>::PairType(a, 100));
			it = hashmap.insert(HashMap<Foo, int>::PairType(b, 200));
			const HashMap<Foo, int> const_hashmap(hashmap);
			Assert::AreEqual(const_hashmap.at(b), 200);
			Assert::AreEqual(const_hashmap.at(a), 100);
			auto expression = [&] { int d = const_hashmap.at(c); };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(HashMapTestBegin)
		{
			Foo a(100);
			Foo b(200);
			Foo c(300);
			HashMap<Foo, int> hashmap{ {a,100}, {b,200}, {c,300} };
			HashMap<Foo, int>::Iterator it = hashmap.begin();
			Assert::AreEqual((*it).first, c);
			hashmap.remove(c);
			it = hashmap.begin();
			Assert::AreEqual((*it).first, a);
			hashmap.remove(a);
			it = hashmap.begin();
			Assert::AreEqual((*it).first, b);
		}

		TEST_METHOD(HashMapTestBeginConst)
		{
			Foo a(100);
			Foo b(200);
			HashMap<Foo, int> hashmap{ {a,100}, {b,200} };
			const HashMap<Foo, int> hashmap_two(hashmap);
			HashMap<Foo, int>::ConstIterator it = hashmap_two.begin();
			Assert::AreEqual((*it).first, a);
			const HashMap<Foo, int> hashmap_three(3);
			it = hashmap_three.begin();
			Assert::IsTrue(hashmap_three.end() == it);
		}
		
		TEST_METHOD(HashMapTestCBegin)
		{
			Foo a(1);
			Foo b(2);
			HashMap<Foo, int> hashmap{ {a,1}, {b,2} };
			HashMap<Foo, int>::ConstIterator it = hashmap.cbegin();
			Assert::AreEqual((*it).first, b);
			hashmap.remove(b);
			it = hashmap.cbegin();
			Assert::AreEqual((*it).first, a);
		}

		TEST_METHOD(HashMapTestEnd)
		{
			HashMap<Foo, int> hashmap(3);
			HashMap<Foo, int>::Iterator it = hashmap.end();
			auto expression = [&] { Foo a = (*it).first; };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(HashMapTestEndConst)
		{
			const HashMap<Foo, int> hashmap(3);
			HashMap<Foo, int>::ConstIterator it = hashmap.end();
			auto expression = [&] { Foo a = (*it).first; };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(HashMapTestCEnd)
		{
			HashMap<Foo, int> hashmap(3);
			HashMap<Foo, int>::ConstIterator it = hashmap.cend();
			auto expression = [&] { Foo a = (*it).first; };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(HashMapTestRehash)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			HashMap<Foo, int> hashmap(5);
			hashmap.insert(HashMap<Foo, int>::PairType(a, 10));
			hashmap.insert(HashMap<Foo, int>::PairType(b, 20));
			hashmap.insert(HashMap<Foo, int>::PairType(c, 30));
			float_t previous_load_factor = hashmap.loadFactor();
			hashmap.rehash(10);
			Assert::AreEqual(10, hashmap[a]);
			Assert::AreEqual(20, hashmap[b]);
			Assert::AreEqual(30, hashmap[c]);
			float_t new_load_factor = hashmap.loadFactor();
			Assert::IsTrue(previous_load_factor > new_load_factor);
		}

#pragma endregion

#pragma region IteratorTests
		TEST_METHOD(HashMapTestIteratorConstructor)
		{
			HashMap<Foo, int> hashmap(5);
			HashMap<Foo, int>::Iterator it;
			it = hashmap.begin();
			auto expression = [&] { int a = (*it).second; };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(HashMapTestIteratorPreIncrement)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			HashMap<Foo, int> hashmap{ {a,10}, {b,20}, {c,30} };
			HashMap<Foo, int>::Iterator it;
			auto expression = [&] { ++it; };
			Assert::ExpectException<std::exception>(expression);

			it = hashmap.begin();
			Assert::AreEqual(30, (*it).second);
			++it;
			Assert::AreEqual(10, (*it).second);
			++it;
			Assert::AreEqual(20, (*it).second);
			++it;
			Assert::IsTrue(it == hashmap.end());
			auto expression_two = [&] { ++it; };
			Assert::ExpectException<std::exception>(expression_two);
		}

		TEST_METHOD(HashMapTestIteratorPostIncrement)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			HashMap<Foo, int> hashmap{ {a,10}, {b,20}, {c,30} };
			HashMap<Foo, int>::Iterator it;
			auto expression = [&] { it++; };
			Assert::ExpectException<std::exception>(expression);

			it = hashmap.begin();
			Assert::AreEqual(30, (*it).second);
			it++;
			Assert::AreEqual(10, (*it).second);
			it++;
			Assert::AreEqual(20, (*it).second);
			it++;
			Assert::IsTrue(it == hashmap.end());
			auto expression_two = [&] { it++; };
			Assert::ExpectException<std::exception>(expression_two);
		}

		TEST_METHOD(HashMapTestIteratorDereference)
		{
			Foo a(10);
			Foo b(20);
			HashMap<Foo, int> hashmap{ {a,10}, {b,20} };
			HashMap<Foo, int>::Iterator it;
			auto expression = [&] { int a = (*it).second; };
			Assert::ExpectException<std::exception>(expression);
			it = hashmap.begin();
			Assert::AreEqual(10, (*it).second);
			++it;
			Assert::AreEqual(20, (*it).second);
			++it;
			auto expression_two = [&] { int b = (*it).second; };
			Assert::ExpectException<std::exception>(expression_two);
		}

		TEST_METHOD(HashMapTestIteratoPointerMemberAccess)
		{
			Foo a(10);
			Foo b(20);
			HashMap<Foo, int> hashmap{ {a,10}, {b,20} };
			HashMap<Foo, int>::Iterator it = hashmap.begin();
			Assert::AreEqual(10, it->second);
		}

		TEST_METHOD(HashMapTestIteratorComparison)
		{
			Foo a(10);
			Foo b(20);
			Foo c(20);
			HashMap<Foo, int> hashmap_one{ {a,10}, {b,20}, {c,20} };
			HashMap<Foo, int> hashmap_two{ {a,10} };
			HashMap<Foo, int>::Iterator it_one = hashmap_one.begin();
			HashMap<Foo, int>::Iterator it_two = hashmap_two.begin();
			Assert::IsTrue(it_one != it_two);
			it_two = hashmap_one.begin();
			it_two++;
			Assert::IsTrue(it_one != it_two);
			it_two = hashmap_one.begin();
			Assert::IsTrue(it_one == it_two);
		}

#pragma endregion

#pragma region ConstIteratorTests

		TEST_METHOD(HashMapTestConstIteratorConstructor)
		{
			Foo a(10);
			HashMap<Foo, int> hashmap(5);
			HashMap<Foo, int>::ConstIterator it;
			it = hashmap.cbegin();
			auto expression = [&] { int a = (*it).second; };
			Assert::ExpectException<std::exception>(expression);
			hashmap.insert(HashMap<Foo, int>::PairType(a, 10));
			it = hashmap.cbegin();
			Assert::AreEqual(10, (*it).second);
		}

		TEST_METHOD(HashMapTestConstIteratorImplicitConversion)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			HashMap<Foo, int> hashmap{ {a,10},{b,20},{c,30} };
			HashMap<Foo, int>::Iterator it = hashmap.begin();
			Assert::AreEqual(30, (*it).second);
			HashMap<Foo, int>::ConstIterator c_it(it);
			Assert::AreEqual(30, (*c_it).second);
			c_it++;
			Assert::AreEqual(10, (*c_it).second);
			c_it++;
			Assert::AreEqual(20, (*c_it).second);
		}

		TEST_METHOD(HashMapTestConstIteratorPreIncrement)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			HashMap<Foo, int> hashmap{ {a,10}, {b,20}, {c,30} };
			HashMap<Foo, int>::ConstIterator it;
			auto expression = [&] { ++it; };
			Assert::ExpectException<std::exception>(expression);

			it = hashmap.cbegin();
			Assert::AreEqual(30, (*it).second);
			++it;
			Assert::AreEqual(10, (*it).second);
			++it;
			Assert::AreEqual(20, (*it).second);
			++it;
			Assert::IsTrue(it == hashmap.cend());
			auto expression_two = [&] { ++it; };
			Assert::ExpectException<std::exception>(expression_two);
		}

		TEST_METHOD(HashMapTestConstIteratorPostIncrement)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			HashMap<Foo, int> hashmap{ {a,10}, {b,20}, {c,30} };
			HashMap<Foo, int>::ConstIterator it;
			auto expression = [&] { it++; };
			Assert::ExpectException<std::exception>(expression);

			it = hashmap.cbegin();
			Assert::AreEqual(30, (*it).second);
			it++;
			Assert::AreEqual(10, (*it).second);
			it++;
			Assert::AreEqual(20, (*it).second);
			it++;
			Assert::IsTrue(it == hashmap.cend());
			auto expression_two = [&] { it++; };
			Assert::ExpectException<std::exception>(expression_two);
		}

		TEST_METHOD(HashMapTestConstIteratorDereference)
		{
			Foo a(10);
			Foo b(20);
			HashMap<Foo, int> hashmap{ {a,10}, {b,20} };
			HashMap<Foo, int>::ConstIterator it;
			auto expression = [&] { int a = (*it).second; };
			Assert::ExpectException<std::exception>(expression);
			it = hashmap.cbegin();
			Assert::AreEqual(10, (*it).second);
			++it;
			Assert::AreEqual(20, (*it).second);
			++it;
			auto expression_two = [&] { int b = (*it).second; };
			Assert::ExpectException<std::exception>(expression_two);
		}

		TEST_METHOD(HashMapTestConstIteratorPointerMemberAccess)
		{
			Foo a(10);
			Foo b(20);
			HashMap<Foo, int> hashmap{ {a,10}, {b,20} };
			HashMap<Foo, int>::ConstIterator it = hashmap.cbegin();
			Assert::AreEqual(10, it->second);
		}

		TEST_METHOD(HashMapTestConstIteratorComparison)
		{
			Foo a(10);
			Foo b(20);
			Foo c(20);
			HashMap<Foo, int> hashmap_one{ {a,10}, {b,20}, {c,20} };
			HashMap<Foo, int> hashmap_two{ {a,10} };
			HashMap<Foo, int>::ConstIterator it_one = hashmap_one.cbegin();
			HashMap<Foo, int>::ConstIterator it_two = hashmap_two.cbegin();
			Assert::IsTrue(it_one != it_two);
			it_two = hashmap_one.cbegin();
			it_two++;
			Assert::IsTrue(it_one != it_two);
			it_two = hashmap_one.cbegin();
			Assert::IsTrue(it_one == it_two);
		}

#pragma endregion

	private:
		static _CrtMemState s_start_mem_state;
	};

	_CrtMemState HashMapTest::s_start_mem_state;
}