#include "Core/FSGAssert.hpp"
#include "Core/FSGLog.hpp"
#include "Math/FSGMath.hpp"
#include "Memory/FSGHeap.hpp"
#include "Memory/FSGMemoryInfo.hpp"
#include "Memory/FSGMemoryManager.hpp"
#include "Memory/FSGMemoryEntry.hpp"
#include "System/Thread_Common.hpp"

Heap::Heap(void* memory, u32 length, static_string name, i32 shouldFill, i32 (*callback)(class Heap*, u32))
{
    this->data                        = static_cast<u8*>(memory);
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
    this->ignoreFailedALlocations     = 0;
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

    this->firstBlock->blockStart = static_cast<u8*>(memory);
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
    CriticalSectionLock lock(&this->mutex);
    auto                success = false;

    if(!this->allocationsLocked)
    {
        success                 = true;
        this->allocationsLocked = true;
    }

    return success;
}

void Heap::UnlockAllocations()
{
    CriticalSectionLock lock(&this->mutex);

    if(this->allocationsLocked)
    {
        this->allocationsLocked = false;
    }
}

void Heap::AttachSibling(Heap* heap)
{
    CriticalSectionLock lock(&this->mutex);
    this->nextHeap = heap;
}

class HeapSnapshot* Heap::PushSnapshot(static_string heapName, static_string previousHeapName, u32 previousIndex)
{
    CriticalSectionLock lock(&this->mutex);

    this->currentSnapshot++;
    FSG_ASSERT(this->currentSnapshot >= 0, "Invalid snapshot index");
    FSG_ASSERT(this->currentSnapshot < MAX_SNAPSHOT_STACK_SIZE, "Invalid snapshot index!");

    auto snapshot = this->snapshots[this->currentSnapshot];
    snapshot.Create(this, this->currentSnapshot, heapName, previousHeapName, previousIndex);

    return &this->snapshots[this->currentSnapshot];
}

void Heap::PopSnapshot()
{
    CriticalSectionLock lock(&this->mutex);

    this->currentSnapshot--;

    FSG_ASSERT(this->currentSnapshot >= -1, "Invalid snapshot index");
    FSG_ASSERT(this->currentSnapshot < MAX_SNAPSHOT_STACK_SIZE, "Invalid snapshot index!");
}

void* Heap::operator new(size_t size)
{
    return malloc(size);
}

void Heap::operator delete(void* memory) noexcept
{
    free(memory);
}

void* Heap::Allocate(u32 size, u32 alignment, i32 isArray, i32 useGlobalHeap, u32 a6, static_string a7)
{
    void* allocated;
    for(allocated = this->InternalAllocate(size, alignment, a6, a7, isArray, useGlobalHeap);;
        allocated = this->InternalAllocate(size, alignment, a6, a7, isArray, useGlobalHeap))
    {
        if(allocated != nullptr)
        {
            break;
        }
        if(this->callback == nullptr)
        {
            break;
        }

        if(this->callback(this, size) && !this->ignoreFailedALlocations)
        {
            FSG_ASSERT(false, "Heap::Allocate() [%s] - ERROR Memory allocation failed, please check heap size.", this->name);
        }
    }

    for(auto snapshotHeap = this->snapshots[0].GetHeap(); allocated == nullptr; snapshotHeap = this->snapshots[0].GetHeap())
    {
        if(!snapshotHeap) break;
        allocated = snapshotHeap->InternalAllocate(size, alignment, a6, a7, isArray, useGlobalHeap);
    }

    FSG_ASSERT(FSG_IS_ALIGNED(allocated, alignment), "Allocation logic error, memory start address is not aligned.");
    return allocated;
}

