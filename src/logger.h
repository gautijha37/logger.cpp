#pragma once

#include <chrono>
#include <ctime>
#include <deque>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <sys/time.h>
#include <utility>
#include <vector>

namespace
{
    std::string get_time()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto nanoSeconds =
            std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime);

        auto nanoSecondsCount = nanoSeconds.time_since_epoch().count();

        // Convert nanoseconds to seconds and fractional seconds
        auto seconds = nanoSecondsCount / 1'000'000'000;
        auto fracSeconds = nanoSecondsCount % 1'000'000'000;

        // Convert seconds to std::time_t
        std::time_t time =
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        // Convert time to struct tm in local time zone
        std::tm *tm = std::localtime(&time);

        // Print the time in HH:MM:SS.ns format
        std::ostringstream oss;
        oss << std::put_time(tm, "%H:%M:%S") << "." << std::setw(9)
            << std::setfill('0') << fracSeconds;

        return oss.str();
    }
} // namespace

#define LOG(_type, _fmt, _args...) \
    fprintf(stdout, "[%s] [%s] " _fmt "\n", ::get_time().c_str(), _type, ##_args);

#define LOG_INFO(_fmt, _args...) LOG("INFO", _fmt, ##_args)
#define LOG_WARN(_fmt, _args...) LOG("WARN", _fmt, ##_args)
#define LOG_ERROR(_fmt, _args...) LOG("ERROR", _fmt, ##_args)
#define LOG_ABORT(_fmt, _args...) LOG("ABORT", _fmt, ##_args)

class my_logger
{
    enum class log_type : uint8_t
    {
        INFO,
        ERROR,
        WARN,
        ABORT
    };

    std::deque<std::pair<std::string, log_type>> m_lines;
    std::mutex mu_lines;

public:
    my_logger();
    template <typename string_type>
    void info(string_type input);

    template <typename string_type>
    void error(string_type input);

    template <typename string_type>
    void warn(string_type input);

    template <typename string_type>
    void abort(string_type input);
};