#ifdef WIN32

    #include "System/FSGThread.hpp"
    #include "Windows.h"

u64 FSGThread::CurrentThreadId()
{
    return GetCurrentThreadId();
}

void FSGThread::SleepCurrentThread(i32 milliseconds)
{
    Sleep(milliseconds);
}

void FSGThread::YieldCurrentThread()
{
    Sleep(0);
}

u32 FSGThread::AtomicIncrement(volatile std::atomic<u32>& atomic)
{
    return atomic++;
}

u32 FSGThread::AtomicDecrement(volatile std::atomic<u32>& atomic)
{
    return atomic--;
}

bool FSGThread::AtomicCompareAndExchange(volatile std::atomic<u32>& atomic, u32 comparison, u32 exchange)
{
    return atomic.compare_exchange_strong(comparison, exchange);
}

#endif