#include "Memory/FSGHeapSnapshot.hpp"
#include "Collections/FSGString.hpp"

HeapSnapshot::HeapSnapshot()
{
    this->heap                = nullptr;
    this->index               = 0;
    this->previousIndex       = 0;
    this->heapName[0]         = 0;
    this->previousHeapName[0] = 0;
}

void HeapSnapshot::Create(class Heap* heap, u32 index, static_string heapName, static_string previousHeapName, u32 previousIndex)
{
    this->heap          = heap;
    this->index         = index;
    this->previousIndex = previousIndex;

    if(heapName != nullptr)
    {
        String::StringCopy(this->heapName, HEAP_NAME_CAPACITY, heapName);
    }
    else
    {
        this->heapName[0] = 0;
    }

    if(previousHeapName != nullptr)
    {
        String::StringCopy(this->previousHeapName, HEAP_NAME_CAPACITY, previousHeapName);
    }
    else
    {
        this->previousHeapName[0] = 0;
    }
}

Heap* HeapSnapshot::GetHeap()
{
    return this->heap;
}