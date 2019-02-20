#pragma once
#include <glm/fwd.hpp>
#include "RTTI.h"
#include "Datum.h"
#include "Scope.h"


namespace FieaGameEngine
{
	class Signature final
	{
	public:
		std::string sig_name;
		Datum::DatumType sig_type;
		uint32_t sig_size;
		size_t sig_offset;
		Signature(const std::string& t_name, const Datum::DatumType& t_type, const uint32_t& t_size, const size_t& t_offset);
	};

	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);

	public:
		Attributed();
		Attributed(const Attributed& t_rhs);
		Attributed(Attributed&& t_rhs);
		Attributed& operator=(const Attributed& t_rhs);
		Attributed& operator=(Attributed&& t_rhs);
		void clear() override;
		virtual ~Attributed() = default;
		bool isAttribute(const std::string& t_name);
		bool isPrescribedAttribute(const std::string& t_name);
		bool isAuxillaryAttribute(const std::string& t_name);
		Datum& appendAuxillaryAttribute(const std::string& t_name);

	protected:
		explicit Attributed(uint32_t t_type_id);
		void populate(uint32_t t_type_id);
		const HashMap<std::string, Datum*> getPrescribedAttributes() const;
		const HashMap<std::string, Datum*> getAuxillaryAttributes() const;

		Datum& addInternalAttribute(const std::string& t_name, const int32_t& t_value, const uint32_t& t_size);
		Datum& addInternalAttribute(const std::string& t_name, const float_t& t_value, const uint32_t& t_size);
		Datum& addInternalAttribute(const std::string& t_name, const std::string& t_value, const uint32_t& t_size);
		Datum& addInternalAttribute(const std::string& t_name, const glm::vec4& t_value, const uint32_t& t_size);
		Datum& addInternalAttribute(const std::string& t_name, const glm::mat4x4& t_value, const uint32_t& t_size);

		Datum& addExternalAttribute(const std::string& t_name, int32_t* t_location, const uint32_t& t_size);
		Datum& addExternalAttribute(const std::string& t_name, float_t* t_location, const uint32_t& t_size);
		Datum& addExternalAttribute(const std::string& t_name, std::string* t_location, const uint32_t& t_size);
		Datum& addExternalAttribute(const std::string& t_name, glm::vec4* t_location, const uint32_t& t_size);
		Datum& addExternalAttribute(const std::string& t_name, glm::mat4x4* t_location, const uint32_t& t_size);

		Datum& addNestedScope(const std::string& t_name);
		Datum& createNestedScope(const std::string& t_name);

	};
}

