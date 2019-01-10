#include "pch.h"
#include "Foo.h"


namespace FieaGameEngine
{
	Foo::Foo(int data): mData(data)
	{
	}

	int Foo::Data() const
	{
		return mData;
	}

}