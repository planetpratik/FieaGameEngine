#pragma once

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define GLM_ENABLE_EXPERIMENTAL
// reference additional headers your program requires here
#include <exception>
#include <stdint.h>
#pragma warning(push)
#pragma warning(disable: 4201)
#include "glm/vec4.hpp"
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 4201)
#include "glm/mat4x4.hpp"
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 4201)
#include <glm/gtx/string_cast.hpp>
#pragma warning(pop)
#include "RTTI.h"