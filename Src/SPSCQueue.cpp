#include "../Include/SPSCQueue.h"


SPSCQueue::SPSCQueue(size_t capacity) :
    buffer(capacity),
    head(0),
    tail(0),
    capacity(capacity) {}

bool SPSCQueue::Empty() const
{
    return head == tail;
}

bool SPSCQueue::Full() const
{
    return ((tail + 1) % capacity) == head;
}

bool SPSCQueue::Push(int value)
{
    if (Full())
        return false;

    buffer[tail] = value;
    tail = (tail + 1) % capacity;
    return true;
}

bool SPSCQueue::Pop(int& value)
{
    if (Empty())
        return false;

    value = buffer[head];
    head = (head + 1) % capacity;
    return true;
}