#include "pch.h"
#include "Foo.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Foo);

	Foo::Foo(int data): m_data(data)
	{
	}

	int Foo::data() const
	{
		return m_data;
	}

	void Foo::SetData(int data)
	{
		m_data = data;
	}

	bool Foo::operator==(const Foo & t_rhs) const
	{
		return m_data == t_rhs.m_data;
	}

	bool Foo::operator!=(const Foo & t_rhs) const
	{
		return !(*this == t_rhs);
	}

}