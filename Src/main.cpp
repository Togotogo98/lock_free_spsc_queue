#include "SPSCQueue.h"
#include <iostream>
#include <thread>
#include <mutex>

std::mutex printMutex;
int mismatchCount = 0;

void CheckValue(int value, int expected) 
{
    if (value != expected) 
    {
        std::lock_guard<std::mutex> lock(printMutex);
        std::cerr << "Error: Expected " << expected << ", but got " << value << std::endl;
        ++mismatchCount;
    }
}

int main()
{
    SPSCQueue queue(5);

    std::thread producer([&queue]() 
    {
        for (int i = 0; i < 10; ++i) 
        {
            while (queue.Full() || !queue.Push(i)) 
            {
                // Queue is full, wait till its false.
            }
        }
        {
            std::lock_guard<std::mutex> lock(printMutex);
            std::cout << "Producer finished producing." << std::endl;
        }
    });

    std::thread consumer([&queue]() 
    {
        for (int i = 0; i < 10; ++i) 
        {
            int value;
            while (queue.Empty() || !queue.Pop(value)) 
            {
               // Queue is empty, wait till its false.
            }
            CheckValue(value, i);
        }
        {
            std::lock_guard<std::mutex> lock(printMutex);
            if (mismatchCount == 0) 
            {
                std::cout << "Consumer finished consuming. No mismatches." << std::endl;
            } 
            else 
            {
                std::cerr << "Total mismatches: " << mismatchCount << std::endl;
            }
        }
    });

    producer.join();
    consumer.join();

    return 0;
}