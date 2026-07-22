#include "SPSCQueue.h"
#include "MutexQueue.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>

const size_t QUEUE_SIZE = 1024;
const int NUM_MESSAGES = 1000000;
std::mutex printMutex;
int mismatchCount = 0;

template<typename QueueType>

void RunBenchmark(const std::string& name)
{
    QueueType queue(QUEUE_SIZE);

    auto start = std::chrono::high_resolution_clock::now();

    std::thread producer([&]()
    {
        for (int i = 0; i < NUM_MESSAGES; ++i)
        {
            while (!queue.Push(i))
            {
                // Busy wait until space is available.
            }
        }
    });

    std::thread consumer([&]() 
    {
        int value;
        int expected = 0;

        while (expected < NUM_MESSAGES) 
        {
            while (!queue.Pop(value)) 
            {
                // busy wait.
            }
            if (value != expected) 
            {
                std::lock_guard<std::mutex> lock(printMutex);
                std::cout << "Error: Expected " 
                            << expected 
                            << ", but got "
                            << value 
                            << std::endl;
                ++mismatchCount;
            }
            ++expected;
        }
    });

    producer.join();
    consumer.join();

    // Print the benchmark result
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << "Benchmark completed in " << duration.count() << " ms." << std::endl;

    double throughput =
        static_cast<double>(NUM_MESSAGES) / (duration.count() / 1000.0);

    double throughputM = throughput / 1000000.0;

    std::cout << "=============================\n";
    std::cout << name << '\n';
    std::cout << "=============================\n";
    std::cout << "Messages   : " << NUM_MESSAGES << '\n';
    std::cout << "Time       : " << duration.count() << " ms\n";
    std::cout << "Throughput : " << throughputM << " Mil msg/sec\n\n";
}

void CheckValue(int value, int expected) 
{
    if (value != expected) 
    {
        std::lock_guard<std::mutex> lock(printMutex);
        std::cerr << "Error: Expected " 
                    << expected 
                    << ", but got " 
                    << value 
                    << std::endl;
        ++mismatchCount;
    }
}

int main()
{
    RunBenchmark<SPSCQueue>("Lock-Free SPSC Queue");
    RunBenchmark<MutexQueue>("Mutex Queue");

    return 0;
}