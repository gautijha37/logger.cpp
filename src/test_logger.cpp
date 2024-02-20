#include "logger.h"
#include <chrono>
#include <fstream>

int main()
{
    char buf[10];
    constexpr unsigned RUNS = 33333;
    // fprintf(stdout, "str: %s", get_time());
    // fprintf(stdout, "str: %s", get_time());
    auto start = std::chrono::high_resolution_clock::now();
    my_logger logger;

    for (size_t i = 0; i < RUNS; i++)
    {
        // logger.info("this is a %s string with id: %u and double: %f", "INFO", i, static_cast<double>(i));
        // LOG_INFO("this is a %s string with id: %u and double: %f", "INFO", i, static_cast<double>(i));
        // LOG_WARN("this is a %s string with id: %u and double: %f", "WARN", 2 * i, static_cast<double>(2 * i));
        // LOG_ERROR("this is a %s string with id: %u and double: %f", "ERROR", 4 * i, static_cast<double>(4 * i));
    }
    auto finish = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
    std::cout << "Time taken: " << time << " us\n";

    static constexpr auto benchmark_file_path = "benchmarks";
    std::fstream file(benchmark_file_path, std::ios::out | std::ios::app);
    file << "RUNS: " << 3 * RUNS << " TIME: " << time << " us\n";
    return 0;
}