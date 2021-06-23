#ifdef WIN32

    #include "System/FSGMutex.hpp"
    #include "Core/FSGAssert.hpp"
    #include "System/FSGThread.hpp"

ThreadMutex::ThreadMutex()
{
    this->mutex = CreateMutexA(nullptr, 0, nullptr);
}

ThreadMutex::~ThreadMutex()
{
    CloseHandle(this->mutex);
}

void ThreadMutex::LockMutex()
{
    WaitForSingleObject(this->mutex, MAXDWORD32);
}

void ThreadMutex::UnlockMutex()
{
    ReleaseMutex(this->mutex);
}

RecursiveMutex::RecursiveMutex()
{
    this->threadMutex = ThreadMutex();
    this->lockCount   = 0;
    this->threadId    = 0;
}

RecursiveMutex::~RecursiveMutex()
{
    FSG_ASSERT(this->lockCount == 0, nullptr);
    this->threadMutex.~ThreadMutex();
}

void RecursiveMutex::LockMutex()
{
    auto currentThread = FSGThread::CurrentThreadId();
    if(this->lockCount == 0 || this->threadId != currentThread)
    {
        this->threadMutex.LockMutex();
        this->threadId = currentThread;
    }
    this->lockCount++;
}

void RecursiveMutex::UnlockMutex()
{
    FSG_ASSERT(this->lockCount > 0 && this->threadId == FSGThread::CurrentThreadId(), nullptr);
    this->lockCount--;
    if(this->lockCount == 0)
    {
        this->threadMutex.UnlockMutex();
    }
}

u32 RecursiveMutex::GetLockCount() const
{
    return this->lockCount;
}

u64 RecursiveMutex::GetThreadId() const
{
    return this->threadId;
}

#endif