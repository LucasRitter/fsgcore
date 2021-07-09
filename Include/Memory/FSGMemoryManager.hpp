#pragma once

#include "Primitives.hpp"
#include "System/FSGMutex.hpp"

#define MAX_DEFAULT_HEAP_STACK_SIZE 2

class MemoryManager : public ThreadMutex
{
    private:
    static MemoryManager* singleton;

    protected:
    class Heap* smallHeap;
    class Heap* allocHiHeap;
    class Heap* firstHeap;

    u32         defaultHeapTop;
    void*       field0x18;
    class Heap* defaultHeaps[MAX_DEFAULT_HEAP_STACK_SIZE]{};
    u32         minimumSmallHeapAllocation;

    private:
    MemoryManager();
    ~MemoryManager();

    public:
    static class MemoryManager& GetInstance();
    static bool                 IsSingletonValid();
    static void                 CreateSingleton();
    static void                 DeleteSingleton();

    class Heap* GetHeapByName(static_string name) const;
    class Heap* GetNextHeap(class Heap* heap) const;

    void        PushDefaultHeap(class Heap* heap);
    class Heap* PopDefaultHeap();

    class Heap* SetSmallHeap(class Heap* heap, size_t minSmallHeapAlloc);
    class Heap* SetAllocHiHeap(class Heap* heap);

    void* Allocate(u32 dataLength, u32 alignment, i32 a3, i32 useGlobalHeap, u32 a5, static_string a6);
    void* Free(void* memory, i32 a2, u32 a3, static_string a4);

    bool LockAllocations();
    bool UnlockAllocations();
    bool AreAllocationsLocked() const;

    static void* AllocateAlignedWithoutMM(size_t size, u32 align);
    static void  FreeAlignedWithoutMM(void* mem);
    static void* ReallocateAlignedWithoutMM(void* mem, size_t size, u32 align);

    void AddHeap(class Heap* heap);
    void RemoveHeap(class Heap* heap);

    private:
    bool IsValidHeap(class Heap* heap);
};