#pragma once

#ifdef WIN32
    #include <Windows.h>
#endif

#include "Primitives.hpp"

class BaseMutex
{
    public:
    virtual void LockMutex()   = 0;
    virtual void UnlockMutex() = 0;
};

class ThreadMutex : public BaseMutex
{
    protected:
#ifdef WIN32
    HANDLE mutex;
#endif

    public:
    ThreadMutex();
    ~ThreadMutex();

    void LockMutex() override;
    void UnlockMutex() override;
};

class RecursiveMutex : public BaseMutex
{
    protected:
    ThreadMutex threadMutex;
    u32         lockCount;
    u64         threadId;

    public:
    RecursiveMutex();
    ~RecursiveMutex();

    void LockMutex() override;
    void UnlockMutex() override;

    u32 GetLockCount() const;
    u64 GetThreadId() const;
};