void* Heap::Reallocate(void* oldBlock, u32 size, u32 alignment, i32 isArray, u32 a6, static_string a7)
{
    if(oldBlock == nullptr)
    {
        auto newBlock = this->Allocate(size, alignment, isArray, false, a6, a7);
        FSG_ASSERT(newBlock != nullptr, nullptr);
        return newBlock;
    }

    auto existingHeader = FSGGetMemoryEntryFromBlock(oldBlock);
    FSG_ASSERT(existingHeader->flagArray == isArray, "Array flag does not match old allocation array flag");
    FSG_ASSERT(FSG_IS_ALIGNED(oldBlock, alignment), "Old block was not aligned to the new requested alignment");

    if(existingHeader->unalignedLength >= size)
    {
        return oldBlock;
    }

    auto newBlock = this->Allocate(size, alignment, isArray, false, a6, a7);
    FSG_ASSERT(newBlock != nullptr, nullptr);
    memcpy(newBlock, oldBlock, existingHeader->unalignedLength);
    this->InternalFree(oldBlock, 0, nullptr, isArray);

    return newBlock;
}

void Heap::Free(void* block, i32 a3, u32 a4, static_string a5)
{
    this->InternalFree(block, a4, a5, a3);
}

void Heap::DumpStatistics()
{
    CriticalSectionLock lock(&this->mutex);
    MemoryInfo          info{};

    FSGDoLog("---=== Heap statistics for Heap::\"%s\" ===---\n", this->name);
    GetMemoryInfo(info, this->dataLength);
    FSGDoLog("Heap capacity: %s (%d bytes in total).\n", info.formatted, this->dataLength);
    FSGDoLog("Total number of successful allocations made: %d\n", this->numSuccessfulAllocations);
    FSGDoLog("Number of failed allocation requests made: %d\n", this->numFailedAllocationRequests);
    FSGDoLog("Number of locked allocation requests made: %d\n", this->numLockedRequestsMade);
    FSGDoLog("Current live allocations: %d\n", this->liveAllocations);
    FSGDoLog("Current number of free memory blocks: %d\n", this->freeBlocks);
    GetMemoryInfo(info, this->unalignedLength);
    FSGDoLog("Current memory allocation: %s (%d bytes in total).\n", info.formatted, this->unalignedLength);
    GetMemoryInfo(info, this->totalMemoryAllocation);
    FSGDoLog("Current memory allocation (including internal overhead): %s (%d bytes in total).\n", info.formatted, this->totalMemoryAllocation);
    GetMemoryInfo(info, this->peakDataLength);
    FSGDoLog("Peak memory allocation: %s (%d bytes in total).\n", info.formatted, this->peakDataLength);
    GetMemoryInfo(info, this->peakMemoryAllocations);
    FSGDoLog("Peak memory allocation (including internal overhead): %s (%d bytes in total).\n", info.formatted, this->peakMemoryAllocations);
}

void Heap::SetAllocationAlignment(u32 alignment)
{
    CriticalSectionLock lock(&this->mutex);

    FSG_ASSERT(0 == (alignment & (alignment - 1)), "Alignment value was not a power of 2.");
    FSG_ASSERT(alignment == 16 || alignment == 32 || alignment == 64 || alignment == 128, "Alignment is not one of 16, 32, 64, or 128.");

    auto oldPeakAlignment  = this->peakDefaultAlignment;
    this->defaultAlignment = alignment;
    if(oldPeakAlignment < alignment)
    {
        this->peakDefaultAlignment = alignment;
    }
}

Heap* Heap::GetMemoryHeap(void* block)
{
    auto header = FSGGetMemoryEntryFromBlock(block);
    return header->heap;
}

u8* Heap::DetermineAlignedMemoryPoint(struct MemoryEntry* block, u32 length, u32 a4, u32 userAlignment, i32 a6) const
{
    FSG_ASSERT(FSG_IS_ALIGNED(block, SYSTEM_MINIMUM_ALIGNMENT), nullptr);
    FSG_ASSERT(userAlignment >= this->defaultAlignment, nullptr);

    if(a6)
    {
        auto endOfBlock = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(block) + length);
        FSG_ASSERT(FSG_IS_ALIGNED(endOfBlock, SYSTEM_MINIMUM_ALIGNMENT), nullptr);

        // Todo: Make this line more legible.
        auto userMem =
          reinterpret_cast<u8*>(((reinterpret_cast<uintptr_t>(endOfBlock) + userAlignment - a4 - 1) / userAlignment - 1) * userAlignment);
        FSG_ASSERT(FSG_IS_ALIGNED(userMem, userAlignment), nullptr);

        return userMem;
    }
    else
    {
        // Todo: Make this line more legible.
        return reinterpret_cast<u8*>(((reinterpret_cast<uintptr_t>(&block->field0x1c) + userAlignment + 3) / userAlignment * userAlignment));
    }
}

