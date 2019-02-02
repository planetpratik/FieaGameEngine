#include "pch.h"
#include "Bar.h"


namespace FieaGameEngine
{
	Bar::Bar(int data) : m_data(data)
	{
	}

	int Bar::data() const
	{
		return m_data;
	}

	void Bar::SetData(int data)
	{
		m_data = data;
	}

	bool Bar::operator==(const Bar& t_rhs) const
	{
		return m_data == t_rhs.m_data;
	}

	bool Bar::operator!=(const Bar& t_rhs) const
	{
		return !(*this == t_rhs);
	}

}