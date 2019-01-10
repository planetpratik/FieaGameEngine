
#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files
#include <windows.h>

// OpenGL
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#pragma warning(push)
#pragma warning(disable: 4201)
#include "glm/glm.hpp"
#pragma warning(pop)

// reference additional headers your program requires here