i32 Heap::CombineFreeBlocks(MemoryEntry* blockA, MemoryEntry* blockB)
{
    FSG_ASSERT(blockA != nullptr, "An attempt was made to combine a nullptr memory block.");
    FSG_ASSERT(blockB != nullptr, "An attempt was made to combine a nullptr memory block.");
    FSG_ASSERT(blockA < blockB, "The first entry of a combine request did not come first in memory");

    FSG_ASSERT((u8*)blockA == blockA->blockStart, nullptr);
    FSG_ASSERT((u8*)blockB == blockB->blockStart, nullptr);

    auto oldLength = blockA->dataLength;
    if(blockA->blockStart + oldLength == blockB->blockStart)
    {
        blockA->dataLength += blockB->dataLength;
        blockA->next = blockB->next;
        if(blockA->next) blockA->next->previous = blockB;
        this->freeBlocks--;
        return true;
    }

    return false;
}

void Heap::PrepareHeader(MemoryEntry* entry, Heap* heap, MemoryBlockType type)
{
    FSG_ASSERT(entry != nullptr, nullptr);
    memset(entry, 0, sizeof(MemoryEntry));
    Heap::PrepareHeader_ChangeUsage(entry, type);
    entry->heap = heap;
}

void Heap::PrepareHeader_ChangeUsage(MemoryEntry* entry, MemoryBlockType type)
{
    FSG_ASSERT(entry != nullptr, nullptr);
    switch(type)
    {
        case MemoryBlockType::FREE:
            entry->blockType[0] = 'F';
            entry->blockType[1] = 'R';
            entry->blockType[2] = 'E';
            entry->flag0 &= 1;
            entry->flag1 &= 1;
            entry->flag2 &= 1;
            entry->flag3 &= 1;
            entry->flag4 &= 1;
            entry->flagWarnOnDelete &= 1;
            entry->flagArray &= 1;
            break;
        case MemoryBlockType::DEL:
            entry->blockType[0] = 'D';
            entry->blockType[1] = 'E';
            entry->blockType[2] = 'L';
            entry->flag0 &= 1;
            entry->flag1 &= 1;
            entry->flag2 &= 1;
            entry->flag3 &= 1;
            entry->flag4 &= 1;
            entry->flagWarnOnDelete &= 1;
            entry->flagArray &= 1;
            break;
        case MemoryBlockType::FSG:
            entry->blockType[0] = 'F';
            entry->blockType[1] = 'S';
            entry->blockType[2] = 'G';
            entry->flagAlive |= 1;
            break;

        case MemoryBlockType::STAGNANT:
            entry->blockType[0] = 'S';
            entry->blockType[1] = 'T';
            entry->blockType[2] = 'A';
            entry->flagAlive |= 1;
            break;

        default:
            FSG_ASSERT(type < MemoryBlockType::MAX, "Invalid block type")
            entry->blockType[0] = 'I';
            entry->blockType[1] = 'N';
            entry->blockType[2] = 'V';
            entry->flag0 &= 1;
            entry->flag1 &= 1;
            entry->flag2 &= 1;
            entry->flag3 &= 1;
            entry->flag4 &= 1;
            entry->flagWarnOnDelete &= 1;
            entry->flagArray &= 1;
            break;
    }
}

