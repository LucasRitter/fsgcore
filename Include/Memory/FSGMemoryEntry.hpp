#pragma once

#include "Primitives.hpp"

enum MemoryEntryFlags : u8
{
    ARRAY = 1 << 6
};

struct MemoryEntry
{
    u8           blockType[3];
    u8           flag0            : 1;
    u8           flag1            : 1;
    u8           flag2            : 1;
    u8           flag3            : 1;
    u8           flag4            : 1;
    u8           flagWarnOnDelete : 1;
    u8           flagArray        : 1;
    u8           flag7            : 1;
    u32          unalignedLength;
    u32          dataLength;
    u8*          blockStart;
    class Heap*  heap;
    MemoryEntry* next;
    MemoryEntry* previous;
    i32          field0x1c;
};

MemoryEntry* FSGGetMemoryEntryFromBlock(void* block);