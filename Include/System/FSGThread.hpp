#pragma once

#include <atomic>
#include "Primitives.hpp"

namespace FSGThread
{
    /**
     * @return The current thread ID.
     */
    u64 CurrentThreadId();

    /**
     * Puts the current thread to sleep for a given amount of @b milliseconds.
     * @param milliseconds How long the current thread should sleep for.
     */
    void SleepCurrentThread(i32 milliseconds);

    /**
     * Yields the current thread.
     */
    void YieldCurrentThread();

    u32  AtomicIncrement(volatile std::atomic<u32>& atomic);
    u32  AtomicDecrement(volatile std::atomic<u32>& atomic);
    bool AtomicCompareAndExchange(volatile std::atomic<u32>& atomic, u32 comparison, u32 exchange);
}  // namespace FSGThread