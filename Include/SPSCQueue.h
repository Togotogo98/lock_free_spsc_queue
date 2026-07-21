#pragma once

#include <vector>
#include <cstddef>

class SPSCQueue
{
private:
    std::vector<int> buffer;
    size_t head; //next element to read
    size_t tail; //next element to write
    size_t capacity;

public:
    SPSCQueue(size_t capacity);

    bool Push(int value);

    bool Pop(int& value);

    bool Empty() const;

    bool Full() const;
};
