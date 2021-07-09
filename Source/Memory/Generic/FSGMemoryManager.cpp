#include <Collections/FSGString.hpp>
#include "Core/FSGAssert.hpp"
#include "Core/FSGLog.hpp"
#include "Math/FSGMath.hpp"
#include "Memory/FSGHeap.hpp"
#include "Memory/FSGMemoryInfo.hpp"
#include "Memory/FSGMemoryManager.hpp"
#include "Memory/FSGMemoryEntry.hpp"
#include "System/Thread_Common.hpp"

MemoryManager::MemoryManager() : ThreadMutex()
{
    this->smallHeap                  = nullptr;
    this->allocHiHeap                = nullptr;
    this->firstHeap                  = nullptr;
    this->minimumSmallHeapAllocation = 0x20;
    this->defaultHeapTop             = 0;
    this->field0x18                  = nullptr;
    this->defaultHeaps[0]            = nullptr;
    this->defaultHeaps[1]            = nullptr;
}

MemoryManager::~MemoryManager()
{
    auto heap = this->firstHeap;
    if(heap != nullptr)
    {
        Heap* nextHeap;
        do
        {
            nextHeap = heap->GetNextHeap();
            delete heap;
            heap = nextHeap;
        } while(nextHeap != nullptr);
    }
}

MemoryManager& MemoryManager::GetInstance()
{
    return *MemoryManager::singleton;
}

bool MemoryManager::IsSingletonValid()
{
    return MemoryManager::singleton != nullptr;
}

void MemoryManager::CreateSingleton()
{
    FSG_ASSERT(MemoryManager::singleton == nullptr, "Singleton has already been created.")
    MemoryManager::singleton = new MemoryManager();
}

void MemoryManager::DeleteSingleton()
{
    FSG_ASSERT(MemoryManager::singleton != nullptr, "Singleton has not been created.")
    delete MemoryManager::singleton;
    MemoryManager::singleton = nullptr;
}

class Heap* MemoryManager::GetHeapByName(static_string name) const
{
    for(auto heap = this->firstHeap; heap; heap = this->GetNextHeap(heap))
    {
        auto heapName = heap->GetName();
        if(strcmp(name, heapName) == 0)
        {
            return heap;
        };
    }

    return nullptr;
}

class Heap* MemoryManager::GetNextHeap(class Heap* heap) const
{
    FSG_ASSERT(this == &heap->GetMemoryManager(), "Heap list being walked is not managed by the memory manager.")
    return heap->GetNextHeap();
}

void MemoryManager::PushDefaultHeap(class Heap* heap)
{
    CriticalSectionLock lock(this);
    FSG_ASSERT(this->defaultHeapTop < MAX_DEFAULT_HEAP_STACK_SIZE, nullptr);
    this->defaultHeaps[this->defaultHeapTop] = heap;
    this->defaultHeapTop++;
}

class Heap* MemoryManager::PopDefaultHeap()
{
    CriticalSectionLock lock(this);
    FSG_ASSERT(this->defaultHeapTop >= 1, nullptr)

    auto heap = this->defaultHeaps[this->defaultHeapTop];

    this->defaultHeaps[this->defaultHeapTop] = nullptr;
    this->defaultHeapTop--;

    return heap;
}

class Heap* MemoryManager::SetSmallHeap(class Heap* heap, size_t minSmallHeapAlloc)
{
    FSG_ASSERT(heap != nullptr, nullptr);
    FSG_ASSERT(this == &heap->GetMemoryManager(), "Small heap being set does not belong to the memory manager.")
    auto old = this->smallHeap;

    this->smallHeap                  = heap;
    this->minimumSmallHeapAllocation = minSmallHeapAlloc;

    return old;
}

class Heap* MemoryManager::SetAllocHiHeap(class Heap* heap)
{
    FSG_ASSERT(heap != nullptr, nullptr);
    FSG_ASSERT(this == &heap->GetMemoryManager(), "High heap being set does not belong to the memory manager.")
    auto old = this->allocHiHeap;

    this->allocHiHeap = heap;

    return old;
}

