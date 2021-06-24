#include "Core/FSGAssert.hpp"
#include "Memory/FSGHeap.hpp"
#include "Memory/FSGMemoryManager.hpp"
#include "Memory/FSGMemoryEntry.hpp"
#include "System/Thread_Common.hpp"

Heap::Heap(void* memory, u32 length, static_string name, i32 shouldFill, i32 (*callback)(class Heap*, u32))
{
    this->data                        = memory;
    this->dataLength                  = length;
    this->name                        = name;
    this->memoryManager               = &MemoryManager::GetInstance();
    this->nextHeap                    = nullptr;
    this->firstBlock                  = nullptr;
    this->firstAllocatedBlock         = nullptr;
    this->field0x0x1c                 = 0;
    this->defaultAlignment            = 0x10;
    this->peakDefaultAlignment        = 0;
    this->numSuccessfulAllocations    = 0;
    this->numLockedRequestsMade       = 0;
    this->numFailedAllocationRequests = 0;
    this->liveAllocations             = 0;
    this->freeBlocks                  = 1;
    this->unalignedLength             = 0;
    this->totalMemoryAllocation       = 0;
    this->allocationsLocked           = 0;
    this->field0x54                   = 0;
    this->mutex                       = RecursiveMutex();

    this->numLockedAllocations  = 0;
    this->peakDataLength        = 0;
    this->peakMemoryAllocations = 0;
    this->field0x50             = 0;

    for(auto snapshot : this->snapshots)
    {
    }

    this->callback = callback;
    FSG_ASSERT(this->name != nullptr, "No heap name was supplied to the Heap constructor.");
    FSG_ASSERT(this->dataLength > sizeof(MemoryEntry), "Heap data size was too small to support allocations.");
    FSG_ASSERT(this->data != nullptr, "A nullptr data buffer was supplied when creating a Heap object.");

    FSG_ASSERT(FSG_IS_ALIGNED(this->data, SYSTEM_MINIMUM_ALIGNMENT), nullptr);
    FSG_ASSERT(!(this->dataLength & 3), nullptr);

    if(shouldFill && this->dataLength)
    {
        memset(this->data, 0x88, this->dataLength);
    }

    this->firstBlock = static_cast<MemoryEntry*>(memory);
    Heap::PrepareHeader(this->firstBlock, this, MemoryBlockType::FREE);

    this->firstBlock->blockStart = memory;
    this->firstBlock->dataLength = dataLength;
    this->currentSnapshot        = -1;

    this->PushSnapshot(name, nullptr, 0);
    this->memoryManager->AddHeap(this);
}

Heap::~Heap()
{
    this->PopSnapshot();
    FSG_ASSERT(this->currentSnapshot != -1, "Snapshots not all popped from stack!");
    this->memoryManager->RemoveHeap(this);
    this->mutex.~RecursiveMutex();
}

bool Heap::LockAllocations()
{
    auto lock    = new CriticalSectionLock(&this->mutex);
    auto success = false;

    if(!this->allocationsLocked)
    {
        success                 = true;
        this->allocationsLocked = true;
    }

    delete lock;
    return success;
}

void Heap::UnlockAllocations()
{
    auto lock = new CriticalSectionLock(&this->mutex);

    if(this->allocationsLocked)
    {
        this->allocationsLocked = false;
    }

    delete lock;
}

void Heap::AttachSibling(Heap* heap)
{
    auto lock      = new CriticalSectionLock(&this->mutex);
    this->nextHeap = heap;
    delete lock;
}

class HeapSnapshot* Heap::PushSnapshot(static_string heapName, static_string previousHeapName, u32 previousIndex)
{
    auto lock = new CriticalSectionLock(&this->mutex);

    this->currentSnapshot++;
    FSG_ASSERT(this->currentSnapshot >= 0, "Invalid snapshot index");
    FSG_ASSERT(this->currentSnapshot < MAX_SNAPSHOT_STACK_SIZE, "Invalid snapshot index!");

    auto snapshot = this->snapshots[this->currentSnapshot];
    snapshot.Create(this, this->currentSnapshot, heapName, previousHeapName, previousIndex);

    delete lock;
    return &this->snapshots[this->currentSnapshot];
}

void Heap::PopSnapshot()
{
    auto lock = new CriticalSectionLock(&this->mutex);

    this->currentSnapshot--;

    FSG_ASSERT(this->currentSnapshot >= -1, "Invalid snapshot index");
    FSG_ASSERT(this->currentSnapshot < MAX_SNAPSHOT_STACK_SIZE, "Invalid snapshot index!");

    delete lock;
}

void* Heap::operator new(size_t size)
{
    return malloc(size);
}

void Heap::operator delete(void* memory) noexcept
{
    free(memory);
}

void* Heap::Allocate(u32 size, u32 alignment, i32 a4, i32 useGlobalHeap, u32 a6, static_string a7)
{
    void* allocated;
    for(allocated = this->InternalAllocate(size, alignment, a6, a7, a4, useGlobalHeap);;
        allocated = this->InternalAllocate(size, alignment, a6, a7, a4, useGlobalHeap))
    {
        if(allocated != nullptr)
        {
            break;
        }
        if(this->callback == nullptr)
        {
            break;
        }

        if(this->callback(this, size) && !this->field0x54)
        {
            FSG_ASSERT(false, "Heap::Allocate() [%s] - ERROR Memory allocation failed, please check heap size.", this->name);
        }
    }

    for(auto snapshotHeap = this->snapshots[0].GetHeap(); allocated == nullptr; snapshotHeap = this->snapshots[0].GetHeap())
    {
        if(!snapshotHeap) break;
        allocated = snapshotHeap->InternalAllocate(size, alignment, a6, a7, a4, useGlobalHeap);
    }

    FSG_ASSERT(FSG_IS_ALIGNED(allocated, alignment), "Allocation logic error, memory start address is not aligned.");
    return allocated;
}

void * Heap::Reallocate(void* oldBlock, u32 size, u32 alignment, i32 a5, u32 a6, static_string a7)
{
    if (oldBlock == nullptr) {
        auto newBlock = this->Allocate(size, alignment, a5, false, a6, a7);
        FSG_ASSERT(newBlock != nullptr, nullptr);
    }
}