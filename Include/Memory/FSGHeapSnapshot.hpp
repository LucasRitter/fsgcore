#pragma once

#include "Primitives.hpp"

#define HEAP_NAME_CAPACITY 0x100

class HeapSnapshot
{
    public:
    HeapSnapshot();
    void Create(class Heap* heap, u32 index, static_string heapName, static_string previousHeapName, u32 previousIndex);

    Heap* GetHeap();

    private:
    class Heap* heap;
    u32         index;
    u32         previousIndex;
    character   heapName[HEAP_NAME_CAPACITY];
    character   previousHeapName[HEAP_NAME_CAPACITY];
};