#pragma once

#include <queue>
#include <mutex>
#include <cstddef>

class MutexQueue
{
private:
    std::queue<int> buffer;
    mutable std::mutex q_mutex;
    size_t capacity;

public:
    explicit MutexQueue(size_t capacity);

    bool Push(int value);

    bool Pop(int& value);

    bool Empty() const;

    bool Full() const;
};