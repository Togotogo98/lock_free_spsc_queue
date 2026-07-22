#include "../Include/MutexQueue.h"
#include <stdexcept>

MutexQueue::MutexQueue(size_t capacity)
    : capacity(capacity)
{
    if (capacity < 2)
    {
        throw std::invalid_argument("Queue capacity must be at least 2.");
    }
}

bool MutexQueue::Push(int value)
{
    std::lock_guard<std::mutex> lock(q_mutex);
    if (buffer.size() >= capacity)
    {
        return false;
    }
    buffer.push(value);
    return true;
}

bool MutexQueue::Pop(int& value)
{
    std::lock_guard<std::mutex> lock(q_mutex);
    if (buffer.empty())
    {
        return false;
    }
    value = buffer.front();
    buffer.pop();
    return true;
}

bool MutexQueue::Empty() const
{
    std::lock_guard<std::mutex> lock(q_mutex);
    return buffer.empty();
}

bool MutexQueue::Full() const
{
    std::lock_guard<std::mutex> lock(q_mutex);
    return buffer.size() >= capacity;
}