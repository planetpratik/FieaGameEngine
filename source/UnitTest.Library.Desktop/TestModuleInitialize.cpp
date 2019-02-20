#include "pch.h"
#include "TypeManager.h"
#include "AttributedFoo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_MODULE_INITIALIZE(TestModuleInitialize)
	{
		TypeManager::registerType(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
	}

	TEST_MODULE_CLEANUP(TestModuleCleanup)
	{
		TypeManager::clear();
	}
}