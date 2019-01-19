#pragma once
namespace FieaGameEngine
{
	class Foo
	{
	public:
		explicit Foo(int data = 0);
		~Foo() = default;
		int data() const;
		bool operator==(const Foo& t_rhs) const;
		bool operator!=(const Foo & t_rhs) const;
	private:
		int m_data;
	};
}

