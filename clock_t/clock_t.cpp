// clock_t.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <time.h>
#include <vector>
#include <stdint.h>

#include <chrono>

#ifdef WIN32
#include "windows.h"
uint64_t TimerResolution() {
    uint64_t frequency;
    QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
    return frequency;
}

uint64_t SystemTimer() {
    uint64_t counter;
    QueryPerformanceCounter((LARGE_INTEGER*)&counter);
    return counter;
}
#else
#include "sys/time.h"
uint64_t TimerResolution() {
    return (uint64_t)1000000000;    // nanoseconds
}

uint64_t SystemTimer() {
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (uint64_t)ts.tv_nsec;
}
#endif

uint64_t fibonacci(std::vector<uint64_t>& values, uint32_t count) {
    values[0] = 0;
    values[1] = 1;
    for (uint32_t i = 2; i < count; i++) {
        values[i] = values[i - 1] + values[i - 2];
    }

    return values.back();
}

int main(int argc, char* argv[])
{
    uint32_t count = 1000000;
    std::vector<uint64_t> values(count);

    clock_t start = clock();

    uint64_t fib = fibonacci(values, count);

    clock_t end = clock();

    printf("clock()\n");
    printf("Fibonacci[%d] = %llu\n", count, values.back());
    printf("Start: %llu\n", uint64_t(start));
    printf("End:   %llu\n", uint64_t(end));
    printf("Resolution: %llu\n", uint64_t(CLOCKS_PER_SEC));
    double elapsed = uint64_t(end - start) * 1000 / double(CLOCKS_PER_SEC);
    printf("Elapsed milliseconds: %lf\n", elapsed);
    printf("\n");

    auto t1 = std::chrono::high_resolution_clock::now();

    fib = fibonacci(values, count);

    auto t2 = std::chrono::high_resolution_clock::now();

    printf("std::chrono\n");
    printf("Fibonacci[%d] = %llu\n", count, values.back());
    printf("Start: %llu\n", uint64_t(0));
    printf("End:   %llu\n", uint64_t((t2 - t1).count()));
    printf("Resolution: %llu / %llu seconds\n", (uint64_t)std::chrono::high_resolution_clock::period::den, (uint64_t)std::chrono::high_resolution_clock::period::num);
    elapsed = uint64_t((t2 - t1).count()) * 1000 * double(std::chrono::high_resolution_clock::period::num) / double(std::chrono::high_resolution_clock::period::den);
    printf("Elapsed milliseconds: %lf\n", elapsed);
    printf("\n");

    uint64_t count1 = SystemTimer();

    fib = fibonacci(values, count);

    uint64_t count2 = SystemTimer();

    printf("SystemTimer\n");
    printf("Fibonacci[%d] = %llu\n", count, values.back());
    printf("Start: %llu\n", count1);
    printf("End:   %llu\n", count2);
    printf("Resolution: %llu\n", TimerResolution());
    elapsed = (count2 - count1) * 1000 / double(TimerResolution());
    printf("Elapsed milliseconds: %lf\n", elapsed);
    printf("\n");

    return 0;
}

