# Lock-Free Single Producer Single Consumer Queue

An implementation of a bounded **lock-free Single Producer Single Consumer (SPSC) queue** using a circular ring buffer and atomic operations. The project also includes a mutex-based queue implementation to compare performance between lock-free and traditional synchronization techniques.


## Build

Compile using g++:

```bash
g++ Src/main.cpp Src/SPSCQueue.cpp Src/MutexQueue.cpp -I Include -pthread -o Build/spsc_queue
```

Run:

```bash
./Build/spsc_queue
```

## Sample Output

```text
=============================
      Benchmark Results
=============================
Queue Size: 10000
Number of Messages: 100000000

=============================
Lock-Free SPSC Queue
=============================
Time       : 7.54 seconds
Throughput : 13.26 million msg/sec

=============================
Mutex Queue
=============================
Time       : 28.48 seconds
Throughput : 3.51 million msg/sec
```


## Project Phases

### Phase 1 – Ring Buffer Implementation

* Implemented a bounded circular queue
* Added `Push()`, `Pop()`, `Empty()`, and `Full()`
* Verified FIFO ordering

### Phase 2 – Lock-Free Queue

* Replaced indices with `std::atomic`
* Implemented producer and consumer threads
* Applied Acquire/Release memory ordering
* Removed all mutexes and condition variables

### Phase 3 – Benchmarking and Comparison

* Implemented a mutex-based queue
* Created a generic benchmark using C++ templates
* Measured execution time and throughput
* Compared lock-free and mutex-based performance

## Benchmark Summary

The lock-free implementation achieved approximately **3.8× higher throughput** than the mutex-based queue during benchmarking.
