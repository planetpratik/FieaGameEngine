#pragma once
#include "RTTI.h"

namespace FieaGameEngine
{
	class Bar : public RTTI
	{
		RTTI_DECLARATIONS(Bar, RTTI);

	public:
		explicit Bar(int data = 0);
		~Bar() = default;
		int data() const;
		void SetData(int data);
		bool operator==(const Bar& t_rhs) const;
		bool operator!=(const Bar& t_rhs) const;
	private:
		int m_data;
	};
}