#include "SPSCQueue.h"
#include "MutexQueue.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>

const size_t QUEUE_SIZE = 10000;
const int NUM_MESSAGES = 100000000;
std::mutex printMutex;
int mismatchCount = 0;

std::string ThroughputUnit(double& throughput)
{
    std::string unit = "msg/sec";

    if (throughput >= 1e9)
    {
        throughput /= 1e9;
        unit = "billion msg/sec";
    }
    else if (throughput >= 1e6)
    {
        throughput /= 1e6;
        unit = "million msg/sec";
    }
    else if (throughput >= 1e3)
    {
        throughput /= 1e3;
        unit = "thousand msg/sec";
    }

    return unit;
}

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
                std::cout << "Mismatches so far: " 
                            << mismatchCount << std::endl;
            }
            ++expected;
        }
    });

    producer.join();
    consumer.join();

    // Print the benchmark result
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    // std::cout << "Benchmark completed in " << duration.count() << " ms." << std::endl;

    double inSeconds = duration.count() / 1000000.0;
    double throughput =
        static_cast<double>(NUM_MESSAGES) / inSeconds;

    //double throughputM = throughput / 1000000.0;
    std::string unit = ThroughputUnit(throughput);

    std::cout << "=============================\n";
    std::cout << name << '\n';
    std::cout << "=============================\n";
    //std::cout << "Messages   : " << NUM_MESSAGES << '\n';
    std::cout << "Time       : " << inSeconds << " seconds\n";
    std::cout << "Throughput : " << throughput << " " << unit << "\n\n";
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
    std::cout << "=============================\n";
    std::cout << "      Benchmark Results\n";
    std::cout << "=============================\n";
    std::cout << "Queue Size: " << QUEUE_SIZE << "\n";
    std::cout << "Number of Messages: " << NUM_MESSAGES << "\n";

    RunBenchmark<SPSCQueue>("Lock-Free SPSC Queue");
    RunBenchmark<MutexQueue>("Mutex Queue");

    return 0;
}