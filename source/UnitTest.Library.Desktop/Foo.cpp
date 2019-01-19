#include "pch.h"
#include "Foo.h"


namespace FieaGameEngine
{
	Foo::Foo(int data): m_data(data)
	{
	}

	int Foo::data() const
	{
		return m_data;
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