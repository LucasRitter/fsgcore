#include "System/Thread_Common.hpp"
#include "System/FSGMutex.hpp"
#include "Core/FSGAssert.hpp"

CriticalSectionLock::CriticalSectionLock(BaseMutex* mutex)
{
    FSG_ASSERT(mutex != nullptr, nullptr);
    this->mutex = mutex;
    this->mutex->LockMutex();
}

CriticalSectionLock::~CriticalSectionLock()
{
    FSG_ASSERT(this->mutex != nullptr, nullptr);
    this->mutex->UnlockMutex();
}