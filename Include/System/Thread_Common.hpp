#pragma once

#include "Primitives.hpp"

class CriticalSectionLock
{
    protected:
    class BaseMutex* mutex;

    public:
    CriticalSectionLock(class BaseMutex* mutex);
    ~CriticalSectionLock();
};