#include "pch.h"
#include <string>
#include <utility>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<RTTI>(RTTI* t_temp)
	{
		RETURN_WIDE_STRING(t_temp);
	}

	template<>
	std::wstring ToString<AttributedFoo>(AttributedFoo* t_temp)
	{
		RETURN_WIDE_STRING(t_temp);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(AttributedTest)
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
			AttributedFoo a;
			RTTI* rtti = &a;
			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsTrue(rtti->Is("AttributedFoo"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
			Assert::IsTrue(rtti->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(AttributedFoo::TypeIdClass(), rtti->TypeIdInstance());

			Bar* b = rtti->As<Bar>();
			Assert::IsNull(b);

			AttributedFoo* f = rtti->As<AttributedFoo>();
			Assert::IsNotNull(f);
			Assert::AreEqual(&a, f);
			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNotNull(r);

			/*AttributedFoo other_foo;
			Assert::IsTrue(rtti->Equals(&other_foo));*/

			Bar otherBar(10);
			Assert::IsFalse(rtti->Equals(&otherBar));

			Assert::AreEqual("AttributedFoo"s, r->ToString());
		}

		TEST_METHOD(Attributes)
		{
			AttributedFoo foo;

			Assert::IsTrue(foo.isAttribute("this"));
			Assert::IsTrue(foo.isAttribute("ExternalInteger"));
			Assert::IsTrue(foo.isAttribute("ExternalFloat"));
			Assert::IsTrue(foo.isAttribute("ExternalString"));
			Assert::IsTrue(foo.isAttribute("ExternalVector"));
			Assert::IsTrue(foo.isAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.isAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.isAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.isAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.isAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.isAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.isAttribute("NestedScope"));
			Assert::IsTrue(foo.isAttribute("NestedScopeArray"));

			Assert::IsTrue(foo.isPrescribedAttribute("this"));
			Assert::IsTrue(foo.isPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foo.isPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foo.isPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foo.isPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foo.isPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.isPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.isPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.isPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.isPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.isPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.isPrescribedAttribute("NestedScope"));
			Assert::IsTrue(foo.isPrescribedAttribute("NestedScopeArray"));

			Assert::IsFalse(foo.isAuxillaryAttribute("this"));
			Assert::IsFalse(foo.isAuxillaryAttribute("ExternalInteger"));
			Assert::IsFalse(foo.isAuxillaryAttribute("ExternalFloat"));
			Assert::IsFalse(foo.isAuxillaryAttribute("ExternalString"));
			Assert::IsFalse(foo.isAuxillaryAttribute("ExternalVector"));
			Assert::IsFalse(foo.isAuxillaryAttribute("ExternalMatrix"));
			Assert::IsFalse(foo.isAuxillaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(foo.isAuxillaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(foo.isAuxillaryAttribute("ExternalStringArray"));
			Assert::IsFalse(foo.isAuxillaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(foo.isAuxillaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(foo.isAuxillaryAttribute("NestedScope"));
			Assert::IsFalse(foo.isAuxillaryAttribute("NestedScopeArray"));

			Assert::AreEqual(static_cast<RTTI*>(&foo), foo["this"].get<RTTI*>());
		}

		TEST_METHOD(AppendAuxillaryAttribute)
		{
			AttributedFoo foo;
			foo.appendAuxillaryAttribute("AuxillaryInteger");
			Assert::IsTrue(foo.isAttribute("AuxillaryInteger"));
			Assert::IsTrue(foo.isAuxillaryAttribute("AuxillaryInteger"));
			Assert::IsFalse(foo.isPrescribedAttribute("AuxillaryInteger"));

			Datum d1;
			Datum d2 = foo.appendAuxillaryAttribute("AuxillaryIntegerWithDatum", d1);
			Assert::IsTrue(foo.isAttribute("AuxillaryIntegerWithDatum"));
			Assert::IsTrue(foo.isAuxillaryAttribute("AuxillaryIntegerWithDatum"));
			Assert::IsFalse(foo.isPrescribedAttribute("AuxillaryIntegerWithDatum"));
			Assert::IsTrue(d1 == d2);

			Datum d3;
			std::pair<std::string, Datum> pair = std::make_pair("AuxillaryIntegerWithPair"s, d3);
			Datum d4 = foo.appendAuxillaryAttribute(pair);
			Assert::IsTrue(foo.isAttribute("AuxillaryIntegerWithPair"));
			Assert::IsTrue(foo.isAuxillaryAttribute("AuxillaryIntegerWithPair"));
			Assert::IsFalse(foo.isPrescribedAttribute("AuxillaryIntegerWithPair"));
			Assert::IsTrue(d3 == d4);

			Assert::ExpectException<std::exception>([&foo] {foo.appendAuxillaryAttribute("ExternalInteger"); });
			Assert::ExpectException<std::exception>([&foo, &d1] {foo.appendAuxillaryAttribute("ExternalInteger", d1); });
			Assert::ExpectException<std::exception>([&foo, &d1] {foo.appendAuxillaryAttribute(std::make_pair("ExternalInteger", d1)); });
		}

		TEST_METHOD(ExternalAssignment)
		{
			AttributedFoo foo;

			Datum* datum = foo.find("ExternalInteger");
			Assert::AreEqual(1U, datum->size());
			Assert::AreEqual(datum->get<int32_t>(), static_cast<int32_t>(foo.external_integer));

			int32_t a = 5;
			datum->set(5);
			Assert::AreEqual(a, datum->get<int32_t>());
			Assert::AreEqual(a, static_cast<int32_t>(foo.external_integer));

			float_t b = 6.4f;
			foo["ExternalFloat"] = b;
			Assert::AreEqual(b, foo.external_float);
		}

		TEST_METHOD(CopyConstructor)
		{
			AttributedFoo foo;
			AttributedFoo another_foo = foo;

			Assert::IsTrue(another_foo.isAttribute("this"));
			Assert::IsTrue(another_foo.isAttribute("ExternalInteger"));
			Assert::IsTrue(another_foo.isAttribute("ExternalFloat"));
			Assert::IsTrue(another_foo.isAttribute("ExternalString"));
			Assert::IsTrue(another_foo.isAttribute("ExternalVector"));
			Assert::IsTrue(another_foo.isAttribute("ExternalMatrix"));
			Assert::IsTrue(another_foo.isAttribute("ExternalIntegerArray"));
			Assert::IsTrue(another_foo.isAttribute("ExternalFloatArray"));
			Assert::IsTrue(another_foo.isAttribute("ExternalStringArray"));
			Assert::IsTrue(another_foo.isAttribute("ExternalVectorArray"));
			Assert::IsTrue(another_foo.isAttribute("ExternalMatrixArray"));
			Assert::IsTrue(another_foo.isAttribute("NestedScope"));
			Assert::IsTrue(another_foo.isAttribute("NestedScopeArray"));

			Assert::IsTrue(another_foo.isPrescribedAttribute("this"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalString"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("NestedScope"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("NestedScopeArray"));

			Assert::IsFalse(another_foo.isAuxillaryAttribute("this"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalInteger"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalFloat"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalString"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalVector"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalMatrix"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalStringArray"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("NestedScope"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("NestedScopeArray"));

			int32_t& foo_data = foo["ExternalInteger"].get<int32_t>();
			int32_t& another_foo_data = another_foo["ExternalInteger"].get<int32_t>();
			Assert::AreNotEqual(&another_foo_data, &foo_data);
		}

		TEST_METHOD(CopyAssignmentOperator)
		{
			AttributedFoo foo;
			AttributedFoo another_foo;
			another_foo = foo;

			Assert::IsTrue(another_foo.isAttribute("this"));
			Assert::IsTrue(another_foo.isAttribute("ExternalInteger"));
			Assert::IsTrue(another_foo.isAttribute("ExternalFloat"));
			Assert::IsTrue(another_foo.isAttribute("ExternalString"));
			Assert::IsTrue(another_foo.isAttribute("ExternalVector"));
			Assert::IsTrue(another_foo.isAttribute("ExternalMatrix"));
			Assert::IsTrue(another_foo.isAttribute("ExternalIntegerArray"));
			Assert::IsTrue(another_foo.isAttribute("ExternalFloatArray"));
			Assert::IsTrue(another_foo.isAttribute("ExternalStringArray"));
			Assert::IsTrue(another_foo.isAttribute("ExternalVectorArray"));
			Assert::IsTrue(another_foo.isAttribute("ExternalMatrixArray"));
			Assert::IsTrue(another_foo.isAttribute("NestedScope"));
			Assert::IsTrue(another_foo.isAttribute("NestedScopeArray"));

			Assert::IsTrue(another_foo.isPrescribedAttribute("this"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalString"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("NestedScope"));
			Assert::IsTrue(another_foo.isPrescribedAttribute("NestedScopeArray"));

			Assert::IsFalse(another_foo.isAuxillaryAttribute("this"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalInteger"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalFloat"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalString"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalVector"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalMatrix"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalStringArray"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("NestedScope"));
			Assert::IsFalse(another_foo.isAuxillaryAttribute("NestedScopeArray"));

			Assert::AreEqual(&another_foo, another_foo["this"].get<RTTI*>()->As<AttributedFoo>());
		}

		TEST_METHOD(MoveConstructor)
		{
			AttributedFoo another_foo;

			Assert::IsTrue(another_foo.isAttribute("this"));
			another_foo["ExternalInteger"] = 2;
			another_foo["ExternalFloat"] = 3.3f;
			AttributedFoo foo(std::move(another_foo));

			Assert::IsTrue(foo.isAttribute("this"));
			Assert::AreEqual(2U, foo.external_integer);
			Assert::AreEqual(3.3f, foo.external_float);

			Assert::AreEqual(&foo, foo["this"].get<RTTI*>()->As<AttributedFoo>());

		}

		TEST_METHOD(MoveAssignmentOperator)
		{
			AttributedFoo foo;
			Assert::IsTrue(foo.isAttribute("this"));
			Assert::AreEqual(0U, foo.external_integer);
			Assert::AreEqual(0.0f, foo.external_float);
		
			AttributedFoo another_foo;
			another_foo["ExternalInteger"] = 2;
			another_foo["ExternalFloat"] = 3.3f;
			foo = std::move(another_foo);

			Assert::IsTrue(foo.isAttribute("this"));
			Assert::AreEqual(2U, foo.external_integer);
			Assert::AreEqual(3.3f, foo.external_float);
		}

		TEST_METHOD(Clear)
		{
			AttributedFoo foo;
			size_t size = foo.size();
			foo.appendAuxillaryAttribute("AuxillaryInteger");
			Assert::AreEqual(size + 1, foo.size());
			foo.clear();
			Assert::AreEqual(0, static_cast<int>(foo.size()));
		}

		TEST_METHOD(AccessorMethods)
		{
			AttributedFoo foo;
			foo["ExternalInteger"] = 2;
			foo["ExternalFloat"] = 55.3f;
			Vector<std::pair<std::string, Datum>*> attributes = foo.getAttributes();
			Assert::AreEqual(13U, attributes.size());
			Assert::IsTrue(attributes.front()->first == "this");
			Datum d1;
			d1 = 10;
			foo.appendAuxillaryAttribute("MyInteger", d1);
			Datum d2;
			d2 = 25.5f;
			foo.appendAuxillaryAttribute("MyFloat", d2);
			Vector<std::pair<std::string, Datum>*> prescribed_attributes = foo.getPrescribedAttributes();
			Assert::AreEqual(12U, prescribed_attributes.size());

			Assert::AreEqual("ExternalInteger"s, prescribed_attributes[0]->first);
			Assert::AreEqual("ExternalFloat"s, prescribed_attributes[1]->first);
			Assert::AreEqual("ExternalString"s, prescribed_attributes[2]->first);
			Assert::AreEqual("ExternalVector"s, prescribed_attributes[3]->first);
			Assert::AreEqual("ExternalMatrix"s, prescribed_attributes[4]->first);
			Assert::AreEqual("ExternalIntegerArray"s, prescribed_attributes[5]->first);
			Assert::AreEqual("ExternalFloatArray"s, prescribed_attributes[6]->first);
			Assert::AreEqual("ExternalStringArray"s, prescribed_attributes[7]->first);
			Assert::AreEqual("ExternalVectorArray"s, prescribed_attributes[8]->first);
			Assert::AreEqual("ExternalMatrixArray"s, prescribed_attributes[9]->first);
			Assert::AreEqual("NestedScope"s, prescribed_attributes[10]->first);
			Assert::AreEqual("NestedScopeArray"s, prescribed_attributes[11]->first);

			Vector<std::pair<std::string, Datum>*> auxillary_attributes = foo.getAuxillaryAttributes();
			Assert::AreEqual(2U, auxillary_attributes.size());
			Assert::AreEqual("MyInteger"s, auxillary_attributes[0]->first);
			Assert::AreEqual("MyFloat"s, auxillary_attributes[1]->first);
		}

	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState AttributedTest::s_start_mem_state;
}