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
    void*                 data;
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
    u32                   field0x54;
    RecursiveMutex        mutex;
    HeapSnapshot          snapshots[MAX_SNAPSHOT_STACK_SIZE];
    HeapAllocatedCallback callback;
    i32                   currentSnapshot;

    public:
    Heap(void* memory, u32 length, static_string name, i32 a5, i32 (*callback)(class Heap*, u32));
    ~Heap();

    bool LockAllocations();
    void UnlockAllocations();

    void                AttachSibling(Heap* heap);
    class HeapSnapshot* PushSnapshot(static_string heapName, static_string previousHeapName, u32 previousIndex);
    void                PopSnapshot();

    static void* operator new(size_t size);
    static void  operator delete(void* memory) noexcept;

    void* Allocate(u32 size, u32 alignment, i32 a4, i32 useGlobalHeap, u32 a6, static_string a7);
    void* Reallocate(void* oldBlock, u32 size, u32 alignment, i32 a5, u32 a6, static_string a7);
    void  Free(void*, i32, u32, static_string);

    void DumpAllocs(string);
    void DumpStatistics();

    void SetAllocationAlignment(u32);

    static class Heap* GetMemoryHeap(void*);

    private:
    string DetermineAlignedMemoryPoint(struct MemoryEntry*, u32, u32, u32, i32) const;
    i32    CombineFreeBlocks(struct MemoryEntry*, struct MemoryEntry*);

    static void PrepareHeader(struct MemoryEntry*, Heap*, MemoryBlockType);
    static void PrepareHeader_ChangeUsage(struct MemoryEntry*, MemoryBlockType);

    struct MemoryEntry* FindFreeBlock(u32, u32, i32) const;
    u32                 CalculateMemoryRequired(struct MemoryEntry*, u32, u32, u32, i32) const;

    void* InternalAllocate(u32, u32, u32, static_string, i32, i32);
    void  InternalFree(void*, u32, static_string, i32);

    void AddAllocatedBlock(struct MemoryEntry*);
    void AddFreeBlock(struct MemoryEntry*);
};