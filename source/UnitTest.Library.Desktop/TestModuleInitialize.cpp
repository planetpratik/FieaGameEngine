#include "pch.h"
#include "TypeManager.h"
#include "AttributedFoo.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_MODULE_INITIALIZE(TestModuleInitialize)
	{
		TypeManager::registerType(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
		TypeManager::registerType(Entity::TypeIdClass(), Entity::Signatures());
		TypeManager::registerType(Sector::TypeIdClass(), Sector::Signatures());
		TypeManager::registerType(World::TypeIdClass(), World::Signatures());
	}

	TEST_MODULE_CLEANUP(TestModuleCleanup)
	{
		// ONLY FOR CODE COVERAGE IMPROVEMENT PURPOSE. WILL BE REMOVED ONCE removeType IMPLEMENTED!
		uint64_t type_id = AttributedFoo::TypeIdClass();
		TypeManager::removeType(type_id);
		/*-----------------------------------------------*/
		TypeManager::clear();
	}
}