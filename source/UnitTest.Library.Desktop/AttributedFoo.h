#pragma once
#include "Attributed.h"
#include "TypeManager.h"


namespace FieaGameEngine
{
	class AttributedFoo final : public Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, RTTI);
		static const uint32_t ARRAY_SIZE = 5;
	public:
		AttributedFoo();
		AttributedFoo(const AttributedFoo& t_rhs) = default;
		AttributedFoo(AttributedFoo&& t_rhs) = default;
		AttributedFoo& operator=(const AttributedFoo& t_rhs) = default;
		AttributedFoo& operator=(AttributedFoo&& t_rhs) = default;
		~AttributedFoo() = default;

		uint32_t external_integer = 0;
		float_t external_float = 0.0f;
		std::string external_string;
		glm::vec4 external_glm_vector;
		glm::mat4x4 external_glm_matrix;

		uint32_t external_integer_array[ARRAY_SIZE];
		float_t external_float_array[ARRAY_SIZE];
		std::string external_string_array[ARRAY_SIZE];
		glm::vec4 external_glm_vector_array[ARRAY_SIZE];
		glm::mat4x4 external_glm_matrix_array[ARRAY_SIZE];

		bool Equals(const RTTI* t_rhs) const override;
		std::string ToString() const override;
		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	};
}

