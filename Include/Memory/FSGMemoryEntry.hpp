#pragma once

#include "Primitives.hpp"

enum MemoryEntryFlags : u8
{
    ARRAY = 1 << 6
};

struct MemoryEntry
{
    u8               blockType[3];
    MemoryEntryFlags flags;
    u32              unalignedLength;
    u32              dataLength;
    void*            blockStart;
    class Heap*      heap;
    MemoryEntry*     next;
    MemoryEntry*     previous;
    i32              field0x1c;
};