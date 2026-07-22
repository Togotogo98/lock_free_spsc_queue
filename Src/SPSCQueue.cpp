#include "../Include/SPSCQueue.h"
#include <stdexcept>

const size_t MIN_CAPACITY = 2;

SPSCQueue::SPSCQueue(size_t capacity) :
    buffer(capacity),
    head(0),
    tail(0),
    capacity(capacity) 
{
    if (capacity < MIN_CAPACITY)
    {
        throw std::invalid_argument("Queue capacity must be at least 2.");
    }
}

bool SPSCQueue::Empty() const
{
    return head.load(std::memory_order_acquire) ==
            tail.load(std::memory_order_acquire);
}

bool SPSCQueue::Full() const
{
    size_t currentTail = tail.load(std::memory_order_acquire);
    size_t currentHead = head.load(std::memory_order_acquire);

    return ((currentTail + 1) % capacity) == currentHead;
}

bool SPSCQueue::Push(int value)
{
    // relaxed because this is the only one that modifies the tail in a single-producer scenario
    size_t currentTail = tail.load(std::memory_order_relaxed);
    // acquire because head is modified by the consumer and we need to ensure we see the latest value
    size_t currentHead = head.load(std::memory_order_acquire);

    //full queue check, do it directly to avoid loading the atomics twice.
    size_t nextTail = (currentTail + 1) % capacity;

    if (nextTail == currentHead)
    {
        return false;
    }

    buffer[currentTail] = value;
    tail.store(nextTail, std::memory_order_release);
    return true;
}

bool SPSCQueue::Pop(int& value)
{
    size_t currentHead = head.load(std::memory_order_relaxed);
    size_t currentTail = tail.load(std::memory_order_acquire);
    
    if (currentHead == currentTail)
    {
        return false;
    }
    
    value = buffer[currentHead];

    size_t nextHead = (currentHead + 1) % capacity;
    head.store(nextHead, std::memory_order_release);

    return true;
}