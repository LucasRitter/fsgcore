#pragma once

#include "Primitives.hpp"
#ifdef WIN32
#include <Windows.h>
#endif


#if defined(_WIN64)
    #define SYSTEM_MINIMUM_ALIGNMENT 8
#elif defined(_WIN32)
    #define SYSTEM_MINIMUM_ALIGNMENT 4
#endif
#define FSG_IS_ALIGNED(OFFSET, ALIGNMENT) (reinterpret_cast<uintptr_t>(OFFSET) & (ALIGNMENT - 1)) == 0