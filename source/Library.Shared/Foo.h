#pragma once
namespace FieaGameEngine
{
	class Foo
	{
	public:
		explicit Foo(int data = 0);
		~Foo() = default;
		int Data() const;
	private:
		int mData;
	};
}

