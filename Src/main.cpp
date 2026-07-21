#include "SPSCQueue.h"
#include <iostream>


int main()
{
    SPSCQueue queue(8);

    std::cout << queue.Empty() << std::endl;   // Expected: 1 (true)
    std::cout << queue.Full() << std::endl;    // Expected: 0 (false)

    queue.Push(10);
    queue.Push(20);
    queue.Push(30);

    int value;

    queue.Pop(value);
    std::cout << value << std::endl;

    queue.Pop(value);
    std::cout << value << std::endl;

    queue.Pop(value);
    std::cout << value << std::endl;

    /* check if queue is empty */
    std::cout << queue.Empty() << std::endl;   // Expected: 1 (true)

    return 0;
}