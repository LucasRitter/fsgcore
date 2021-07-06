#include "Math/FSGMath.hpp"
#include "Memory/FSGMemoryEntry.hpp"

MemoryEntry* FSGGetMemoryEntryFromBlock(void* block)
{
    return reinterpret_cast<MemoryEntry*>(reinterpret_cast<uintptr_t>(block) - FSGNextMultiple(sizeof(MemoryEntry), 32));
}