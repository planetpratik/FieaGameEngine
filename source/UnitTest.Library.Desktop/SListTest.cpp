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
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListTest)
	{
	public:
		SList<int>* list_of_integers;
		SList<int*>* list_of_integer_pointers;
		SList<Foo>* list_of_foos;

		TEST_METHOD_INITIALIZE(Initialize)
		{
			#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&s_start_mem_state);
			#endif

			list_of_integers = new SList<int>();
			list_of_integer_pointers = new SList<int*>();
			list_of_foos = new SList<Foo>();
		}
		TEST_METHOD_CLEANUP(Cleanup)
		{
			delete list_of_integers;
			delete list_of_integer_pointers;
			delete list_of_foos;

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
		
		// Unit Test for Integer List

		// Test constructor SList() for list of Integers
		TEST_METHOD(SListIntTestConstructor)
		{
			// Initially, Constructor should create Empty list.
			Assert::IsTrue(list_of_integers->isEmpty());
		}

		// Test method pushFront() for list of Integers
		TEST_METHOD(SListIntTestPushFront)
		{
			list_of_integers->pushFront(10);
			// When There is only one element in list, m_front and m_back should be equal
			Assert::AreEqual(10, list_of_integers->front());
			Assert::AreEqual(10, list_of_integers->back());
			// When second element is pushed, m_back will remain same but m_front will be changed.
			list_of_integers->pushFront(20);
			Assert::AreEqual(10, list_of_integers->back());
			Assert::AreEqual(20, list_of_integers->front());
			// At this stage, Size of the list should be 2.
			Assert::AreEqual(2, static_cast<int>(list_of_integers->size()));
		}

		// Test method popFront() for list of Integers
		TEST_METHOD(SListIntTestPopFront)
		{
			// If we try to perform popFront() on empty list, it should throw an error
			auto expression = [this]
			{
				list_of_integers->popFront();
			};
			Assert::ExpectException<std::exception>(expression);

			// popFront() should change front element
			list_of_integers->pushFront(10);
			list_of_integers->pushFront(20);
			list_of_integers->popFront();
			Assert::AreEqual(10, list_of_integers->front());

			// List now contains only one element, removing it should make it empty.
			list_of_integers->popFront();
			Assert::IsTrue(list_of_integers->isEmpty());

			// As the list is empty, front and back pointers should be equal to nullptr.
			// So, if we try to get front or back element, it should result in exception.
			auto error_expression = [this]
			{
				list_of_integers->front();
			};
			Assert::ExpectException<std::exception>(error_expression);
		}

		// Test method pushBack() for list of Integers
		TEST_METHOD(SListIntTestPushBack)
		{
			list_of_integers->pushBack(10);
			// When There is only one element in list, m_front and m_back should be equal
			Assert::AreEqual(10, list_of_integers->front());
			Assert::AreEqual(10, list_of_integers->back());
			// When second element is pushed, m_front will remain same but m_back will be changed.
			list_of_integers->pushBack(20);
			Assert::AreEqual(10, list_of_integers->front());
			Assert::AreEqual(20, list_of_integers->back());
			// At this stage, Size of the list should be 2.
			Assert::AreEqual(2, static_cast<int>(list_of_integers->size()));
		}

		// Test method popBack() for list of Integers
		TEST_METHOD(SListIntTestPopBack)
		{
			// If we try to perform popBack() on empty list, it should throw an error
			auto expression = [this]
			{
				list_of_integers->popBack();
			};
			Assert::ExpectException<std::exception>(expression);

			// popBack() should change back element
			list_of_integers->pushBack(10);
			list_of_integers->pushBack(20);
			list_of_integers->popBack();
			Assert::AreEqual(10, list_of_integers->back());

			// List now contains only one element, removing it should make it empty.
			list_of_integers->popBack();
			Assert::IsTrue(list_of_integers->isEmpty());

			// As the list is empty, front and back pointers should be equal to nullptr.
			// So, if we try to get front or back element, it should result in exception.
			auto error_expression = [this]
			{
				list_of_integers->back();
			};
			Assert::ExpectException<std::exception>(error_expression);
		}

		// Test method clear() for list of Integers
		TEST_METHOD(SListIntTestClear)
		{
			list_of_integers->pushFront(10);
			list_of_integers->pushFront(20);
			list_of_integers->pushFront(30);
			list_of_integers->clear();
			// Once list is cleared, size returned should be zero.
			Assert::AreEqual(0, static_cast<int>(list_of_integers->size()));
		}

		// Test method front() for list of Integers
		TEST_METHOD(SListIntTestFront)
		{
			// If list is empty, calling front on empty list should throw an exception
			auto expression = [this]
			{
				list_of_integers->front();
			};
			Assert::ExpectException<std::exception>(expression);
			// Else it will return data associated with front node
			list_of_integers->pushFront(10);
			list_of_integers->pushFront(20);
			Assert::AreEqual(20, list_of_integers->front());
		}

		// Test method back() for list of Integers
		TEST_METHOD(SListIntTestBack)
		{
			// If list is empty, calling back on empty list should throw an exception
			auto expression = [this]
			{
				list_of_integers->back();
			};
			Assert::ExpectException<std::exception>(expression);
			//Else it will return data associated with Last node
			list_of_integers->pushBack(10);
			list_of_integers->pushBack(20);
			Assert::AreEqual(20, list_of_integers->back());
		}

		// Test method size() for list of Integers
		TEST_METHOD(SListIntTestSize)
		{
			// When initialised, size returned should be zero.
			Assert::AreEqual(0, static_cast<int>(list_of_integers->size()));
			// After inserting 2 elements, size should be 2.
			list_of_integers->pushFront(10);
			list_of_integers->pushBack(20);
			Assert::AreEqual(2, static_cast<int>(list_of_integers->size()));
		}

		// Test method isEmpty() for list of Integers
		TEST_METHOD(SListIntTestIsEmpty)
		{
			// When initialised, list will be empty.
			Assert::IsTrue(list_of_integers->isEmpty());
			// After inserting an element, isEmpty() should return false.
			list_of_integers->pushBack(10);
			Assert::IsFalse(list_of_integers->isEmpty());
		}

		// Test copy constructor for list of Integers
		TEST_METHOD(SListIntTestCopyConstructor)
		{
			list_of_integers->pushBack(10);
			list_of_integers->pushBack(20);
			list_of_integers->pushBack(30);
			// Create a copy of current list using copy constructor
			SList<int>* temp_list(list_of_integers);
			// Check that passed list isn't empty.
			Assert::IsFalse(list_of_integers->isEmpty());
			// Print size of the list - It should be 3 at this stage.
			Assert::AreEqual(3, static_cast<int>(temp_list->size()));
			// Remove last element from temp list. Last element in the list should be 20 now.
			temp_list->popBack();
			Assert::AreEqual(20, temp_list->back()); 
			temp_list->popBack();
			// Now list has only one element.
			Assert::AreEqual(10, temp_list->back());
		}

		// Test Assignment operator overload for list of Integers
		TEST_METHOD(SListIntTestAssignOprOverload)
		{
			SList<int> temp_list;
			temp_list.pushBack(10);
			temp_list.pushBack(20);
			temp_list.pushBack(30);
			// Assign temporary list to list of integers
			*list_of_integers = temp_list;
			Assert::AreEqual(30, list_of_integers->back());
			list_of_integers->popBack();
			// After deleting last element, list size should be 2.
			Assert::AreEqual(2, static_cast<int>(list_of_integers->size()));
			Assert::AreEqual(20, list_of_integers->back());
			list_of_integers->popBack();
			// After deleting last element, list size should be 1.
			Assert::AreEqual(1, static_cast<int>(list_of_integers->size()));
			Assert::AreEqual(10, list_of_integers->back());
		}


		// Unit Test for List of Integer pointers

		// Test constructor SList() for list of Integer pointers
		TEST_METHOD(SListIntPointersTestConstructor)
		{
			// Initially, Constructor should create Empty list.
			Assert::IsTrue(list_of_integer_pointers->isEmpty());
		}

		// Test method pushFront() for list of Integer pointers
		TEST_METHOD(SListIntPointersTestPushFront)
		{
			int a = 10;
			int b = 20;
			list_of_integer_pointers->pushFront(&a);
			// When There is only one element in list, m_front and m_back should be equal
			Assert::AreEqual(&a, list_of_integer_pointers->front());
			Assert::AreEqual(&a, list_of_integer_pointers->back());
			// When second element is pushed, m_back will remain same but m_front will be changed.
			list_of_integer_pointers->pushFront(&b);
			Assert::AreEqual(&a, list_of_integer_pointers->back());
			Assert::AreEqual(&b, list_of_integer_pointers->front());
			// At this stage, Size of the list should be 2.
			Assert::AreEqual(2, static_cast<int>(list_of_integer_pointers->size()));
		}

		// Test method popFront() for list of Integer pointers
		TEST_METHOD(SListIntPointersTestPopFront)
		{
			int a = 10;
			int b = 20;
			// If we try to perform popFront() on empty list, it should throw an error
			auto expression = [this]
			{
				list_of_integer_pointers->popFront();
			};
			Assert::ExpectException<std::exception>(expression);

			// popFront() should change front element
			list_of_integer_pointers->pushFront(&a);
			list_of_integer_pointers->pushFront(&b);
			list_of_integer_pointers->popFront();
			Assert::AreEqual(&a, list_of_integer_pointers->front());

			// List now contains only one element, removing it should make it empty.
			list_of_integer_pointers->popFront();
			Assert::IsTrue(list_of_integer_pointers->isEmpty());

			// As the list is empty, front and back pointers should be equal to nullptr.
			// So, if we try to get front or back element, it should result in exception.
			auto error_expression = [this]
			{
				list_of_integer_pointers->front();
			};
			Assert::ExpectException<std::exception>(error_expression);
		}

		// Test method pushBack() for list of Integer Pointers
		TEST_METHOD(SListIntPointersTestPushBack)
		{
			int a = 10;
			int b = 20;
			list_of_integer_pointers->pushBack(&a);
			// When There is only one element in list, m_front and m_back should be equal
			Assert::AreEqual(&a, list_of_integer_pointers->front());
			Assert::AreEqual(&a, list_of_integer_pointers->back());
			// When second element is pushed, m_front will remain same but m_back will be changed.
			list_of_integer_pointers->pushBack(&b);
			Assert::AreEqual(&a, list_of_integer_pointers->front());
			Assert::AreEqual(&b, list_of_integer_pointers->back());
			// At this stage, Size of the list should be 2.
			Assert::AreEqual(2, static_cast<int>(list_of_integer_pointers->size()));
		}

		// Test method popBack() for list of Integer pointers
		TEST_METHOD(SListIntPointersTestPopBack)
		{
			int a = 10;
			int b = 20;
			// If we try to perform popBack() on empty list, it should throw an error
			auto expression = [this]
			{
				list_of_integer_pointers->popBack();
			};
			Assert::ExpectException<std::exception>(expression);

			// popBack() should change back element
			list_of_integer_pointers->pushBack(&a);
			list_of_integer_pointers->pushBack(&b);
			list_of_integer_pointers->popBack();
			Assert::AreEqual(&a, list_of_integer_pointers->back());

			// List now contains only one element, removing it should make it empty.
			list_of_integer_pointers->popBack();
			Assert::IsTrue(list_of_integer_pointers->isEmpty());

			// As the list is empty, front and back pointers should be equal to nullptr.
			// So, if we try to get front or back element, it should result in exception.
			auto error_expression = [this]
			{
				list_of_integer_pointers->back();
			};
			Assert::ExpectException<std::exception>(error_expression);
		}

		// Test method clear() for list of Integer pointers
		TEST_METHOD(SListIntPointersTestClear)
		{
			int a = 10;
			int b = 20;
			int c = 30;
			list_of_integer_pointers->pushFront(&a);
			list_of_integer_pointers->pushFront(&b);
			list_of_integer_pointers->pushFront(&c);
			list_of_integer_pointers->clear();
			// Once list is cleared, size returned should be zero.
			Assert::AreEqual(0, static_cast<int>(list_of_integer_pointers->size()));
		}

		// Test method front() for list of Integer pointers
		TEST_METHOD(SListIntTestPointersTestFront)
		{
			int a = 10;
			int b = 20;
			// If list is empty, calling front on empty list should throw an exception
			auto expression = [this]
			{
				list_of_integer_pointers->front();
			};
			Assert::ExpectException<std::exception>(expression);
			// Else it will return data associated with front node
			list_of_integer_pointers->pushFront(&a);
			list_of_integer_pointers->pushFront(&b);
			Assert::AreEqual(&b, list_of_integer_pointers->front());
		}

		// Test method back() for list of Integer pointers
		TEST_METHOD(SListIntPointersTestBack)
		{
			int a = 10;
			int b = 20;
			// If list is empty, calling back on empty list should throw an exception
			auto expression = [this]
			{
				list_of_integer_pointers->back();
			};
			Assert::ExpectException<std::exception>(expression);
			//Else it will return data associated with Last node
			list_of_integer_pointers->pushBack(&a);
			list_of_integer_pointers->pushBack(&b);
			Assert::AreEqual(&b, list_of_integer_pointers->back());
		}

		// Test method size() for list of Integer pointers
		TEST_METHOD(SListIntPointersTestSize)
		{
			int a = 10;
			int b = 20;
			// When initialised, size returned should be zero.
			Assert::AreEqual(0, static_cast<int>(list_of_integer_pointers->size()));
			// After inserting 2 elements, size should be 2.
			list_of_integer_pointers->pushFront(&a);
			list_of_integer_pointers->pushBack(&b);
			Assert::AreEqual(2, static_cast<int>(list_of_integer_pointers->size()));
		}

		// Test method isEmpty() for list of Integer pointers
		TEST_METHOD(SListIntPointersTestIsEmpty)
		{
			int a = 10;
			// When initialised, list will be empty.
			Assert::IsTrue(list_of_integer_pointers->isEmpty());
			// After inserting an element, isEmpty() should return false.
			list_of_integer_pointers->pushBack(&a);
			Assert::IsFalse(list_of_integer_pointers->isEmpty());
		}

		// Test copy constructor for list of Integer pointers - Also covers assignment operator overload logic.
		TEST_METHOD(SListIntPointersTestCopyConstructor)
		{
			int a = 10;
			int b = 20;
			int c = 30;
			list_of_integer_pointers->pushBack(&a);
			list_of_integer_pointers->pushBack(&b);
			list_of_integer_pointers->pushBack(&c);
			// Create a copy of current list using copy constructor
			SList<int*> temp_list(*list_of_integer_pointers);
			// Check that passed list isn't empty.
			Assert::IsFalse(list_of_integer_pointers->isEmpty());
			// Print size of the list - It should be 3 at this stage.
			Assert::AreEqual(3, static_cast<int>(temp_list.size()));
			// Remove last element from temp list. Last element in the list should be 20 now.
			temp_list.popBack();
			Assert::AreEqual(&b, temp_list.back());
			temp_list.popBack();
			// Now list has only one element.
			Assert::AreEqual(&a, temp_list.back());
		}

		// Test Assignment operator overload for list of Integer pointers
		TEST_METHOD(SListIntPointersTestAssignOprOverload)
		{
			int a = 10;
			int b = 20;
			int c = 30;
			SList<int*> temp_list;
			temp_list.pushBack(&a);
			temp_list.pushBack(&b);
			temp_list.pushBack(&c);
			// Assign temporary list to list of integer pointers
			*list_of_integer_pointers = temp_list;
			Assert::AreEqual(&c, list_of_integer_pointers->back());
			list_of_integer_pointers->popBack();
			// After deleting last element, list size should be 2.
			Assert::AreEqual(2, static_cast<int>(list_of_integer_pointers->size()));
			Assert::AreEqual(&b, list_of_integer_pointers->back());
			list_of_integer_pointers->popBack();
			// After deleting last element, list size should be 1.
			Assert::AreEqual(1, static_cast<int>(list_of_integer_pointers->size()));
			Assert::AreEqual(&a, list_of_integer_pointers->back());
		}

		// Unit Test for List of Foo objects ( User defined type )

		// Test constructor SList() for list of Foos
		TEST_METHOD(SListFoosTestConstructor)
		{
			// Initially, Constructor should create Empty list.
			Assert::IsTrue(list_of_foos->isEmpty());
		}

		// Test method pushFront() for list of Foos
		TEST_METHOD(SListFoosTestPushFront)
		{
			Foo a(10);
			Foo b(20);
			list_of_foos->pushFront(a);
			// When There is only one element in list, m_front and m_back should be equal
			Assert::AreEqual(a, list_of_foos->front());
			Assert::AreEqual(a, list_of_foos->back());
			// When second element is pushed, m_back will remain same but m_front will be changed.
			list_of_foos->pushFront(b);
			Assert::AreEqual(a, list_of_foos->back());
			Assert::AreEqual(b, list_of_foos->front());
			// At this stage, Size of the list should be 2.
			Assert::AreEqual(2, static_cast<int>(list_of_foos->size()));
		}

		// Test method popFront() for list of Foos
		TEST_METHOD(SListFoosTestPopFront)
		{
			Foo a(10);
			Foo b(20);
			// If we try to perform popFront() on empty list, it should throw an error
			auto expression = [this]
			{
				list_of_foos->popFront();
			};
			Assert::ExpectException<std::exception>(expression);

			// popFront() should change front element
			list_of_foos->pushFront(a);
			list_of_foos->pushFront(b);
			list_of_foos->popFront();
			Assert::AreEqual(a, list_of_foos->front());

			// List now contains only one element, removing it should make it empty.
			list_of_foos->popFront();
			Assert::IsTrue(list_of_foos->isEmpty());

			// As the list is empty, front and back pointers should be equal to nullptr.
			// So, if we try to get front or back element, it should result in exception.
			auto error_expression = [this]
			{
				list_of_foos->front();
			};
			Assert::ExpectException<std::exception>(error_expression);
		}

		// Test method pushBack() for list of Foos
		TEST_METHOD(SListFoosTestPushBack)
		{
			Foo a(10);
			Foo b(20);
			list_of_foos->pushBack(a);
			// When There is only one element in list, m_front and m_back should be equal
			Assert::AreEqual(a, list_of_foos->front());
			Assert::AreEqual(a, list_of_foos->back());
			// When second element is pushed, m_front will remain same but m_back will be changed.
			list_of_foos->pushBack(b);
			Assert::AreEqual(a, list_of_foos->front());
			Assert::AreEqual(b, list_of_foos->back());
			// At this stage, Size of the list should be 2.
			Assert::AreEqual(2, static_cast<int>(list_of_foos->size()));
		}

		// Test method popBack() for list of Foos
		TEST_METHOD(SListFoosTestPopBack)
		{
			Foo a(10);
			Foo b(20);
			// If we try to perform popBack() on empty list, it should throw an error
			auto expression = [this]
			{
				list_of_foos->popBack();
			};
			Assert::ExpectException<std::exception>(expression);

			// popBack() should change back element
			list_of_foos->pushBack(a);
			list_of_foos->pushBack(b);
			list_of_foos->popBack();
			Assert::AreEqual(a, list_of_foos->back());

			// List now contains only one element, removing it should make it empty.
			list_of_foos->popBack();
			Assert::IsTrue(list_of_foos->isEmpty());

			// As the list is empty, front and back pointers should be equal to nullptr.
			// So, if we try to get front or back element, it should result in exception.
			auto error_expression = [this]
			{
				list_of_foos->back();
			};
			Assert::ExpectException<std::exception>(error_expression);
		}

		// Test method clear() for list of Foos
		TEST_METHOD(SListFoosTestClear)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			list_of_foos->pushFront(a);
			list_of_foos->pushFront(b);
			list_of_foos ->pushFront(c);
			list_of_foos->clear();
			// Once list is cleared, size returned should be zero.
			Assert::AreEqual(0, static_cast<int>(list_of_foos->size()));
		}

		// Test method front() for list of Foos
		TEST_METHOD(SListFoosTestFront)
		{
			Foo a(10);
			Foo b(20);

			// If list is empty, calling front on empty list should throw an exception
			auto expression = [this]
			{
				list_of_foos->front();
			};
			Assert::ExpectException<std::exception>(expression);
			// Else it will return data associated with front node
			list_of_foos->pushFront(a);
			list_of_foos->pushFront(b);
			Assert::AreEqual(b, list_of_foos->front());
		}

		// Test method back() for list of Foos
		TEST_METHOD(SListFoosTestBack)
		{
			Foo a(10);
			Foo b(20);
			// If list is empty, calling back on empty list should throw an exception
			auto expression = [this]
			{
				list_of_foos->back();
			};
			Assert::ExpectException<std::exception>(expression);
			//Else it will return data associated with Last node
			list_of_foos->pushBack(a);
			list_of_foos->pushBack(b);
			Assert::AreEqual(b, list_of_foos->back());
		}

		// Test method size() for list of Foos
		TEST_METHOD(SListFoosTestSize)
		{
			Foo a(10);
			Foo b(20);
			// When initialised, size returned should be zero.
			Assert::AreEqual(0, static_cast<int>(list_of_foos->size()));
			// After inserting 2 elements, size should be 2.
			list_of_foos->pushFront(a);
			list_of_foos->pushBack(b);
			Assert::AreEqual(2, static_cast<int>(list_of_foos->size()));
		}

		// Test method isEmpty() for list of Foos
		TEST_METHOD(SListFoosTestIsEmpty)
		{
			Foo a(10);
			// When initialised, list will be empty.
			Assert::IsTrue(list_of_foos->isEmpty());
			// After inserting an element, isEmpty() should return false.
			list_of_foos->pushBack(a);
			Assert::IsFalse(list_of_foos->isEmpty());
		}

		// Test copy constructor for list of Foos - Also covers assignment operator overload logic.
		TEST_METHOD(SListFoosTestCopyConstructor)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			list_of_foos->pushBack(a);
			list_of_foos->pushBack(b);
			list_of_foos->pushBack(c);
			// Create a copy of current list using copy constructor
			SList<Foo>* temp_list(list_of_foos);
			// Check that passed list isn't empty.
			Assert::IsFalse(list_of_foos->isEmpty());
			// Print size of the list - It should be 3 at this stage.
			Assert::AreEqual(3, static_cast<int>(temp_list->size()));
			// Remove last element from temp list. Last element in the list should be 20 now.
			temp_list->popBack();
			Assert::AreEqual(b, temp_list->back());
			temp_list->popBack();
			// Now list has only one element.
			Assert::AreEqual(a, temp_list->back());
		}

		// Test Assignment operator overload for list of Foos
		TEST_METHOD(SListFoosTestAssignOprOverload)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			SList<Foo> temp_list;
			temp_list.pushBack(a);
			temp_list.pushBack(b);
			temp_list.pushBack(c);
			// Assign temporary list to list of Foos
			*list_of_foos = temp_list;
			Assert::AreEqual(c, list_of_foos->back());
			list_of_foos->popBack();
			// After deleting last element, list size should be 2.
			Assert::AreEqual(2, static_cast<int>(list_of_foos->size()));
			Assert::AreEqual(b, list_of_foos->back());
			list_of_foos->popBack();
			// After deleting last element, list size should be 1.
			Assert::AreEqual(1, static_cast<int>(list_of_foos->size()));
			Assert::AreEqual(a, list_of_foos->back());
		}

	private:
		static _CrtMemState s_start_mem_state;
	};

	_CrtMemState SListTest::s_start_mem_state;
}