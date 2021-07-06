#pragma once

#include "Primitives.hpp"

#define MEM_INFO_FORMATTED_STRING_CAPACITY 0x100

struct MemoryInfo
{
    u32       total;
    u32       bytes;
    u32       kibibytes;
    u32       mebibytes;
    character formatted[MEM_INFO_FORMATTED_STRING_CAPACITY];
};

void GetMemoryInfo(MemoryInfo& info, u32 length);