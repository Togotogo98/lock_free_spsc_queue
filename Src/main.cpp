#include "SPSCQueue.h"
#include <iostream>
#include <thread>


int main()
{
    SPSCQueue queue(5);

    std::thread producer([&queue]() {
        for (int i = 0; i < 10; ++i) {
            while (queue.Full() || !queue.Push(i)) {
                // Queue is full, wait till its false.
            }
            std::cout << "Produced: " << i << std::endl;
        }
    });

    std::thread consumer([&queue]() {
        for (int i = 0; i < 10; ++i) {
            int value;
            while (queue.Empty() || !queue.Pop(value)) 
            {
               // Queue is empty, wait till its false.
            }
            std::cout << "Consumed: " << value << std::endl;
        }
    });

    producer.join();
    consumer.join();

    return 0;
}