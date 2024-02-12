#include "logger.h"
#include <chrono>

int main()
{
    char buf[10];
    constexpr int RUNS = 33333;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < RUNS; i++)
    {
        LOG_INFO("this is a %s string with id: %d and double: %f", "INFO", i, static_cast<double>(i));
        LOG_WARN("this is a %s string with id: %d and double: %f", "WARN", 2 * i, static_cast<double>(2 * i));
        LOG_ERROR("this is a %s string with id: %d and double: %f", "ERROR", 4 * i, static_cast<double>(4 * i));
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << " ns\n";
    return 0;
}