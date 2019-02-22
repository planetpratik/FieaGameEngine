#pragma once

#include <SDKDDKVer.h>

// Headers for CppUnitTest
#include "CppUnitTest.h"
#include "Foo.h"
#include "Bar.h"
#include "SList.h"
#include "Vector.h"
#include "DefaultHash.h"
#include "HashMap.h"
#include "Datum.h"
#include "AttributedFoo.h"
#include "Scope.h"
#include "Attributed.h"

#pragma warning(disable: 4189)
// TODO: reference additional headers your program requires here
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif