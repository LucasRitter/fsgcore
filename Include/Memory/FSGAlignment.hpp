#include "Primitives.hpp"

#pragma once

#define SYSTEM_MINIMUM_ALIGNMENT 4
#define FSG_IS_ALIGNED(OFFSET, ALIGNMENT) (reinterpret_cast<uintptr_t>(OFFSET) & (ALIGNMENT - 1)) == 0