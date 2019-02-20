#include "pch.h"
#include "AttributedFoo.h"

using namespace std::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(AttributedFoo)

	AttributedFoo::AttributedFoo() :
		Attributed(AttributedFoo::TypeIdClass())
	{
	}

	const Vector<Signature> AttributedFoo::Signatures()
	{
		return Vector<Signature>
		{
			{ "ExternalInteger", Datum::DatumType::INTEGER, 1, offsetof(AttributedFoo,external_integer) },
			{ "ExternalFloat", Datum::DatumType::FLOAT, 1, offsetof(AttributedFoo,external_float) },
			{ "ExternalString", Datum::DatumType::STRING, 1, offsetof(AttributedFoo,external_string) },
			{ "ExternalVector", Datum::DatumType::VECTOR4, 1, offsetof(AttributedFoo,external_glm_vector) },
			{ "ExternalMatrix", Datum::DatumType::MATRIX4X4, 1, offsetof(AttributedFoo,external_glm_matrix) },
			{ "ExternalIntegerArray", Datum::DatumType::INTEGER, ARRAY_SIZE, offsetof(AttributedFoo,external_integer_array) },
			{ "ExternalFloatArray", Datum::DatumType::FLOAT, ARRAY_SIZE, offsetof(AttributedFoo,external_float_array) },
			{ "ExternalStringArray", Datum::DatumType::STRING, ARRAY_SIZE, offsetof(AttributedFoo,external_string_array) },
			{ "ExternalVectorArray", Datum::DatumType::VECTOR4, ARRAY_SIZE, offsetof(AttributedFoo,external_glm_vector_array) },
			{ "ExternalMatrixArray", Datum::DatumType::MATRIX4X4, ARRAY_SIZE, offsetof(AttributedFoo,external_glm_matrix_array) },
			{ "NestedScope", Datum::DatumType::TABLE, 0, 0 }
		};
	}

	bool AttributedFoo::Equals(const RTTI* t_rhs) const
	{
		return false;
	}
	
	std::string AttributedFoo::ToString() const
	{
		return "AttributedFoo"s;
	}

}