void* MemoryManager::Allocate(u32 dataLength, u32 alignment, i32 a3, i32 useGlobalHeap, u32 a5, static_string a6)
{
    class Heap* heap;

    if(useGlobalHeap)
    {
        heap = this->allocHiHeap;
        if(heap != nullptr) return heap->Allocate(dataLength, alignment, a3, useGlobalHeap, a5, a6);
    }

    heap = this->smallHeap;
    if(heap != nullptr)
    {
        if(dataLength < this->minimumSmallHeapAllocation)
        {
            return heap->Allocate(dataLength, alignment, a3, useGlobalHeap, a5, a6);
        }
    }

    heap = this->defaultHeaps[defaultHeapTop];
    if(heap != nullptr)
    {
        return heap->Allocate(dataLength, alignment, a3, useGlobalHeap, a5, a6);
    }

    FSG_ASSERT(false, "No heaps created.")
    return nullptr;
}

void* MemoryManager::Free(void* memory, i32 a2, u32 a3, static_string a4)
{
    if(memory != nullptr)
    {
        auto heap              = reinterpret_cast<class Heap*>(reinterpret_cast<uintptr_t>(memory) - 0x10);
        auto heapMemoryManager = &heap->GetMemoryManager();

        if(heapMemoryManager != nullptr)
        {
            FSG_ASSERT(this->IsValidHeap(heap), "An attempt was made to free a memory block that was not allocated by the memory manager.")
            heap->Free(memory, a2, a3, a4);
        }
        else
        {
            MemoryManager::FreeAlignedWithoutMM(memory);
        }
    }

    return nullptr;
}

bool MemoryManager::LockAllocations()
{
    auto success = false;

    if(this->smallHeap != nullptr)
    {
        success |= this->smallHeap->LockAllocations();
    }
    if(this->defaultHeaps[this->defaultHeapTop])
    {
        success |= this->defaultHeaps[this->defaultHeapTop]->LockAllocations();
    }

    return success;
}

bool MemoryManager::UnlockAllocations()
{
    auto success = false;

    if(this->smallHeap != nullptr)
    {
        success |= this->smallHeap->UnlockAllocations();
    }
    if(this->defaultHeaps[this->defaultHeapTop])
    {
        success |= this->defaultHeaps[this->defaultHeapTop]->UnlockAllocations();
    }

    return success;
}

bool MemoryManager::AreAllocationsLocked() const
{
    if(this->smallHeap != nullptr && this->smallHeap->AreAllocationsLocked())
    {
        return true;
    }

    if(this->defaultHeaps[this->defaultHeapTop] != nullptr && this->defaultHeaps[this->defaultHeapTop]->AreAllocationsLocked())
    {
        return true;
    }

    return false;
}

void* MemoryManager::AllocateAlignedWithoutMM(size_t size, u32 align)
{
    // FIXME: Implement MemoryManager::AllocateAlignedWithoutMM
    // Although this one is almost never used, since the Memory Manager is initialised during first boot.

    return nullptr;
}

void MemoryManager::FreeAlignedWithoutMM(void* mem)
{
    // FIXME: Implement MemoryManager::FreeAlignedWithoutMM
}

void* MemoryManager::ReallocateAlignedWithoutMM(void* mem, size_t size, u32 align)
{
    // FIXME: Implement MemoryManager::ReallocateAlignedWithoutMM
    return nullptr;
}

void MemoryManager::AddHeap(class Heap* heap)
{
    FSG_ASSERT(heap != nullptr, nullptr);

    heap->AttachSibling(this->firstHeap);
    this->firstHeap = heap;
}

void MemoryManager::RemoveHeap(class Heap* heap)
{
    FSG_ASSERT(heap != nullptr, nullptr)
    FSG_ASSERT(this == &heap->GetMemoryManager(), "Heap being deleted did not belong to the memory manager")

    auto        tempHeap = this->firstHeap;
    class Heap* lastHeap = nullptr;

    while(tempHeap != nullptr && tempHeap != heap)
    {
        lastHeap = tempHeap;
        tempHeap = tempHeap->GetNextHeap();
    }

    FSG_ASSERT(tempHeap != nullptr, "Heap was not registered with the memory manager.")
    FSG_ASSERT(
      this->defaultHeaps[this->defaultHeapTop] != heap, "Warning! Heap being deleted [%s] is currently set as the default heap.", heap->GetName())
    FSG_ASSERT(this->smallHeap != heap, "Warning! Heap being deleted [%s] is currently set as the small allocation heap.", heap->GetName())

    if(lastHeap != nullptr)
    {
        lastHeap->AttachSibling(heap->GetNextHeap());
    }
    else
    {
        this->firstHeap = heap->GetNextHeap();
    }
}

bool MemoryManager::IsValidHeap(class Heap* heap)
{
    for (auto i = this->firstHeap; i; i = i->GetNextHeap()) {
        if (i == heap) {
            return true;
        }
    }

    return false;
}