MemoryEntry* Heap::FindFreeBlock(u32 length, u32 alignment, i32 lastBlock) const
{
    // Fixme: This code needs heavy clean-up.

    MemoryEntry* entry = nullptr;
    auto         block = this->firstBlock;
    if(block != nullptr)
    {
        if(lastBlock)
        {
            for(auto nextBlock = block->next; nextBlock != nullptr; nextBlock = nextBlock->next)
            {
                block = nextBlock;
            }
        }

        FSG_ASSERT(this->defaultAlignment <= alignment, "User alignment less than default heap alignment")
        while(true)
        {
            // Check that there's enough space
            //  - Next block doesn't exist OR
            //
            //  - Next block must come after current memory buffer
            //  - Start of next block must be outside the current heaps range:
            //      memoryBuffer + heapLength < nextBlock

            FSG_ASSERT(block->next != nullptr || (block->next >= (void*)this->data && (block->next < (void*)(this->data + this->dataLength))),
                       "Next pointer [%p] is bad!")

            // Check that there's enough space
            //   - Previous block doesn't exist OR

            //   - Previous block must come after the current memory buffer
            //   - Start of previous block must be inside heap's range
            FSG_ASSERT(block->previous != nullptr ||
                         (block->previous >= (void*)this->data && block->previous < (void*)(this->data + this->dataLength)),
                       "Previous pointer [%p] is bad!");

            auto currentLength = this->dataLength;
            if(length <= currentLength)
            {
                auto required = this->CalculateMemoryRequired(block, currentLength, length, alignment, lastBlock);
                if(required <= currentLength)
                {
                    if(lastBlock)
                    {
                        return block;
                    }
                    if(entry != nullptr && currentLength >= entry->dataLength)
                    {
                        block = block->next;
                        if(block == nullptr)
                        {
                            return nullptr;
                        }
                    }
                    entry = block;
                }
            }
            if(!lastBlock)
            {
                block = block->next;
                if(!block)
                {
                    return nullptr;
                }
            }
            block = block->previous;
        }
    }
    return entry;
}

u32 Heap::CalculateMemoryRequired(MemoryEntry* block, u32 currentLength, u32 length, u32 alignment, i32 lastBlock) const
{
    // Todo: Figure out how this works

    FSG_ASSERT(FSG_IS_ALIGNED(block, SYSTEM_MINIMUM_ALIGNMENT), nullptr)
    auto aligned = this->DetermineAlignedMemoryPoint(block, currentLength, length, alignment, lastBlock);
    FSG_ASSERT(FSG_IS_ALIGNED(aligned, SYSTEM_MINIMUM_ALIGNMENT), nullptr)
    FSG_ASSERT(FSG_IS_ALIGNED(aligned, alignment), nullptr)

    if(lastBlock)
    {
        auto priorHeader = reinterpret_cast<MemoryEntry*>(reinterpret_cast<uintptr_t>(aligned) - sizeof(MemoryEntry));
        return reinterpret_cast<uintptr_t>(block) + 4 * (1 - priorHeader->flag2) + currentLength;
    }

    return reinterpret_cast<MemoryEntry*>(reinterpret_cast<uintptr_t>(aligned) + length)->flag2 - reinterpret_cast<uintptr_t>(block);
}

