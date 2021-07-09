#pragma once

#include "Primitives.hpp"
#include "Memory/FSGMemoryBlockType.hpp"
#include "System/FSGMutex.hpp"
#include "Memory/FSGHeapSnapshot.hpp"
#include "Memory/FSGAlignment.hpp"

#define MAX_SNAPSHOT_STACK_SIZE 5

typedef int(__cdecl* HeapAllocatedCallback)(class Heap*, u32);

class Heap
{
    private:
    u8*                   data;
    u32                   dataLength;
    static_string         name;
    class MemoryManager*  memoryManager;
    class Heap*           nextHeap;
    struct MemoryEntry*   firstBlock;
    struct MemoryEntry*   firstAllocatedBlock;
    u32                   field0x0x1c;
    u32                   defaultAlignment;
    u32                   peakDefaultAlignment;
    u32                   numSuccessfulAllocations;
    u32                   numLockedAllocations;
    u32                   numLockedRequestsMade;
    u32                   numFailedAllocationRequests;
    u32                   liveAllocations;
    u32                   freeBlocks;
    u32                   unalignedLength;
    u32                   totalMemoryAllocation;
    u32                   peakDataLength;
    u32                   peakMemoryAllocations;
    u32                   allocationsLocked;
    u32                   field0x50;
    u32                   ignoreFailedALlocations;
    RecursiveMutex        mutex;
    HeapSnapshot          snapshots[MAX_SNAPSHOT_STACK_SIZE];
    HeapAllocatedCallback callback;
    i32                   currentSnapshot;

    public:
    Heap(void* memory, u32 length, static_string name, i32 a5, i32 (*callback)(class Heap*, u32));
    ~Heap();

    bool LockAllocations();
    bool UnlockAllocations();
    bool AreAllocationsLocked() const;

    void                AttachSibling(Heap* heap);
    class HeapSnapshot* PushSnapshot(static_string heapName, static_string previousHeapName, u32 previousIndex);
    void                PopSnapshot();

    static void* operator new(size_t size);
    static void  operator delete(void* memory) noexcept;

    void* Allocate(u32 size, u32 alignment, i32 isArray, i32 useGlobalHeap, u32 a6, static_string a7);
    void* Reallocate(void* oldBlock, u32 size, u32 alignment, i32 isArray, u32 a6, static_string a7);
    void  Free(void*, i32, u32, static_string);

    // Not Implemented in-game
    // void DumpAllocs(string);
    void DumpStatistics();

    void                 SetAllocationAlignment(u32 alignment);
    class Heap*          GetNextHeap();
    static_string        GetName();
    class MemoryManager& GetMemoryManager();

    static class Heap* GetMemoryHeap(void* block);

    private:
    u8* DetermineAlignedMemoryPoint(struct MemoryEntry* block, u32 length, u32 a4, u32 userAlignment, i32 a6) const;
    i32 CombineFreeBlocks(struct MemoryEntry* blockA, struct MemoryEntry* blockB);

    static void PrepareHeader(struct MemoryEntry* entry, class Heap* heap, MemoryBlockType type);
    static void PrepareHeader_ChangeUsage(struct MemoryEntry* entry, MemoryBlockType type);

    [[nodiscard]] struct MemoryEntry* FindFreeBlock(u32 length, u32 alignment, i32 lastBlock) const;
    u32 CalculateMemoryRequired(struct MemoryEntry* block, u32 currentLength, u32 length, u32 alignment, i32 lastBlock) const;

    void* InternalAllocate(u32 length, u32 alignment, u32 a4, static_string a5, i32 isArray, i32 useGlobalHeap);
    void  InternalFree(void* memory, u32 a3, static_string a4, i32 isArray);

    void AddAllocatedBlock(struct MemoryEntry* block);
    void AddFreeBlock(struct MemoryEntry* freeBlock);

    void RemoveAllocatedBlock(struct MemoryEntry* block);
};