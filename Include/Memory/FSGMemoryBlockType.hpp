#pragma once

#include "Primitives.hpp"

enum MemoryBlockType
{
    FREE     = 0x0,
    DELETE   = 0x1,
    FSG      = 0x2,
    STAGNANT = 0x3,
    INVALID  = 0x4,
    MAX      = 0x5
};