void* Heap::InternalAllocate(u32 length, u32 alignment, u32 a4, static_string a5, i32 isArray, i32 useGlobalHeap)
{
    // Create section lock
    CriticalSectionLock lock(&this->mutex);

    auto newEntry = MemoryEntry();

    FSG_ASSERT(length > 0, "Allocations of zero bytes not allowed")

    // Enforce minimum alignment of 0x10 bytes
    if(alignment < 0x10) alignment = 0x10;

    // Find the first free block that matches criteria
    auto freeBlock = this->FindFreeBlock(length, alignment, useGlobalHeap);

    MemoryEntry* nextBlock = nullptr;
    MemoryEntry* prevBlock = nullptr;
    if(freeBlock != nullptr)
    {
        nextBlock = freeBlock->next;
        if(nextBlock != nullptr) nextBlock->previous = freeBlock->previous;
        prevBlock = freeBlock->previous;
        if(prevBlock != nullptr) prevBlock->next = freeBlock->next;
        else
            this->firstBlock = freeBlock->next;

        // Set references to nullptr
        freeBlock->next     = nullptr;
        freeBlock->previous = nullptr;

        // Decrement free blocks
        this->freeBlocks--;

        // Alignment
        auto aligned     = this->DetermineAlignedMemoryPoint(freeBlock, freeBlock->dataLength, length, alignment, useGlobalHeap);
        auto blockLength = freeBlock->dataLength;

        u32 newBlockLength = 0;
        if(useGlobalHeap)
        {
            newBlockLength = reinterpret_cast<uintptr_t>(freeBlock) + blockLength - (reinterpret_cast<uintptr_t>(freeBlock) + length);
        }
        else
        {
            newBlockLength = blockLength - this->CalculateMemoryRequired(freeBlock, freeBlock->dataLength, length, alignment, 0);
        }

        auto userMem  = reinterpret_cast<MemoryEntry*>(aligned - sizeof(MemoryEntry));
        auto newBlock = (freeBlock + blockLength - newBlockLength);

        FSG_ASSERT(FSG_IS_ALIGNED(userMem, SYSTEM_MINIMUM_ALIGNMENT), nullptr)

        // Todo: Check if this is actually valid code
        auto sizeBefore = userMem - freeBlock;
        FSG_ASSERT(FSG_IS_ALIGNED(reinterpret_cast<void*>(sizeBefore), SYSTEM_MINIMUM_ALIGNMENT), nullptr);

        auto v29 = this->CalculateMemoryRequired(newBlock, newBlockLength, 0x10, this->defaultAlignment, useGlobalHeap);
        if(v29 <= newBlockLength && newBlockLength - v29 >= this->peakDefaultAlignment)
        {
            freeBlock->dataLength -= newBlockLength;
            Heap::PrepareHeader(newBlock, this, MemoryBlockType::FREE);
            newBlock->blockStart = reinterpret_cast<u8*>(newBlock);
            newBlock->dataLength = newBlockLength;
            this->AddFreeBlock(newBlock);
        }
        memcpy(&newEntry, freeBlock, sizeof(newEntry));
        auto v30 = this->CalculateMemoryRequired(freeBlock, userMem - freeBlock, 0x10, this->defaultAlignment, useGlobalHeap);
        if(v30 <= sizeBefore && sizeBefore - v30 >= this->peakDefaultAlignment)
        {
            newEntry.blockStart = reinterpret_cast<u8*>(userMem - sizeof(MemoryEntry));
            newEntry.dataLength -= sizeBefore;
            Heap::PrepareHeader(freeBlock, this, MemoryBlockType::FREE);
            freeBlock->blockStart = reinterpret_cast<u8*>(freeBlock);
            freeBlock->dataLength = sizeBefore;
            this->AddFreeBlock(freeBlock);
        }
        Heap::PrepareHeader_ChangeUsage(&newEntry, MemoryBlockType::FSG);
        newEntry.unalignedLength  = length;
        newEntry.previous         = nullptr;
        newEntry.next             = nullptr;
        newEntry.flag0            = true;
        newEntry.flag1            = true;
        newEntry.flag2            = true;
        newEntry.flag3            = true;
        newEntry.flag4            = true;
        newEntry.flagWarnOnDelete = true;
        newEntry.flagArray        = true;
        newEntry.flagAlive        = true;

        Heap::PrepareHeader(freeBlock, this, MemoryBlockType::INVALID);
        memcpy(&userMem - sizeof(MemoryEntry), &newEntry, sizeof(MemoryEntry));
        this->AddAllocatedBlock(userMem - sizeof(MemoryEntry));
        FSG_ASSERT(
          FSG_IS_ALIGNED(userMem, alignment), "[%s] - ERROR Allocation logic error, memory start address is incorrectly aligned.", this->name)

        return userMem;
    }

    if(this->snapshots[0].GetHeap() != nullptr || this->currentSnapshot == -1)
        FSG_ASSERT(this->ignoreFailedALlocations, "[%s] - ERROR Memory allocation failed for %i bytes, please check heap size.", this->name, length)
    else
        FSG_ASSERT(this->ignoreFailedALlocations, "[%s] - ERROR Memory allocation failed, please check heap size.", this->name);

    this->numFailedAllocationRequests++;
    return nullptr;
}

