#pragma once
#include "RTTI.h"
#include "Factory.h"
namespace FieaGameEngine
{
	class Foo : public RTTI
	{
		RTTI_DECLARATIONS(Foo, RTTI);
	public:
		explicit Foo(int data = 0);
		~Foo() = default;
		int data() const;
		void SetData(int data);
		bool operator==(const Foo& t_rhs) const;
		bool operator!=(const Foo & t_rhs) const;
	private:
		int m_data;
	};

	ConcreteFactory(Foo, RTTI);
}

