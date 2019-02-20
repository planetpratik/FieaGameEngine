#include "pch.h"
#include "Attributed.h"
#include "TypeManager.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed);

#pragma region Signature

	Signature::Signature(const std::string & t_name, const Datum::DatumType & t_type, const uint32_t & t_size, const size_t & t_offset) :
		sig_name(t_name), sig_type(t_type), sig_size(t_size), sig_offset(t_offset)
	{

	}

#pragma endregion


#pragma region Attributed

	Attributed::Attributed() :
		Scope(), m_prescribed_attributes(), m_auxillary_attributes()
	{
		(*this)["this"].set(this);
	}

	Attributed::Attributed(uint32_t t_type_id) :
		Scope(), m_prescribed_attributes(), m_auxillary_attributes()
	{
		populate(t_type_id);
	}

	Attributed::Attributed(const Attributed& t_rhs) :
		Scope(t_rhs), m_prescribed_attributes(t_rhs.m_prescribed_attributes), m_auxillary_attributes(t_rhs.m_auxillary_attributes)
	{
		(*this)["this"].set(this);
		populate(t_rhs.TypeIdInstance());
	}
	Attributed::Attributed(Attributed&& t_rhs) :
		Scope(std::move(t_rhs)), m_prescribed_attributes(t_rhs.m_prescribed_attributes), m_auxillary_attributes(t_rhs.m_auxillary_attributes)
	{
		(*this)["this"].set(this);
	}
	Attributed& Attributed::operator=(const Attributed& t_rhs)
	{
		if (this != &t_rhs)
		{
			clear();
			Scope::operator=(t_rhs);
			(*this)["this"].set(this);
			populate(t_rhs.TypeIdInstance());
		}
		return *this;
	}
	Attributed& Attributed::operator=(Attributed&& t_rhs)
	{
		if (this != &t_rhs)
		{
			clear();
			Scope::operator=(std::move(t_rhs));
			(*this)["this"].set(this);
			populate(t_rhs.TypeIdInstance());
		}
		return *this;
	}

	void Attributed::clear()
	{
		Scope::clear();
		m_prescribed_attributes.clear();
		m_auxillary_attributes.clear();
	}
	
	bool Attributed::isAttribute(const std::string& t_name)
	{
		return (find(t_name) != nullptr);
	}
	bool Attributed::isPrescribedAttribute(const std::string& t_name)
	{
		bool found = true;
		if (m_prescribed_attributes.find(t_name) == m_prescribed_attributes.end())
		{
			found = false;
		}
		return found;
	}
	bool Attributed::isAuxillaryAttribute(const std::string& t_name)
	{
		return isAttribute(t_name) && !isPrescribedAttribute(t_name);
	}
	Datum& Attributed::appendAuxillaryAttribute(const std::string& t_name)
	{
		if (isPrescribedAttribute(t_name))
		{
			throw std::exception("Invalid Operation! You cannot append already appended prescribed attribute as Auxillary.");
		}
		Datum& datum = append(t_name);
		m_auxillary_attributes.insert(std::pair<std::string, Datum*>(t_name, &datum));
	}

	void Attributed::populate(uint32_t t_type_id)
	{

	}

	const HashMap<std::string, Datum*> Attributed::getPrescribedAttributes() const
	{
		return m_prescribed_attributes;
	}

	const HashMap<std::string, Datum*> Attributed::getAuxillaryAttributes() const
	{
		return m_auxillary_attributes;
	}

#pragma endregion

#pragma region AddInternalAttributes

	Datum& Attributed::addInternalAttribute(const std::string& t_name, const int32_t& t_value, const uint32_t& t_size)
	{
		Datum& datum = append(t_name);
		//
	}

	Datum& Attributed::addInternalAttribute(const std::string& t_name, const float_t& t_value, const uint32_t& t_size)
	{
		// TODO: insert return statement here
	}

	Datum& Attributed::addInternalAttribute(const std::string& t_name, const std::string& t_value, const uint32_t& t_size)
	{
		// TODO: insert return statement here
	}

	Datum& Attributed::addInternalAttribute(const std::string& t_name, const glm::vec4& t_value, const uint32_t& t_size)
	{
		// TODO: insert return statement here
	}

	Datum& Attributed::addInternalAttribute(const std::string& t_name, const glm::mat4x4& t_value, const uint32_t& t_size)
	{
		// TODO: insert return statement here
	}

#pragma endregion

#pragma region AddExternalAttributes

	Datum& Attributed::addExternalAttribute(const std::string& t_name, int32_t* t_value, const uint32_t& t_size)
	{
		// TODO: insert return statement here
	}

	Datum& Attributed::addExternalAttribute(const std::string& t_name, float_t* t_value, const uint32_t& t_size)
	{
		// TODO: insert return statement here
	}

	Datum& Attributed::addExternalAttribute(const std::string& t_name, std::string* t_value, const uint32_t& t_size)
	{
		// TODO: insert return statement here
	}

	Datum& Attributed::addExternalAttribute(const std::string& t_name, glm::vec4* t_value, const uint32_t& t_size)
	{
		// TODO: insert return statement here
	}

	Datum& Attributed::addExternalAttribute(const std::string& t_name, glm::mat4x4* t_value, const uint32_t& t_size)
	{
		// TODO: insert return statement here
	}

#pragma endregion

	Datum& FieaGameEngine::Attributed::addNestedScope(const std::string& t_name)
	{
		// TODO: insert return statement here
	}

	Datum& Attributed::createNestedScope(const std::string& t_name)
	{
		// TODO: insert return statement here
	}
}

