#include "pch.h"
#include <string>
#include <tuple>
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
		std::wstring ToString<Datum::DatumType>(const Datum::DatumType& t_rhs)
	{
		RETURN_WIDE_STRING(static_cast<int>(t_rhs));
	}

	template<> inline
		std::wstring ToString<Datum::DatumStorageType>(const Datum::DatumStorageType& t_rhs)
	{
		RETURN_WIDE_STRING(static_cast<int>(t_rhs));
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTest)
	{
		Foo** foos_heap;
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
		#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&s_start_mem_state);
		#endif
			foos_heap = new Foo*[3];
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			delete[] foos_heap;
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

#pragma region DatumConstructorsTests

		TEST_METHOD(DatumTestConstructor)
		{
			Datum datum;
			Assert::AreEqual(0U, datum.size());
			Assert::AreEqual(0U, datum.capacity());
			Assert::AreEqual(Datum::DatumType::UNKNOWN, datum.type());
			Assert::AreEqual(Datum::DatumStorageType::UNKNOWN, datum.storageType());
			Assert::IsFalse(datum.isExternal());
		}

		TEST_METHOD(DatumTestCopyConstructor)
		{
			Datum datum;
			datum.set(50);

			Datum another_datum(datum);
			Assert::AreEqual(1U, another_datum.size());
			Assert::AreEqual(1U, another_datum.capacity());
			Assert::AreEqual(Datum::DatumType::INTEGER, another_datum.type());
			Assert::AreEqual(Datum::DatumStorageType::INTERNAL, another_datum.storageType());
			Assert::IsFalse(another_datum.isExternal());
		}

		TEST_METHOD(DatumTestAssignmentOperator)
		{
			Datum datum;
			datum.set(50);

			Datum another_datum;
			another_datum = datum;
			Assert::AreEqual(1U, another_datum.size());
			Assert::AreEqual(1U, another_datum.capacity());
			Assert::AreEqual(Datum::DatumType::INTEGER, another_datum.type());
			Assert::AreEqual(Datum::DatumStorageType::INTERNAL, another_datum.storageType());
			Assert::IsFalse(another_datum.isExternal());
		}

		TEST_METHOD(DatumTestMoveConstrutor)
		{
			Datum datum;
			datum.set(50);

			Datum another_datum = std::move(datum);
			Assert::AreEqual(1U, another_datum.size());
			Assert::AreEqual(1U, another_datum.capacity());
			Assert::AreEqual(Datum::DatumType::INTEGER, another_datum.type());
			Assert::AreEqual(Datum::DatumStorageType::INTERNAL, another_datum.storageType());
			Assert::IsFalse(another_datum.isExternal());

			auto expression = [&] {int32_t temp = datum.get<int32_t>(0); };
			Assert::ExpectException<std::exception>(expression);
			Assert::AreEqual(0U, datum.size());
			Assert::AreEqual(0U, datum.capacity());
			Assert::AreEqual(Datum::DatumType::UNKNOWN, datum.type());
			Assert::AreEqual(Datum::DatumStorageType::UNKNOWN, datum.storageType());
			Assert::IsFalse(datum.isExternal());
		}

		TEST_METHOD(DatumTestMoveAssignmentOperator)
		{
			Datum datum;
			datum.set(50);
			Datum another_datum;
			another_datum = std::move(datum);

			Assert::AreEqual(1U, another_datum.size());
			Assert::AreEqual(1U, another_datum.capacity());
			Assert::AreEqual(Datum::DatumType::INTEGER, another_datum.type());
			Assert::AreEqual(Datum::DatumStorageType::INTERNAL, another_datum.storageType());
			Assert::IsFalse(another_datum.isExternal());

			auto expression = [&] {int32_t temp = datum.get<int32_t>(0); };
			Assert::ExpectException<std::exception>(expression);
			Assert::AreEqual(0U, datum.size());
			Assert::AreEqual(0U, datum.capacity());
			Assert::AreEqual(Datum::DatumType::UNKNOWN, datum.type());
			Assert::AreEqual(Datum::DatumStorageType::UNKNOWN, datum.storageType());
			Assert::IsFalse(datum.isExternal());
		}

#pragma endregion

#pragma region DatumAssignmentTest

		TEST_METHOD(DatumTestAssignment)
		{
			Datum datum;
			datum = 50;
			Assert::AreEqual(50, datum.get<int32_t>());
			auto expression = [&] { datum = 3.2f; };
			Assert::ExpectException<std::exception>(expression);
			Assert::AreEqual(1U, datum.size());
			datum.setSize(3);
			datum.set(100, 1);
			datum.set(150, 2);
			Assert::AreEqual(100, datum.get<int32_t>(1));
			Assert::AreEqual(150, datum.get<int32_t>(2));

			// Internal to Internal Assignment
			Datum internal_datum;
			internal_datum = datum;
			Assert::AreEqual(3U, internal_datum.size());
			Assert::AreEqual(3U, internal_datum.capacity());
			Assert::AreEqual(Datum::DatumType::INTEGER, internal_datum.type());
			Assert::AreEqual(Datum::DatumStorageType::INTERNAL, internal_datum.storageType());
			Assert::IsFalse(internal_datum.isExternal());

			// External to Internal Assignment
			Datum external_datum;
			external_datum = 30;
			int32_t intArray[] = { 10,20,30,40 };
			external_datum.setStorage(intArray, 4U);
			Assert::AreEqual(Datum::DatumStorageType::EXTERNAL, external_datum.storageType());
			Assert::AreEqual(4U, external_datum.size());
			Assert::AreEqual(4U, external_datum.capacity());
			datum = external_datum;
			Assert::AreEqual(4U, datum.size());
			Assert::AreEqual(4U, datum.capacity());
			Assert::AreEqual(Datum::DatumStorageType::EXTERNAL, datum.storageType());
			Assert::IsTrue(datum.isExternal());
			Assert::AreEqual(10, datum.get<int32_t>(0));
			Assert::AreEqual(20, datum.get<int32_t>(1));
			Assert::AreEqual(30, datum.get<int32_t>(2));
			Assert::AreEqual(40, datum.get<int32_t>(3));

			// External to External Assignment
			Datum another_external_datum;
			another_external_datum = 50;
			int32_t another_int_array[] = { 20,30,40,50 };
			another_external_datum.setStorage(another_int_array, 4U);
			external_datum = another_external_datum;
			Assert::AreEqual(4U, external_datum.size());
			Assert::AreEqual(4U, external_datum.capacity());
			Assert::AreEqual(Datum::DatumStorageType::EXTERNAL, external_datum.storageType());
			Assert::IsTrue(external_datum.isExternal());
			Assert::AreEqual(20, external_datum.get<int32_t>(0));
			Assert::AreEqual(30, external_datum.get<int32_t>(1));
			Assert::AreEqual(40, external_datum.get<int32_t>(2));
			Assert::AreEqual(50, external_datum.get<int32_t>(3));

			////Internal to External Assignment
			//external_datum = internal_datum;
			//Assert::AreEqual(3U, external_datum.size());
			//Assert::AreEqual(4U, external_datum.capacity());
			//Assert::AreEqual(Datum::DatumStorageType::INTERNAL, external_datum.storageType());
			//Assert::IsFalse(external_datum.isExternal());
			//Assert::AreEqual(50, external_datum.get<int32_t>(0));
			//Assert::AreEqual(100, external_datum.get<int32_t>(1));
			//Assert::AreEqual(150, external_datum.get<int32_t>(2));
		}
#pragma endregion

#pragma region DatumSizeAndTypeTests

		TEST_METHOD(DatumTestSetSize)
		{
			Datum datum;
			auto expression = [&] { datum.setSize(3); };
			Assert::ExpectException<std::exception>(expression);
			int int_array[] = { 1,2,3 };
			datum = 10;
			datum.setStorage(int_array, 3);
			auto expression_two = [&] { datum.setSize(5); };
			Assert::ExpectException<std::exception>(expression_two);

			Datum int_datum;
			int_datum = 10;
			int_datum.setSize(5);
			Assert::AreEqual(5U, int_datum.size());
			Assert::AreEqual(5U, int_datum.capacity());
			int_datum.setSize(3);
			Assert::AreEqual(3U, int_datum.size());
			Assert::AreEqual(5U, int_datum.capacity());

			Datum float_datum;
			float_datum = 3.2f;
			float_datum.setSize(5);
			Assert::AreEqual(5U, float_datum.size());
			Assert::AreEqual(5U, float_datum.capacity());
			float_datum.setSize(3);
			Assert::AreEqual(3U, float_datum.size());
			Assert::AreEqual(5U, float_datum.capacity());

			Datum vector_datum;
			vector_datum = glm::vec4(1,2,3,4);
			vector_datum.setSize(5);
			Assert::AreEqual(5U, vector_datum.size());
			Assert::AreEqual(5U, vector_datum.capacity());
			vector_datum.setSize(3);
			Assert::AreEqual(3U, vector_datum.size());
			Assert::AreEqual(5U, vector_datum.capacity());

			Datum mat_datum;
			mat_datum = glm::mat4x4(10);
			mat_datum.setSize(5);
			Assert::AreEqual(5U, mat_datum.size());
			Assert::AreEqual(5U, mat_datum.capacity());
			mat_datum.setSize(3);
			Assert::AreEqual(3U, mat_datum.size());
			Assert::AreEqual(5U, mat_datum.capacity());

			Datum string_datum;
			string_datum = "Pratik";
			string_datum.setSize(5);
			Assert::AreEqual(5U, string_datum.size());
			Assert::AreEqual(5U, string_datum.capacity());
			string_datum.setSize(3);
			Assert::AreEqual(3U, string_datum.size());
			Assert::AreEqual(5U, string_datum.capacity());

			Datum rtti_datum;
			Foo a(10);
			rtti_datum = &a;
			rtti_datum.setSize(5);
			Assert::AreEqual(5U, rtti_datum.size());
			Assert::AreEqual(5U, rtti_datum.capacity());
			rtti_datum.setSize(3);
			Assert::AreEqual(3U, rtti_datum.size());
			Assert::AreEqual(5U, rtti_datum.capacity());
		}

		TEST_METHOD(DatumTestSetType)
		{
			Datum datum;
			datum = 10;
			Assert::IsTrue(datum.storageType() == Datum::DatumStorageType::INTERNAL);
			auto expression = [&] { datum.setType(Datum::DatumType::FLOAT); };
			Assert::ExpectException<std::exception>(expression);
		}

#pragma endregion

#pragma region DatumReserveAndSetStorageTests

		TEST_METHOD(DatumTestReserve)
		{
			Datum datum;
			auto expression = [&] { datum.reserve(5); };
			Assert::ExpectException<std::exception>(expression);
			datum = 10;
			int32_t arr[] = { 1,2,3 };
			datum.setStorage(arr, 3);
			auto expression_two = [&] { datum.reserve(5); };
			Assert::ExpectException<std::exception>(expression_two);
			Datum another_datum;
			another_datum = 10;
			another_datum.reserve(10);
			Assert::AreEqual(1U, another_datum.size());
			Assert::AreEqual(10U, another_datum.capacity());
		}

		TEST_METHOD(DatumTestSetStorage)
		{
			Datum datum;
			datum.setType(Datum::DatumType::INTEGER);
			datum = 10;
			float_t float_array[] = { 1.2f,2.2f,3.2f };
			auto expression = [&] { datum.setStorage(float_array, 3); };
			Assert::ExpectException<std::exception>(expression);
			
			Datum float_datum;
			float_datum.setStorage(float_array, 3);
			Assert::AreEqual(3U, float_datum.size());
			Assert::AreEqual(3U, float_datum.capacity());
			Assert::IsTrue(float_array[1] == float_datum.get<float_t>(1));

			glm::vec4 vector_array[] = { glm::vec4(1),glm::vec4(2),glm::vec4(3) };
			Datum vector_datum;
			vector_datum.setStorage(vector_array, 3);
			Assert::AreEqual(3U, vector_datum.size());
			Assert::AreEqual(3U, vector_datum.capacity());
			Assert::IsTrue(vector_array[1] == vector_datum.get<glm::vec4>(1));

			glm::mat4x4 mat_array[] = { glm::mat4x4(1),glm::mat4x4(2),glm::mat4x4(3) };
			Datum mat_datum;
			mat_datum.setStorage(mat_array, 3);
			Assert::AreEqual(3U, mat_datum.size());
			Assert::AreEqual(3U, mat_datum.capacity());
			Assert::IsTrue(mat_array[1] == mat_datum.get<glm::mat4x4>(1));

			std::string string_array[] = { "1", "2", "3" };
			Datum string_datum;
			string_datum.setStorage(string_array, 3);
			Assert::AreEqual(3U, string_datum.size());
			Assert::AreEqual(3U, string_datum.capacity());
			Assert::IsTrue(string_array[1] == string_datum.get<std::string>(1));

			Foo a(10);
			Foo b(20);
			Foo c(30);
			//Foo foos[] = { a,b,c };
			Foo** foos = new Foo*[3];
			foos[0] = &a;
			foos[1] = &b;
			foos[2] = &c;
			Datum rtti_datum;
			rtti_datum.setStorage(reinterpret_cast<RTTI**>(foos), 3);
			Assert::AreEqual(3U, rtti_datum.size());
			Assert::AreEqual(3U, rtti_datum.capacity());
			Assert::IsTrue(rtti_datum.get<RTTI*>(1)->Equals(foos[1]));
			delete[] foos;
		}

#pragma endregion

#pragma region DatumComparisonOperatorTests
		
		TEST_METHOD(DatumTestDatumComparison)
		{
			Datum int_datum;
			int_datum.setType(Datum::DatumType::INTEGER);
			int_datum.pushBack(10);
			int_datum.pushBack(20);
			int_datum.pushBack(30);
			Datum another_int_datum;
			another_int_datum.setType(Datum::DatumType::INTEGER);
			another_int_datum.pushBack(10);
			another_int_datum.pushBack(30);
			another_int_datum.pushBack(30);
			Assert::IsFalse(int_datum == another_int_datum);

			Datum float_datum;
			float_datum.setType(Datum::DatumType::FLOAT);
			float_datum.pushBack(10.2f);
			float_datum.pushBack(20.4f);
			float_datum.pushBack(30.1f);
			Datum another_float_datum;
			another_float_datum.setType(Datum::DatumType::FLOAT);
			another_float_datum.pushBack(10.2f);
			another_float_datum.pushBack(30.1f);
			another_float_datum.pushBack(30.1f);
			Assert::IsFalse(float_datum == another_float_datum);

			Datum string_datum;
			string_datum.setType(Datum::DatumType::STRING);
			string_datum.pushBack("Pratik"s);
			string_datum.pushBack("Paul"s);
			string_datum.pushBack("Ron"s);
			Datum another_string_datum;
			another_string_datum.setType(Datum::DatumType::STRING);
			another_string_datum.pushBack("Pratik"s);
			another_string_datum.pushBack("Ron"s);
			another_string_datum.pushBack("Ron"s);
			Assert::IsFalse(string_datum == another_string_datum);

			Datum vector_datum;
			glm::vec4 a(10);
			glm::vec4 b(20);
			glm::vec4 c(30);
			vector_datum.setType(Datum::DatumType::VECTOR4);
			vector_datum.pushBack(a);
			vector_datum.pushBack(b);
			vector_datum.pushBack(c);
			Datum another_vector_datum;
			another_vector_datum.setType(Datum::DatumType::VECTOR4);
			another_vector_datum.pushBack(a);
			another_vector_datum.pushBack(b);
			another_vector_datum.pushBack(b);
			Assert::IsFalse(vector_datum == another_vector_datum);

			Datum mat_datum;
			glm::mat4x4 d(10);
			glm::mat4x4 e(20);
			glm::mat4x4 f(30);
			mat_datum.setType(Datum::DatumType::MATRIX4X4);
			mat_datum.pushBack(d);
			mat_datum.pushBack(e);
			mat_datum.pushBack(f);
			Datum another_mat_datum;
			another_mat_datum.setType(Datum::DatumType::MATRIX4X4);
			another_mat_datum.pushBack(d);
			another_mat_datum.pushBack(f);
			another_mat_datum.pushBack(f);
			Assert::IsFalse(mat_datum == another_mat_datum);

			Datum rtti_datum;
			Foo x(10);
			Foo y(20);
			Foo z(30);
			rtti_datum.setType(Datum::DatumType::POINTER);
			rtti_datum.pushBack(&x);
			rtti_datum.pushBack(&y);
			rtti_datum.pushBack(&z);
			Datum another_rtti_datum;
			another_rtti_datum.setType(Datum::DatumType::POINTER);
			another_rtti_datum.pushBack(&x);
			another_rtti_datum.pushBack(&x);
			another_rtti_datum.pushBack(&z);
			Assert::IsFalse(rtti_datum == another_rtti_datum);
		}

		TEST_METHOD(DatumTestComparisonOperator)
		{
			Datum datum;
			datum = 10;
			Assert::IsTrue(datum == 10);
			Assert::IsTrue(datum != 20);
			Datum datum_two;
			Assert::IsTrue(datum != datum_two);
			Datum int_datum;
			int_datum.setType(Datum::DatumType::INTEGER);
			int_datum.setSize(5);
			Assert::IsFalse(int_datum == datum);
			
			Datum float_datum;
			float_datum = 15.5f;
			Assert::IsTrue(float_datum != 20.0f);
			Assert::IsFalse(float_datum == 30.0f);
			Datum float_datum_two;
			float_datum_two = 15.5f;
			Assert::IsTrue(float_datum == float_datum_two);

			Datum vector_datum;
			vector_datum = glm::vec4(10);
			Assert::IsTrue(vector_datum != glm::vec4(20));
			Assert::IsFalse(vector_datum == glm::vec4(30));
			Datum vector_datum_two;
			vector_datum_two = glm::vec4(10);
			Assert::IsTrue(vector_datum == vector_datum_two);

			Datum mat_datum;
			mat_datum = glm::mat4x4(10);
			Assert::IsTrue(mat_datum != glm::mat4x4(20));
			Assert::IsFalse(mat_datum == glm::mat4x4(30));
			Datum mat_datum_two;
			mat_datum_two = glm::mat4x4(10);
			Assert::IsTrue(mat_datum == mat_datum_two);

			Datum string_datum;
			string_datum = "Pratik";
			Assert::IsTrue(string_datum != "Paul");
			Assert::IsFalse(string_datum == "Tom");
			Datum string_datum_two;
			string_datum_two = "Pratik";
			Assert::IsTrue(string_datum == string_datum_two);

			Datum rtti_datum;
			Foo a(10);
			Foo b(20);
			rtti_datum = &a;
			Assert::IsTrue(rtti_datum == &a);
			Assert::IsTrue(rtti_datum != &b);
			Datum rtti_datum_two;
			rtti_datum_two = &a;
			Assert::IsTrue(rtti_datum == rtti_datum_two);
		}

#pragma endregion

#pragma region DatumSetterAndGetterTests
		TEST_METHOD(DatumTestSet)
		{
			Datum int_datum;
			int_datum = 10;
			auto expression = [&] { int_datum.set(20.5f); };
			Assert::ExpectException<std::exception>(expression);
			int32_t int_array[] = { 10,20,30 };
			int_datum.setStorage(int_array, 3);
			auto expression_two = [&] { int_datum.set(40,3); };
			Assert::ExpectException<std::exception>(expression_two);
			Datum float_datum;
			float_datum.set(15.5f);
			Assert::AreEqual(15.5f, float_datum.get<float_t>(0));
			Datum vector_datum;
			vector_datum.set(glm::vec4(10));
			Assert::IsTrue(glm::vec4(10) == vector_datum.get<glm::vec4>(0));
			Datum mat_datum;
			mat_datum.set(glm::mat4x4(10));
			Assert::IsTrue(glm::mat4x4(10) == mat_datum.get<glm::mat4x4>(0));
			Datum string_datum;
			string_datum.set("Pratik"s);
			Assert::AreEqual("Pratik"s, string_datum.get<std::string>(0));
			Datum rtti_datum;
			Foo a(10);
			rtti_datum.set(&a);
			Assert::IsTrue(rtti_datum.get<RTTI*>()->Equals(&a));
		}

		TEST_METHOD(DatumTestGet)
		{
			Datum int_datum;
			int_datum = 10;
			auto expression = [&] { int_datum.get<float_t>(0); };
			Assert::ExpectException<std::exception>(expression);
			auto expression_two = [&] { int_datum.get<int32_t>(4); };
			Assert::ExpectException<std::exception>(expression_two);
			Assert::IsTrue(10 == int_datum.get<int32_t>(0));

			Datum float_datum;
			float_datum = 10.4f;
			auto expression_three = [&] { float_datum.get<glm::vec4>(0); };
			Assert::ExpectException<std::exception>(expression_three);
			auto expression_four = [&] { float_datum.get<float_t>(4); };
			Assert::ExpectException<std::exception>(expression_four);
			Assert::IsTrue(10.4f == float_datum.get<float_t>(0));

			Datum vector_datum;
			vector_datum = glm::vec4(10);
			auto expression_five = [&] { vector_datum.get<glm::mat4x4>(0); };
			Assert::ExpectException<std::exception>(expression_five);
			auto expression_six = [&] { vector_datum.get<glm::vec4>(4); };
			Assert::ExpectException<std::exception>(expression_six);
			Assert::IsTrue(glm::vec4(10) == vector_datum.get<glm::vec4>(0));

			Datum mat_datum;
			mat_datum = glm::mat4x4(10);
			auto expression_seven = [&] { mat_datum.get<glm::vec4>(0); };
			Assert::ExpectException<std::exception>(expression_seven);
			auto expression_eight = [&] { mat_datum.get<glm::mat4x4>(4); };
			Assert::ExpectException<std::exception>(expression_eight);
			Assert::IsTrue(glm::mat4x4(10) == mat_datum.get<glm::mat4x4>(0));

			Datum string_datum;
			string_datum = "Pratik"s;
			auto expression_nine = [&] { string_datum.get<int32_t>(0); };
			Assert::ExpectException<std::exception>(expression_nine);
			auto expression_ten = [&] { string_datum.get<std::string>(4); };
			Assert::ExpectException<std::exception>(expression_ten);
			Assert::IsTrue("Pratik"s == string_datum.get<std::string>(0));

			Datum rtti_datum;
			Foo a(10);
			rtti_datum = &a;
			auto expression_eleven = [&] { rtti_datum.get<std::string>(0); };
			Assert::ExpectException<std::exception>(expression_eleven);
			auto expression_twelve = [&] { rtti_datum.get<RTTI*>(4); };
			Assert::ExpectException<std::exception>(expression_twelve);
			Assert::IsTrue(rtti_datum.get<RTTI*>()->Equals(&a));
		}

		TEST_METHOD(DatumTestGetConst)
		{
			Datum int_datum;
			int_datum = 10;
			const Datum const_int_datum = int_datum;
			auto expression = [&] { const_int_datum.get<float_t>(0); };
			Assert::ExpectException<std::exception>(expression);
			auto expression_two = [&] { const_int_datum.get<int32_t>(4); };
			Assert::ExpectException<std::exception>(expression_two);
			Assert::IsTrue(10 == const_int_datum.get<int32_t>(0));

			Datum float_datum;
			float_datum = 10.4f;
			const Datum const_float_datum = float_datum;
			auto expression_three = [&] { const_float_datum.get<glm::vec4>(0); };
			Assert::ExpectException<std::exception>(expression_three);
			auto expression_four = [&] { const_float_datum.get<float_t>(4); };
			Assert::ExpectException<std::exception>(expression_four);
			Assert::IsTrue(10.4f == const_float_datum.get<float_t>(0));

			Datum vector_datum;
			vector_datum = glm::vec4(10);
			const Datum const_vector_datum = vector_datum;
			auto expression_five = [&] { const_vector_datum.get<glm::mat4x4>(0); };
			Assert::ExpectException<std::exception>(expression_five);
			auto expression_six = [&] { const_vector_datum.get<glm::vec4>(4); };
			Assert::ExpectException<std::exception>(expression_six);
			Assert::IsTrue(glm::vec4(10) == const_vector_datum.get<glm::vec4>(0));

			Datum mat_datum;
			mat_datum = glm::mat4x4(10);
			const Datum const_mat_datum = mat_datum;
			auto expression_seven = [&] { const_mat_datum.get<glm::vec4>(0); };
			Assert::ExpectException<std::exception>(expression_seven);
			auto expression_eight = [&] { const_mat_datum.get<glm::mat4x4>(4); };
			Assert::ExpectException<std::exception>(expression_eight);
			Assert::IsTrue(glm::mat4x4(10) == const_mat_datum.get<glm::mat4x4>(0));

			Datum string_datum;
			string_datum = "Pratik"s;
			const Datum const_string_datum = string_datum;
			auto expression_nine = [&] { const_string_datum.get<int32_t>(0); };
			Assert::ExpectException<std::exception>(expression_nine);
			auto expression_ten = [&] { const_string_datum.get<std::string>(4); };
			Assert::ExpectException<std::exception>(expression_ten);
			Assert::IsTrue("Pratik"s == const_string_datum.get<std::string>(0));

			Datum rtti_datum;
			Foo a(10);
			rtti_datum = &a;
			auto expression_eleven = [&] { rtti_datum.get<std::string>(0); };
			Assert::ExpectException<std::exception>(expression_eleven);
			auto expression_twelve = [&] { rtti_datum.get<RTTI*>(4); };
			Assert::ExpectException<std::exception>(expression_twelve);
			Assert::IsTrue(rtti_datum.get<RTTI*>()->Equals(&a));
		}
#pragma endregion

#pragma region DatumUtilityTests

		TEST_METHOD(DatumTestSetFromString)
		{
			Datum int_datum;
			auto expression = [&] { int_datum.setFromString("10"); };
			Assert::ExpectException<std::exception>(expression);
			int_datum.setType(Datum::DatumType::INTEGER);
			int_datum.setFromString("10");
			Assert::IsTrue(10 == int_datum.get<int32_t>(0));

			Datum float_datum;
			float_datum.setType(Datum::DatumType::FLOAT);
			float_datum.setFromString("10.5f");
			Assert::IsTrue(10.5f == float_datum.get<float_t>(0));

			Datum vector_datum;
			vector_datum.setType(Datum::DatumType::VECTOR4);
			vector_datum.setFromString("vec4(10, 20, 30, 40)");
			Assert::IsTrue(glm::vec4(10, 20, 30, 40) == vector_datum.get <glm::vec4>(0));

			Datum mat_datum;
			mat_datum.setType(Datum::DatumType::MATRIX4X4);
			mat_datum.setFromString("mat4x4((10, 20, 30, 40), (10, 20, 30, 40), (10, 20, 30, 40), (10, 20, 30, 40))");
			Assert::IsTrue(glm::mat4x4(10, 20, 30, 40, 10, 20, 30, 40, 10, 20, 30, 40, 10, 20, 30, 40) == mat_datum.get<glm::mat4x4>(0));

			Datum string_datum;
			string_datum.setType(Datum::DatumType::STRING);
			string_datum.setFromString("Pratik"s);
			Assert::IsTrue("Pratik" == string_datum.get<std::string>(0));

			Datum rtti_datum;
			Foo a(10);
			rtti_datum.setType(Datum::DatumType::POINTER);
			auto expression_two = [&] { rtti_datum.setFromString("&a"); };
			Assert::ExpectException<std::exception>(expression_two);
		}

		TEST_METHOD(DatumTestToString)
		{
			Datum int_datum;
			auto expression = [&] { int_datum.toString(); };
			Assert::ExpectException<std::exception>(expression);
			int_datum.setType(Datum::DatumType::INTEGER);
			int_datum = 10;
			Assert::IsTrue("10" == int_datum.toString());

			Datum float_datum;
			float_datum.setType(Datum::DatumType::FLOAT);
			float_datum = 15.5f;
			Assert::AreEqual("15.500000"s,float_datum.toString());

			Datum vector_datum;
			vector_datum.setType(Datum::DatumType::VECTOR4);
			vector_datum.setFromString("vec4(10, 20, 30, 40)");
			Assert::AreEqual("vec4(10.000000, 20.000000, 30.000000, 40.000000)"s, vector_datum.toString());

			Datum mat_datum;
			mat_datum.setType(Datum::DatumType::MATRIX4X4);
			mat_datum.setFromString("mat4x4((10, 20, 30, 40), (10, 20, 30, 40), (10, 20, 30, 40), (10, 20, 30, 40))");
			Assert::AreEqual("mat4x4((10.000000, 20.000000, 30.000000, 40.000000), (10.000000, 20.000000, 30.000000, 40.000000), (10.000000, 20.000000, 30.000000, 40.000000), (10.000000, 20.000000, 30.000000, 40.000000))"s, mat_datum.toString());
			
			Datum string_datum;
			string_datum.setType(Datum::DatumType::STRING);
			string_datum.setFromString("Pratik"s);
			Assert::AreEqual("Pratik"s, string_datum.toString());

			Datum rtti_datum;
			rtti_datum.setType(Datum::DatumType::POINTER);
			Foo a(10);
			rtti_datum = &a;
			Assert::AreEqual("RTTI"s, rtti_datum.toString());
		}

#pragma endregion

#pragma region DatumPushAndPopBackTest

		TEST_METHOD(DatumTestPushBack)
		{
			{
				Datum int_datum;
				Datum another_int_datum;
				Datum temp_datum;
				temp_datum.setType(Datum::DatumType::POINTER);
				int32_t int_arr[] = { 10,20,30 };
				another_int_datum.setStorage(int_arr, 3);
				auto expression = [&] {another_int_datum.pushBack(40); };
				Assert::ExpectException<std::exception>(expression);
				int_datum.pushBack(40);
				Assert::IsTrue(3 == int_datum.capacity());
				Assert::IsTrue(1 == int_datum.size());
				Assert::IsTrue(40 == int_datum.get<int32_t>());
				auto expression_two = [&] {temp_datum.pushBack(40); };
				Assert::ExpectException<std::exception>(expression_two);
			}
			{
				Datum float_datum;
				Datum another_float_datum;
				Datum temp_datum;
				temp_datum.setType(Datum::DatumType::POINTER);
				float_t float_arr[] = { 10.5f,20.3f,30.1f };
				another_float_datum.setStorage(float_arr, 3);
				auto expression = [&] {another_float_datum.pushBack(40.5f); };
				Assert::ExpectException<std::exception>(expression);
				float_datum.pushBack(40.5f);
				Assert::IsTrue(3 == float_datum.capacity());
				Assert::IsTrue(1 == float_datum.size());
				Assert::IsTrue(40.500000 == float_datum.get<float_t>());
				auto expression_two = [&] {temp_datum.pushBack(40.5f); };
				Assert::ExpectException<std::exception>(expression_two);
			}
			{
				Datum vector_datum;
				Datum another_vector_datum;
				Datum temp_datum;
				temp_datum.setType(Datum::DatumType::POINTER);
				glm::vec4 vector_arr[] = { glm::vec4(10),glm::vec4(20),glm::vec4(30) };
				another_vector_datum.setStorage(vector_arr, 3);
				auto expression = [&] {another_vector_datum.pushBack(glm::vec4(40)); };
				Assert::ExpectException<std::exception>(expression);
				vector_datum.pushBack(glm::vec4(40));
				Assert::IsTrue(3 == vector_datum.capacity());
				Assert::IsTrue(1 == vector_datum.size());
				Assert::IsTrue(glm::vec4(40) == vector_datum.get<glm::vec4>());
				auto expression_two = [&] {temp_datum.pushBack(glm::vec4(40)); };
				Assert::ExpectException<std::exception>(expression_two);
			}
			{
				Datum mat_datum;
				Datum another_mat_datum;
				Datum temp_datum;
				temp_datum.setType(Datum::DatumType::POINTER);
				glm::mat4x4 mat_arr[] = { glm::mat4x4(10),glm::mat4x4(20),glm::mat4x4(30) };
				another_mat_datum.setStorage(mat_arr, 3);
				auto expression = [&] {another_mat_datum.pushBack(glm::mat4x4(40)); };
				Assert::ExpectException<std::exception>(expression);
				mat_datum.pushBack(glm::mat4x4(40));
				Assert::IsTrue(3 == mat_datum.capacity());
				Assert::IsTrue(1 == mat_datum.size());
				Assert::IsTrue(glm::mat4x4(40) == mat_datum.get<glm::mat4x4>());
				auto expression_two = [&] {temp_datum.pushBack(glm::mat4x4(40)); };
				Assert::ExpectException<std::exception>(expression_two);
			}
			{
				Datum string_datum;
				Datum another_string_datum;
				Datum temp_datum;
				temp_datum.setType(Datum::DatumType::POINTER);
				std::string string_arr[] = { "Pratik"s, "Paul"s, "John"s, "Dakky"s };
				another_string_datum.setStorage(string_arr, 4);
				auto expression = [&] {another_string_datum.pushBack("Pratik"s); };
				Assert::ExpectException<std::exception>(expression);
				string_datum.pushBack("Pratik"s);
				Assert::IsTrue(3 == string_datum.capacity());
				Assert::IsTrue(1 == string_datum.size());
				Assert::IsTrue("Pratik"s == string_datum.get<std::string>());
				auto expression_two = [&] {temp_datum.pushBack("Pratik"s); };
				Assert::ExpectException<std::exception>(expression_two);
			}
			{
				Datum rtti_datum;
				Datum another_rtti_datum;
				Datum temp_datum;
				temp_datum.setType(Datum::DatumType::INTEGER);
				Foo a(10);
				Foo b(20);
				Foo c(30);
				Foo d(40);
				
				foos_heap[0] = &a;
				foos_heap[1] = &b;
				foos_heap[2] = &c;
				another_rtti_datum.setStorage(reinterpret_cast<RTTI**>(foos_heap), 3);
				auto expression = [&] {another_rtti_datum.pushBack(&d);};
				Assert::ExpectException<std::exception>(expression);
				rtti_datum.pushBack(&d);
				Assert::IsTrue(3 == rtti_datum.capacity());
				Assert::IsTrue(1 == rtti_datum.size());
				Assert::IsTrue(rtti_datum.get<RTTI*>(0)->Equals(reinterpret_cast<RTTI*>(&d)));
				auto expression_two = [&] {temp_datum.pushBack(&d);};
				Assert::ExpectException<std::exception>(expression_two);
			}
		}

		TEST_METHOD(DatumTestPopBack)
		{
			Datum datum;
			auto expression = [&] {datum.popBack(); };
			Assert::ExpectException<std::exception>(expression);
			datum.setType(Datum::DatumType::INTEGER);
			auto expression_two = [&] {datum.popBack(); };
			Assert::ExpectException<std::exception>(expression_two);
			int32_t int_arr[] = { 1,2,3,4 };
			datum.setStorage(int_arr, 4);
			auto expression_three = [&] {datum.popBack(); };
			Assert::ExpectException<std::exception>(expression_three);
			Datum int_datum;
			int_datum = 10;
			Assert::IsTrue(1 == int_datum.size());
			int_datum.popBack();
			Assert::IsTrue(0 == int_datum.size());
			Datum float_datum;
			float_datum = 15.5f;
			Assert::IsTrue(1 == float_datum.size());
			float_datum.popBack();
			Assert::IsTrue(0 == float_datum.size());
			Datum vector_datum;
			vector_datum = glm::vec4(10);
			Assert::IsTrue(1 == vector_datum.size());
			vector_datum.popBack();
			Assert::IsTrue(0 == vector_datum.size());
			Datum mat_datum;
			mat_datum = glm::mat4(10);
			Assert::IsTrue(1 == mat_datum.size());
			mat_datum.popBack();
			Assert::IsTrue(0 == mat_datum.size());
			Datum string_datum;
			string_datum = "Pratik"s;
			Assert::IsTrue(1 == string_datum.size());
			string_datum.popBack();
			Assert::IsTrue(0 == string_datum.size());
			Datum rtti_datum;
			Foo a(10);
			rtti_datum = &a;
			Assert::IsTrue(1 == rtti_datum.size());
			rtti_datum.popBack();
			Assert::IsTrue(0 == rtti_datum.size());
		}
#pragma endregion

#pragma region DatumFrontBackTests

		TEST_METHOD(DatumTestFront)
		{
			{
				Datum int_datum;
				auto expression = [&] { int_datum.front<int32_t>(); };
				Assert::ExpectException<std::exception>(expression);
				int_datum = 10;
				Assert::IsTrue(10 == int_datum.front<int32_t>());
			}
			{
				Datum float_datum;
				auto expression = [&] { float_datum.front<float_t>(); };
				Assert::ExpectException<std::exception>(expression);
				float_datum = 15.4f;
				Assert::AreEqual(15.4f,float_datum.front<float_t>());
			}
			{
				Datum vector_datum;
				auto expression = [&] { vector_datum.front<glm::vec4>(); };
				Assert::ExpectException<std::exception>(expression);
				vector_datum = glm::vec4(10);
				Assert::IsTrue(glm::vec4(10) == vector_datum.front<glm::vec4>());
			}
			{
				Datum mat_datum;
				auto expression = [&] { mat_datum.front<glm::mat4x4>(); };
				Assert::ExpectException<std::exception>(expression);
				mat_datum = glm::mat4x4(10);
				Assert::IsTrue(glm::mat4x4(10) == mat_datum.front<glm::mat4x4>());
			}
			{
				Datum string_datum;
				auto expression = [&] { string_datum.front<std::string>(); };
				Assert::ExpectException<std::exception>(expression);
				string_datum = "Pratik"s;
				Assert::IsTrue("Pratik"s == string_datum.front<std::string>());
			}
			{
				Datum rtti_datum;
				auto expression = [&] { rtti_datum.front<RTTI*>(); };
				Assert::ExpectException<std::exception>(expression);
				Foo a(10);
				rtti_datum = &a;
				//Assert::IsTrue(rtti_datum.get<RTTI*>()->Equals(reinterpret_cast<RTTI*>(&a)));
				Assert::IsTrue(rtti_datum.front<RTTI*>()->Equals(reinterpret_cast<RTTI*>(&a)));
			}
		}

		TEST_METHOD(DatumTestFrontConst)
		{
			{
				Datum temp_datum;
				const Datum int_datum = temp_datum;
				auto expression = [&] { int_datum.front<int32_t>(); };
				Assert::ExpectException<std::exception>(expression);
				temp_datum = 10;
				const Datum another_const_datum = temp_datum;
				Assert::IsTrue(10 == another_const_datum.front<int32_t>());
			}
			{
				Datum temp_datum;
				const Datum float_datum = temp_datum;
				auto expression = [&] { float_datum.front<float_t>(); };
				Assert::ExpectException<std::exception>(expression);
				temp_datum = 15.4f;
				const Datum another_const_datum = temp_datum;
				Assert::AreEqual(15.4f, another_const_datum.front<float_t>());
			}
			{
				Datum temp_datum;
				const Datum vector_datum = temp_datum;
				auto expression = [&] { vector_datum.front<glm::vec4>(); };
				Assert::ExpectException<std::exception>(expression);
				temp_datum = glm::vec4(10);
				const Datum another_vector_datum = temp_datum;
				Assert::IsTrue(glm::vec4(10) == another_vector_datum.front<glm::vec4>());
			}
			{
				Datum temp_datum;
				const Datum mat_datum = temp_datum;
				auto expression = [&] { mat_datum.front<glm::mat4x4>(); };
				Assert::ExpectException<std::exception>(expression);
				temp_datum = glm::mat4x4(10);
				const Datum another_mat_datum = temp_datum;
				Assert::IsTrue(glm::mat4x4(10) == another_mat_datum.front<glm::mat4x4>());
			}
			{
				Datum temp_datum;
				const Datum string_datum = temp_datum;
				auto expression = [&] { string_datum.front<std::string>(); };
				Assert::ExpectException<std::exception>(expression);
				temp_datum = "Pratik"s;
				const Datum another_string_datum = temp_datum;
				Assert::IsTrue("Pratik"s == another_string_datum.front<std::string>());
			}
			{
				Datum temp_datum;
				const Datum rtti_datum = temp_datum;
				auto expression = [&] { rtti_datum.front<RTTI*>(); };
				Assert::ExpectException<std::exception>(expression);
				Foo a(10);
				temp_datum = &a;
				const Datum another_rtti_datum = temp_datum;
				//Assert::IsTrue(another_rtti_datum.get<RTTI*>()->Equals(reinterpret_cast<RTTI*>(&a)));
				Assert::IsTrue(another_rtti_datum.front<RTTI*>()->Equals(reinterpret_cast<RTTI*>(&a)));
			}
		}

		TEST_METHOD(DatumTestBack)
		{
			{
				Datum int_datum;
				auto expression = [&] { int_datum.back<int32_t>(); };
				Assert::ExpectException<std::exception>(expression);
				int32_t int_arr[] = { 10,20,30 };
				int_datum.setStorage(int_arr, 3);
				Assert::IsTrue(30 == int_datum.back<int32_t>());
			}
			{
				Datum float_datum;
				auto expression = [&] { float_datum.back<float_t>(); };
				Assert::ExpectException<std::exception>(expression);
				float_t float_arr[] = { 11.4f,20.1f,15.4f };
				float_datum.setStorage(float_arr, 3);
				Assert::AreEqual(15.4f, float_datum.back<float_t>());
			}
			{
				Datum vector_datum;
				auto expression = [&] { vector_datum.back<glm::vec4>(); };
				Assert::ExpectException<std::exception>(expression);
				glm::vec4 a(10);
				glm::vec4 b(20);
				glm::vec4 c(30);
				glm::vec4 vec_arr[] = { a,b,c };
				vector_datum.setStorage(vec_arr, 3);
				Assert::IsTrue(glm::vec4(30) == vector_datum.back<glm::vec4>());
			}
			{
				Datum mat_datum;
				auto expression = [&] { mat_datum.back<glm::mat4x4>(); };
				Assert::ExpectException<std::exception>(expression);
				glm::mat4x4 a(10);
				glm::mat4x4 b(20);
				glm::mat4x4 c(30);
				glm::mat4x4 mat_arr[] = { a,b,c };
				mat_datum.setStorage(mat_arr, 3);
				Assert::IsTrue(glm::mat4x4(30) == mat_datum.back<glm::mat4x4>());
			}
			{
				Datum string_datum;
				auto expression = [&] { string_datum.back<std::string>(); };
				Assert::ExpectException<std::exception>(expression);
				std::string str_arr[] = { "Pratik", "Paul", "John" };
				string_datum.setStorage(str_arr, 3);
				Assert::IsTrue("John"s == string_datum.back<std::string>());
			}
			{
				Datum rtti_datum;
				auto expression = [&] { rtti_datum.back<RTTI*>(); };
				Assert::ExpectException<std::exception>(expression);
				Foo a(10);
				Foo b(20);
				Foo c(30);
				Foo** foos = new Foo*[3];
				foos[0] = &a;
				foos[1] = &b;
				foos[2] = &c;
				rtti_datum.setStorage(reinterpret_cast<RTTI**>(foos), 3);
				Assert::IsTrue(rtti_datum.get<RTTI*>(2)->Equals(rtti_datum.back<RTTI*>()));
				delete[] foos;
			}
		}

		TEST_METHOD(DatumTestBackConst)
		{
			{
				Datum temp_datum;
				const Datum int_datum = temp_datum;
				auto expression = [&] { int_datum.back<int32_t>(); };
				Assert::ExpectException<std::exception>(expression);
				int32_t int_arr[] = { 10,20,30 };
				temp_datum.setStorage(int_arr, 3);
				const Datum another_int_datum = temp_datum;
				Assert::IsTrue(30 == another_int_datum.back<int32_t>());
			}
			{
				Datum temp_datum;
				const Datum float_datum = temp_datum;
				auto expression = [&] { float_datum.back<float_t>(); };
				Assert::ExpectException<std::exception>(expression);
				float_t float_arr[] = { 11.4f,20.1f,15.4f };
				temp_datum.setStorage(float_arr, 3);
				const Datum another_float_datum = temp_datum;
				Assert::AreEqual(15.4f, another_float_datum.back<float_t>());
			}
			{
				Datum temp_datum;
				const Datum vector_datum = temp_datum;
				auto expression = [&] { vector_datum.back<glm::vec4>(); };
				Assert::ExpectException<std::exception>(expression);
				glm::vec4 a(10);
				glm::vec4 b(20);
				glm::vec4 c(30);
				glm::vec4 vec_arr[] = { a,b,c };
				temp_datum.setStorage(vec_arr, 3);
				const Datum another_vector_datum = temp_datum;
				Assert::IsTrue(glm::vec4(30) == another_vector_datum.back<glm::vec4>());
			}
			{
				Datum temp_datum;
				const Datum mat_datum = temp_datum;
				auto expression = [&] { mat_datum.back<glm::mat4x4>(); };
				Assert::ExpectException<std::exception>(expression);
				glm::mat4x4 a(10);
				glm::mat4x4 b(20);
				glm::mat4x4 c(30);
				glm::mat4x4 mat_arr[] = { a,b,c };
				temp_datum.setStorage(mat_arr, 3);
				const Datum another_mat_datum = temp_datum;
				Assert::IsTrue(glm::mat4x4(30) == another_mat_datum.back<glm::mat4x4>());
			}
			{
				Datum temp_datum;
				const Datum string_datum = temp_datum;
				auto expression = [&] { string_datum.back<std::string>(); };
				Assert::ExpectException<std::exception>(expression);
				std::string str_arr[] = { "Pratik", "Paul", "John" };
				temp_datum.setStorage(str_arr, 3);
				const Datum another_string_datum = temp_datum;
				Assert::IsTrue("John"s == another_string_datum.back<std::string>());
			}
			{
				Datum temp_datum;
				const Datum rtti_datum = temp_datum;
				auto expression = [&] { rtti_datum.back<RTTI*>(); };
				Assert::ExpectException<std::exception>(expression);
				Foo a(10);
				Foo b(20);
				Foo c(30);
				Foo** foos = new Foo*[3];
				foos[0] = &a;
				foos[1] = &b;
				foos[2] = &c;
				temp_datum.setStorage(reinterpret_cast<RTTI**>(foos), 3);
				const Datum another_rtti_datum = temp_datum;
				Assert::IsTrue(another_rtti_datum.get<RTTI*>(2)->Equals(another_rtti_datum.back<RTTI*>()));
				delete[] foos;
			}
		}
#pragma endregion

#pragma region DatumRemoveTests
		
		TEST_METHOD(DatumTestRemove)
		{
			Datum datum;
			auto expression = [&] { datum.remove(3); };
			Assert::ExpectException<std::exception>(expression);
			datum = 10;
			int32_t int_arr[] = { 1,2,3 };
			datum.setStorage(int_arr, 3);
			auto expression_two = [&] { datum.remove(3); };
			Assert::ExpectException<std::exception>(expression_two);

			Datum int_datum;
			int_datum.setType(Datum::DatumType::INTEGER);
			int_datum.pushBack(1);
			int_datum.pushBack(2);
			int_datum.pushBack(3);
			int_datum.remove(2);
			Assert::IsTrue(1 == int_datum.get<int32_t>());
			Assert::IsTrue(3 == int_datum.get<int32_t>(1));
			Assert::AreEqual(2U, int_datum.size());
			Assert::AreEqual(3U, int_datum.capacity());

			Datum temp_float_datum;
			auto expression_three = [&] { temp_float_datum.remove(3.2f); };
			Assert::ExpectException<std::exception>(expression_three);
			temp_float_datum = 10.4f;
			float_t float_arr[] = { 10.4f,20.3f,31.1f };
			temp_float_datum.setStorage(float_arr, 3);
			auto expression_four = [&] { temp_float_datum.remove(10.4f); };
			Assert::ExpectException<std::exception>(expression_four);

			Datum float_datum;
			float_datum.setType(Datum::DatumType::FLOAT);
			float_datum.pushBack(10.2f);
			float_datum.pushBack(21.1f);
			float_datum.pushBack(39.4f);
			float_datum.remove(21.1f);
			Assert::IsTrue(10.2f == float_datum.get<float_t>());
			Assert::IsTrue(39.4f == float_datum.get<float_t>(1));
			Assert::AreEqual(2U, float_datum.size());
			Assert::AreEqual(3U, float_datum.capacity());

			Datum vector_datum;
			glm::vec4 a(10);
			glm::vec4 b(20);
			glm::vec4 c(30);
			Datum temp_vector_datum;
			auto expression_five = [&] { temp_vector_datum.remove(a); };
			Assert::ExpectException<std::exception>(expression_five);
			temp_vector_datum = a;
			glm::vec4 vec_arr[] = { a,b,c };
			temp_vector_datum.setStorage(vec_arr, 3);
			auto expression_six = [&] { temp_vector_datum.remove(a); };
			Assert::ExpectException<std::exception>(expression_six);
			
			vector_datum.setType(Datum::DatumType::VECTOR4);
			vector_datum.pushBack(a);
			vector_datum.pushBack(b);
			vector_datum.pushBack(c);
			vector_datum.remove(b);
			Assert::IsTrue(a == vector_datum.get<glm::vec4>());
			Assert::IsTrue(c == vector_datum.get<glm::vec4>(1));
			Assert::AreEqual(2U, vector_datum.size());
			Assert::AreEqual(3U, vector_datum.capacity());

			Datum mat_datum;
			glm::mat4x4 d(10);
			glm::mat4x4 e(20);
			glm::mat4x4 f(30);

			Datum temp_mat_datum;
			auto expression_seven = [&] { temp_mat_datum.remove(d); };
			Assert::ExpectException<std::exception>(expression_seven);
			temp_mat_datum = d;
			glm::mat4x4 mat_arr[] = { d,e,f };
			temp_mat_datum.setStorage(mat_arr, 3);
			auto expression_eight = [&] { temp_mat_datum.remove(d); };
			Assert::ExpectException<std::exception>(expression_eight);

			mat_datum.setType(Datum::DatumType::MATRIX4X4);
			mat_datum.pushBack(d);
			mat_datum.pushBack(e);
			mat_datum.pushBack(f);
			mat_datum.remove(e);
			Assert::IsTrue(d == mat_datum.get<glm::mat4x4>());
			Assert::IsTrue(f == mat_datum.get<glm::mat4x4>(1));
			Assert::AreEqual(2U, mat_datum.size());
			Assert::AreEqual(3U, mat_datum.capacity());

			Datum temp_string_datum;
			auto expression_nine = [&] { temp_string_datum.remove("Pratik"); };
			Assert::ExpectException<std::exception>(expression_nine);
			temp_string_datum = "Pratik"s;
			std::string string_arr[] = { "Pratik"s,"Paul"s,"Ron"s };
			temp_string_datum.setStorage(string_arr, 3);
			auto expression_ten = [&] { temp_string_datum.remove("Pratik"s); };
			Assert::ExpectException<std::exception>(expression_ten);

			Datum string_datum;
			string_datum.setType(Datum::DatumType::STRING);
			string_datum.pushBack("Pratik"s);
			string_datum.pushBack("Paul"s);
			string_datum.pushBack("Tom"s);
			string_datum.remove("Paul"s);
			Assert::IsTrue("Pratik"s == string_datum.get<std::string>());
			Assert::IsTrue("Tom"s == string_datum.get<std::string>(1));
			Assert::AreEqual(2U, string_datum.size());
			Assert::AreEqual(3U, string_datum.capacity());

			Datum rtti_datum;
			rtti_datum.setType(Datum::DatumType::POINTER);
			Foo x(10);
			Foo y(20);
			Foo z(30);

			Datum temp_rtti_datum;
			auto expression_eleven = [&] { temp_rtti_datum.remove(x); };
			Assert::ExpectException<std::exception>(expression_eleven);
			temp_rtti_datum = &x;
			RTTI* rtti_arr[] = { &x,&y,&z };
			temp_rtti_datum.setStorage(rtti_arr, 3);
			auto expression_twelve = [&] { temp_rtti_datum.remove(x); };
			Assert::ExpectException<std::exception>(expression_twelve);

			rtti_datum.pushBack(&x);
			rtti_datum.pushBack(&y);
			rtti_datum.pushBack(&z);
			rtti_datum.remove(y);
			Assert::IsTrue(&x == rtti_datum.get<RTTI*>());
			Assert::IsTrue(&z == rtti_datum.get<RTTI*>(1));
			Assert::AreEqual(2U, rtti_datum.size());
			Assert::AreEqual(3U, rtti_datum.capacity());
		}

		TEST_METHOD(DatumTestRemoveAt)
		{
			Datum datum;
			auto expression = [&] { datum.removeAt(0); };
			Assert::ExpectException<std::exception>(expression);
			datum = 10;
			int32_t int_arr[] = { 1,2,3 };
			datum.setStorage(int_arr, 3);
			auto expression_two = [&] { datum.removeAt(1); };
			Assert::ExpectException<std::exception>(expression_two);

			Datum int_datum;
			int_datum.setType(Datum::DatumType::INTEGER);
			int_datum.pushBack(1);
			int_datum.pushBack(2);
			int_datum.pushBack(3);
			int_datum.removeAt(1);
			Assert::IsTrue(1 == int_datum.get<int32_t>());
			Assert::IsTrue(3 == int_datum.get<int32_t>(1));
			Assert::AreEqual(2U, int_datum.size());
			Assert::AreEqual(3U, int_datum.capacity());

			Datum float_datum;
			float_datum.setType(Datum::DatumType::FLOAT);
			float_datum.pushBack(10.2f);
			float_datum.pushBack(21.1f);
			float_datum.pushBack(39.4f);
			float_datum.removeAt(1);
			Assert::IsTrue(10.2f == float_datum.get<float_t>());
			Assert::IsTrue(39.4f == float_datum.get<float_t>(1));
			Assert::AreEqual(2U, float_datum.size());
			Assert::AreEqual(3U, float_datum.capacity());

			Datum vector_datum;
			glm::vec4 a(10);
			glm::vec4 b(20);
			glm::vec4 c(30);
			vector_datum.setType(Datum::DatumType::VECTOR4);
			vector_datum.pushBack(a);
			vector_datum.pushBack(b);
			vector_datum.pushBack(c);
			vector_datum.removeAt(1);
			Assert::IsTrue(a == vector_datum.get<glm::vec4>());
			Assert::IsTrue(c == vector_datum.get<glm::vec4>(1));
			Assert::AreEqual(2U, vector_datum.size());
			Assert::AreEqual(3U, vector_datum.capacity());

			Datum mat_datum;
			glm::mat4x4 d(10);
			glm::mat4x4 e(20);
			glm::mat4x4 f(30);
			mat_datum.setType(Datum::DatumType::MATRIX4X4);
			mat_datum.pushBack(d);
			mat_datum.pushBack(e);
			mat_datum.pushBack(f);
			mat_datum.removeAt(1);
			Assert::IsTrue(d == mat_datum.get<glm::mat4x4>());
			Assert::IsTrue(f == mat_datum.get<glm::mat4x4>(1));
			Assert::AreEqual(2U, mat_datum.size());
			Assert::AreEqual(3U, mat_datum.capacity());

			Datum string_datum;
			string_datum.setType(Datum::DatumType::STRING);
			string_datum.pushBack("Pratik"s);
			string_datum.pushBack("Paul"s);
			string_datum.pushBack("Tom"s);
			string_datum.removeAt(1);
			Assert::IsTrue("Pratik"s == string_datum.get<std::string>());
			Assert::IsTrue("Tom"s == string_datum.get<std::string>(1));
			Assert::AreEqual(2U, string_datum.size());
			Assert::AreEqual(3U, string_datum.capacity());

			Datum rtti_datum;
			rtti_datum.setType(Datum::DatumType::POINTER);
			Foo x(10);
			Foo y(20);
			Foo z(30);
			rtti_datum.pushBack(&x);
			rtti_datum.pushBack(&y);
			rtti_datum.pushBack(&z);
			rtti_datum.removeAt(1);
			Assert::IsTrue(&x == rtti_datum.get<RTTI*>());
			Assert::IsTrue(&z == rtti_datum.get<RTTI*>(1));
			Assert::AreEqual(2U, rtti_datum.size());
			Assert::AreEqual(3U, rtti_datum.capacity());
		}

#pragma endregion

#pragma region DatumFindTests

		TEST_METHOD(DatumTestFind)
		{
			{
				Datum int_datum;
				Datum temp_datum;
				auto expression_one = [&] { int_datum.find(10); };
				Assert::ExpectException<std::exception>(expression_one);
				temp_datum.setType(Datum::DatumType::POINTER);
				auto expression_two = [&] { temp_datum.find(15); };
				Assert::ExpectException<std::exception>(expression_two);
				int_datum.setType(Datum::DatumType::INTEGER);
				int32_t int_arr[] = { 1,2,3 };
				int_datum.setStorage(int_arr, 3);
				auto[result, index] = int_datum.find(1);
				Assert::IsTrue(0 == index);
				auto[result2, index2] = int_datum.find(2);
				Assert::IsTrue(1 == index2);
				auto[result3, index3] = int_datum.find(3);
				Assert::IsTrue(2 == index3);
				auto[result4, index4] = int_datum.find(5);
				Assert::IsTrue(3 == index4);
			}
			{
				Datum float_datum;
				Datum temp_datum;
				auto expression_one = [&] { float_datum.find(10.2f); };
				Assert::ExpectException<std::exception>(expression_one);
				temp_datum.setType(Datum::DatumType::POINTER);
				auto expression_two = [&] { temp_datum.find(15.5f); };
				Assert::ExpectException<std::exception>(expression_two);
				float_datum.setType(Datum::DatumType::FLOAT);
				float_t float_arr[] = { 10.2f,15.5f,31.1f };
				float_datum.setStorage(float_arr, 3);
				auto[result, index] = float_datum.find(10.2f);
				Assert::IsTrue(0 == index);
				auto[result2, index2] = float_datum.find(15.5f);
				Assert::IsTrue(1 == index2);
				auto[result3, index3] = float_datum.find(31.1f);
				Assert::IsTrue(2 == index3);
				auto[result4, index4] = float_datum.find(103.2f);
				Assert::IsTrue(3 == index4);
			}
			{
				Datum vector_datum;
				Datum temp_datum;
				glm::vec4 a(10);
				glm::vec4 b(20);
				glm::vec4 c(30);
				glm::vec4 d(40);
				auto expression_one = [&] { vector_datum.find(a); };
				Assert::ExpectException<std::exception>(expression_one);
				temp_datum.setType(Datum::DatumType::POINTER);
				auto expression_two = [&] { temp_datum.find(b); };
				Assert::ExpectException<std::exception>(expression_two);
				vector_datum.setType(Datum::DatumType::VECTOR4);
				glm::vec4 vector_arr[] = { a,b,c };
				vector_datum.setStorage(vector_arr, 3);
				auto[result, index] = vector_datum.find(a);
				Assert::IsTrue(0 == index);
				auto[result2, index2] = vector_datum.find(b);
				Assert::IsTrue(1 == index2);
				auto[result3, index3] = vector_datum.find(c);
				Assert::IsTrue(2 == index3);
				auto[result4, index4] = vector_datum.find(d);
				Assert::IsTrue(3 == index4);
			}
			{
				Datum mat_datum;
				Datum temp_datum;
				glm::mat4x4 a(10);
				glm::mat4x4 b(20);
				glm::mat4x4 c(30);
				glm::mat4x4 d(40);
				auto expression_one = [&] { mat_datum.find(a); };
				Assert::ExpectException<std::exception>(expression_one);
				temp_datum.setType(Datum::DatumType::POINTER);
				auto expression_two = [&] { temp_datum.find(b); };
				Assert::ExpectException<std::exception>(expression_two);
				mat_datum.setType(Datum::DatumType::MATRIX4X4);
				glm::mat4x4 mat_arr[] = { a,b,c };
				mat_datum.setStorage(mat_arr, 3);
				auto[result, index] = mat_datum.find(a);
				Assert::IsTrue(0 == index);
				auto[result2, index2] = mat_datum.find(b);
				Assert::IsTrue(1 == index2);
				auto[result3, index3] = mat_datum.find(c);
				Assert::IsTrue(2 == index3);
				auto[result4, index4] = mat_datum.find(d);
				Assert::IsTrue(3 == index4);
			}
			{
				Datum string_datum;
				Datum temp_datum;
				auto expression_one = [&] { string_datum.find("Pratik"s); };
				Assert::ExpectException<std::exception>(expression_one);
				temp_datum.setType(Datum::DatumType::POINTER);
				auto expression_two = [&] { temp_datum.find("Ron"s); };
				Assert::ExpectException<std::exception>(expression_two);
				string_datum.setType(Datum::DatumType::STRING);
				std::string string_arr[] = { "Pratik"s, "Paul"s, "Ron"s };
				string_datum.setStorage(string_arr, 3);
				auto[result, index] = string_datum.find("Pratik"s);
				Assert::IsTrue(0 == index);
				auto[result2, index2] = string_datum.find("Paul"s);
				Assert::IsTrue(1 == index2);
				auto[result3, index3] = string_datum.find("Ron"s);
				Assert::IsTrue(2 == index3);
				auto[result4, index4] = string_datum.find("Rick"s);
				Assert::IsTrue(3 == index4);
			}
			{
				Datum rtti_datum;
				Datum temp_datum;
				Foo a(10);
				Foo b(20);
				Foo c(30);
				Foo d(40);
				auto expression_one = [&] { rtti_datum.find(a); };
				Assert::ExpectException<std::exception>(expression_one);
				temp_datum.setType(Datum::DatumType::STRING);
				auto expression_two = [&] { temp_datum.find(b); };
				Assert::ExpectException<std::exception>(expression_two);
				rtti_datum.setType(Datum::DatumType::POINTER);
				Foo** foos = new Foo*[3];
				foos[0] = &a;
				foos[1] = &b;
				foos[2] = &c;
				rtti_datum.setStorage(reinterpret_cast<RTTI**>(foos), 3);
				auto[result, index] = rtti_datum.find(a);
				Assert::IsTrue(0 == index);
				auto[result2, index2] = rtti_datum.find(b);
				Assert::IsTrue(1 == index2);
				auto[result3, index3] = rtti_datum.find(c);
				Assert::IsTrue(2 == index3);
				auto[result4, index4] = rtti_datum.find(d);
				Assert::IsTrue(3 == index4);
				delete[] foos;
			}
		}

		TEST_METHOD(DatumTestFindConst)
		{
			{
				Datum datum_one;
				const Datum int_datum = datum_one;
				Datum datum_two;
				const Datum temp_datum = datum_two;
				auto expression_one = [&] { int_datum.find(10); };
				Assert::ExpectException<std::exception>(expression_one);
				datum_two.setType(Datum::DatumType::POINTER);
				const Datum another_temp_datum = datum_two;
				auto expression_two = [&] { another_temp_datum.find(15); };
				Assert::ExpectException<std::exception>(expression_two);
				datum_one.setType(Datum::DatumType::INTEGER);
				int32_t int_arr[] = { 1,2,3 };
				datum_one.setStorage(int_arr, 3);
				const Datum another_int_datum = datum_one;
				auto[result, index] = another_int_datum.find(1);
				Assert::IsTrue(0 == index);
				auto[result2, index2] = another_int_datum.find(2);
				Assert::IsTrue(1 == index2);
				auto[result3, index3] = another_int_datum.find(3);
				Assert::IsTrue(2 == index3);
				auto[result4, index4] = another_int_datum.find(10);
				Assert::IsTrue(3 == index4);
			}
			{
				Datum datum_one;
				const Datum float_datum = datum_one;
				Datum datum_two;
				const Datum temp_datum = datum_two;
				auto expression_one = [&] { float_datum.find(10.2f); };
				Assert::ExpectException<std::exception>(expression_one);
				datum_two.setType(Datum::DatumType::POINTER);
				const Datum another_temp_datum = datum_two;
				auto expression_two = [&] { another_temp_datum.find(15.5f); };
				Assert::ExpectException<std::exception>(expression_two);
				datum_one.setType(Datum::DatumType::FLOAT);
				float_t float_arr[] = { 10.2f,15.5f,31.1f };
				datum_one.setStorage(float_arr, 3);
				const Datum another_float_datum = datum_one;
				auto[result, index] = another_float_datum.find(10.2f);
				Assert::IsTrue(0 == index);
				auto[result2, index2] = another_float_datum.find(15.5f);
				Assert::IsTrue(1 == index2);
				auto[result3, index3] = another_float_datum.find(31.1f);
				Assert::IsTrue(2 == index3);
				auto[result4, index4] = another_float_datum.find(104.4f);
				Assert::IsTrue(3 == index4);
			}
			{
				Datum datum_one;
				const Datum vector_datum = datum_one;
				Datum datum_two;
				const Datum temp_datum = datum_two;
				glm::vec4 a(10);
				glm::vec4 b(20);
				glm::vec4 c(30);
				glm::vec4 d(40);
				auto expression_one = [&] { vector_datum.find(a); };
				Assert::ExpectException<std::exception>(expression_one);
				datum_two.setType(Datum::DatumType::POINTER);
				const Datum another_temp_datum = datum_two;
				auto expression_two = [&] { another_temp_datum.find(b); };
				Assert::ExpectException<std::exception>(expression_two);
				datum_one.setType(Datum::DatumType::VECTOR4);
				glm::vec4 vector_arr[] = { a,b,c };
				datum_one.setStorage(vector_arr, 3);
				const Datum another_vector_datum = datum_one;
				auto[result, index] = another_vector_datum.find(a);
				Assert::IsTrue(0 == index);
				auto[result2, index2] = another_vector_datum.find(b);
				Assert::IsTrue(1 == index2);
				auto[result3, index3] = another_vector_datum.find(c);
				Assert::IsTrue(2 == index3);
				auto[result4, index4] = another_vector_datum.find(d);
				Assert::IsTrue(3 == index4);
			}
			{
				Datum datum_one;
				const Datum mat_datum = datum_one;
				Datum datum_two;
				const Datum temp_datum = datum_two;
				glm::mat4x4 a(10);
				glm::mat4x4 b(20);
				glm::mat4x4 c(30);
				glm::mat4x4 d(40);
				auto expression_one = [&] { mat_datum.find(a); };
				Assert::ExpectException<std::exception>(expression_one);
				datum_two.setType(Datum::DatumType::POINTER);
				const Datum another_temp_datum = datum_two;
				auto expression_two = [&] { another_temp_datum.find(b); };
				Assert::ExpectException<std::exception>(expression_two);
				datum_one.setType(Datum::DatumType::MATRIX4X4);
				glm::mat4x4 mat_arr[] = { a,b,c };
				datum_one.setStorage(mat_arr, 3);
				const Datum another_mat_datum = datum_one;
				auto[result, index] = another_mat_datum.find(a);
				Assert::IsTrue(0 == index);
				auto[result2, index2] = another_mat_datum.find(b);
				Assert::IsTrue(1 == index2);
				auto[result3, index3] = another_mat_datum.find(c);
				Assert::IsTrue(2 == index3);
				auto[result4, index4] = another_mat_datum.find(d);
				Assert::IsTrue(3 == index4);
			}
			{
				Datum datum_one;
				const Datum string_datum = datum_one;
				Datum datum_two;
				const Datum temp_datum = datum_two;
				auto expression_one = [&] { string_datum.find("Pratik"s); };
				Assert::ExpectException<std::exception>(expression_one);
				datum_two.setType(Datum::DatumType::POINTER);
				const Datum another_temp_datum = datum_two;
				auto expression_two = [&] { another_temp_datum.find("Ron"s); };
				Assert::ExpectException<std::exception>(expression_two);
				datum_one.setType(Datum::DatumType::STRING);
				std::string string_arr[] = { "Pratik"s, "Paul"s, "Ron"s };
				datum_one.setStorage(string_arr, 3);
				const Datum another_string_datum = datum_one;
				auto[result, index] = another_string_datum.find("Pratik"s);
				Assert::IsTrue(0 == index);
				auto[result2, index2] = another_string_datum.find("Paul"s);
				Assert::IsTrue(1 == index2);
				auto[result3, index3] = another_string_datum.find("Ron"s);
				Assert::IsTrue(2 == index3);
				auto[result4, index4] = another_string_datum.find("Rick"s);
				Assert::IsTrue(3 == index4);
			}
			{
				Datum datum_one;
				const Datum rtti_datum = datum_one;
				Datum datum_two;
				const Datum temp_datum = datum_two;
				Foo a(10);
				Foo b(20);
				Foo c(30);
				Foo d(40);
				auto expression_one = [&] { rtti_datum.find(a); };
				Assert::ExpectException<std::exception>(expression_one);
				datum_two.setType(Datum::DatumType::STRING);
				const Datum another_temp_datum = datum_two;
				auto expression_two = [&] { another_temp_datum.find(b); };
				Assert::ExpectException<std::exception>(expression_two);
				datum_one.setType(Datum::DatumType::POINTER);
				Foo** foos = new Foo*[3];
				foos[0] = &a;
				foos[1] = &b;
				foos[2] = &c;
				datum_one.setStorage(reinterpret_cast<RTTI**>(foos), 3);
				const Datum another_rtti_datum = datum_one;
				auto[result, index] = another_rtti_datum.find(a);
				Assert::IsTrue(0 == index);
				auto[result2, index2] = another_rtti_datum.find(b);
				Assert::IsTrue(1 == index2);
				auto[result3, index3] = another_rtti_datum.find(c);
				Assert::IsTrue(2 == index3);
				auto[result4, index4] = another_rtti_datum.find(d);
				Assert::IsTrue(3 == index4);
				delete[] foos;
			}
		}
#pragma endregion
	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState DatumTest::s_start_mem_state;
}