void Heap::InternalFree(void* memory, u32 a3, static_string a4, i32 isArray)
{
    // Create section lock
    CriticalSectionLock lock(&this->mutex);

    if(memory == nullptr) return;

    auto memBlock = reinterpret_cast<MemoryEntry*>(reinterpret_cast<uintptr_t>(memory) - sizeof(MemoryEntry));

    FSG_ASSERT(this == memBlock->heap, "[%s] - ERROR Attempt to free memory block was requested from the wrong heap.", this->name)
    FSG_ASSERT(!memBlock->flagWarnOnDelete, "[%s] - Warn on delete : Memory delete of allocation deleted from %s(%d) : detected", this->name, a4)
    FSG_ASSERT(!memBlock->flagAlive, "[%s] - ERROR An attempt was made to free a memory block that was already released.", this->name)
    FSG_ASSERT(
      !memBlock->flagArray, "[%s] - ERROR Memory block new/delete mismatch. Please check usage of new, new[] and delete, delete[].", this->name)

    this->RemoveAllocatedBlock(memBlock);
    if(memBlock != reinterpret_cast<MemoryEntry*>(memBlock->blockStart))
    {
        auto v17 = MemoryEntry{};
        memcpy(&v17, memBlock, sizeof(MemoryEntry));
        Heap::PrepareHeader(memBlock, this, MemoryBlockType::INVALID);
        memBlock = reinterpret_cast<MemoryEntry*>(v17.blockStart);
        memcpy(v17.blockStart, &v17, sizeof(MemoryEntry));
    }
    this->AddFreeBlock(memBlock);
}

void Heap::AddAllocatedBlock(struct MemoryEntry* block)
{
    this->liveAllocations++;
    this->numSuccessfulAllocations++;
    this->unalignedLength += block->dataLength;
    this->totalMemoryAllocation += block->dataLength;
    if(this->unalignedLength > this->peakDataLength) this->peakDataLength = this->unalignedLength;
    if(this->totalMemoryAllocation > this->peakMemoryAllocations) this->peakMemoryAllocations = this->totalMemoryAllocation;

    auto prevFirstBlock = this->firstAllocatedBlock;
    block->previous     = nullptr;
    block->next         = prevFirstBlock;
    if(prevFirstBlock != nullptr) prevFirstBlock->previous = block;
    this->firstAllocatedBlock = block;
}

void Heap::AddFreeBlock(struct MemoryEntry* freeBlock)
{
    FSG_ASSERT(freeBlock != nullptr, "A nullptr block was specified for addition to the Heap free list.")

    freeBlock->next     = nullptr;
    freeBlock->previous = nullptr;

    MemoryEntry* v5;
    auto         block = this->firstBlock;
    if(block != nullptr)
    {
        while(freeBlock >= block)
        {
            if(!block->next)
            {
                freeBlock->previous = block;
                block->next         = freeBlock;
                this->freeBlocks++;
                this->CombineFreeBlocks(block, freeBlock);
                return;
            }

            v5    = block;
            block = block->next;
        }
        if(v5 != nullptr)
        {
            v5->next = freeBlock;
        }
        else
        {
            this->firstBlock = freeBlock;
        }

        freeBlock->next     = block;
        freeBlock->previous = v5;
        block->previous     = freeBlock;
        this->freeBlocks++;
        this->CombineFreeBlocks(freeBlock, block);

        if(v5 != nullptr)
        {
            block = v5;
            this->CombineFreeBlocks(block, freeBlock);
        }
    }
    else
    {
        this->firstBlock = freeBlock;
        this->freeBlocks++;
    }
}

void Heap::RemoveAllocatedBlock(struct MemoryEntry* block)
{
    FSG_ASSERT(block != nullptr, "A nullptr item was requested for removal from the Heap allocation list.")

    if(block->previous != nullptr) block->previous->next = block->next;
    if(block->next != nullptr) block->next->previous = block->previous;

    Heap::PrepareHeader_ChangeUsage(block, MemoryBlockType::FREE);
    block->next = nullptr;
    block->previous = nullptr;

    this->unalignedLength -= block->unalignedLength;
    this->totalMemoryAllocation -= block->dataLength;
    this->liveAllocations--;
}