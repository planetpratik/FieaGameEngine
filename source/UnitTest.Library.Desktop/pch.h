#pragma once

#include <SDKDDKVer.h>

// Headers for CppUnitTest
#include "CppUnitTest.h"
#include "Foo.h"
#include "SList.h"
#pragma warning(disable: 4189)
// TODO: reference additional headers your program requires here
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif