#include "pch.h"
#include "Attributed.h"
#include "TypeManager.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed);

#pragma region Attributed

	Attributed::Attributed(uint64_t t_type_id) :
		Scope()
	{
		Datum& datum = append("this");
		datum.pushBack(static_cast<RTTI*>(this));
		populate(t_type_id);
	}

	Attributed::Attributed(const Attributed& t_rhs) :
		Scope(t_rhs)
	{
		(*this)["this"].set(static_cast<RTTI*>(this));
		populate(t_rhs.TypeIdInstance());
	}

	Attributed::Attributed(Attributed&& t_rhs) :
		Scope(std::move(t_rhs))
	{
		(*this)["this"].set(static_cast<RTTI*>(this));
		populate(t_rhs.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed& t_rhs)
	{
		if (this != &t_rhs)
		{
			clear();
			Scope::operator=(t_rhs);
			(*this)["this"].set(static_cast<RTTI*>(this));
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
			(*this)["this"].set(static_cast<RTTI*>(this));
			populate(t_rhs.TypeIdInstance());
		}
		return *this;
	}

	void Attributed::clear()
	{
		Scope::clear();
	}
	
	bool Attributed::isAttribute(const std::string& t_name)
	{
		return (find(t_name) != nullptr);
	}

	bool Attributed::isPrescribedAttribute(const std::string& t_name)
	{
		bool found = false;
		const Vector<std::pair<std::string, Datum>*> attribute_pointers = getTableEntryPointers();
		if ("this" == t_name)
		{
			if (attribute_pointers[0]->first == "this")
			{
				return true;
			}
		}
		Vector<Signature> signatures = TypeManager::getSignatures(TypeIdInstance());
		for (auto& signature : signatures)
		{
			if (signature.sig_name == t_name)
			{
				found = true;
				break;
			}
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
		return datum;
	}

	Datum& Attributed::appendAuxillaryAttribute(const std::string& t_name, const Datum& t_datum)
	{
		if (isPrescribedAttribute(t_name))
		{
			throw std::exception("Invalid Operation! You cannot append already appended prescribed attribute as Auxillary.");
		}
		Datum& datum = append(t_name, t_datum);
		return datum;
	}

	Datum& Attributed::appendAuxillaryAttribute(const std::pair<std::string, Datum>& t_pair)
	{
		if (isPrescribedAttribute(t_pair.first))
		{
			throw std::exception("Invalid Operation! You cannot append already appended prescribed attribute as Auxillary.");
		}
		Datum& datum = append(t_pair.first, t_pair.second);
		return datum;
	}


	void Attributed::populate(uint64_t t_type_id)
	{
		Vector<Signature> signatures = TypeManager::getSignatures(t_type_id);
		for (auto& signature : signatures)
		{
			Datum& datum = append(signature.sig_name);
			datum.setType(signature.sig_type);
			if (signature.sig_type != Datum::DatumType::TABLE)
			{
				void* offset = reinterpret_cast<uint8_t*>(this) + signature.sig_offset;
				datum.setStorage(offset, signature.sig_size);
			}
		}
	}

	const Vector<std::pair<std::string, Datum>*> Attributed::getAttributes() const
	{
		const Vector<std::pair<std::string, Datum>*> attribute_pointers = getTableEntryPointers();
		return attribute_pointers;
	}

	const Vector<std::pair<std::string, Datum>*> Attributed::getPrescribedAttributes() const
	{
		Vector<Signature> signatures = TypeManager::getSignatures(TypeIdInstance());
		Vector<std::pair<std::string, Datum>*> attribute_pointers = getTableEntryPointers();
		Vector<std::pair<std::string, Datum>*> prescribed_attributes;
		for (auto& attribute_ptr : attribute_pointers)
		{
			for (auto& signature : signatures)
			{
				if (attribute_ptr->first == signature.sig_name)
				{
					prescribed_attributes.pushBack(attribute_ptr);
				}
			}
		}
		return prescribed_attributes;
	}

	const Vector<std::pair<std::string, Datum>*> Attributed::getAuxillaryAttributes() const
	{
		uint32_t prescribed_signatures_count = TypeManager::getSignatures(TypeIdInstance()).size();
		Vector<std::pair<std::string, Datum>*> attribute_pointers = getTableEntryPointers();
		Vector<std::pair<std::string, Datum>*> auxillary_attributes;

		for (uint32_t i = prescribed_signatures_count + 1; i < attribute_pointers.size(); ++i)
		{
			auxillary_attributes.pushBack(attribute_pointers[i]);
		}
		return auxillary_attributes;
	}

#pragma endregion

}

