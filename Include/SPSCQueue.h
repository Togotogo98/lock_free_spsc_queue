#pragma once

#include <vector>
#include <cstddef>
#include <atomic>

class SPSCQueue
{
private:
    std::vector<int> buffer;
    std::atomic<size_t> head;
    std::atomic<size_t> tail;
    size_t capacity;

public:
    SPSCQueue(size_t capacity);

    bool Push(int value);

    bool Pop(int& value);

    bool Empty() const;

    bool Full() const;
};
