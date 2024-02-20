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
// #include <fmt/core.h>

namespace TIME
{
    void get_time();
} // namespace TIME

class my_logger
{
    enum class log_type : uint8_t
    {
        INFO,
        ERROR,
        WARN,
        ABORT
    };

    static constexpr size_t TIME_BUF_SIZE = 19;
    // static constexpr LOGGING_DELAY
    std::deque<std::pair<std::string, log_type>> m_lines;
    std::mutex mu_lines;
    char *m_buf;
    std::vector<const char*> m_to_print;

    #define LOG(_type, _fmt, _args...) \
    TIME::get_time(); \
    // m_to_print.emplace_back("[%s] [%s] " _fmt "\n", m_buf, _type, ##_args); 

    #define LOG_INFO(_fmt, _args...) LOG("INFO", _fmt, ##_args)
    #define LOG_WARN(_fmt, _args...) LOG("WARN", _fmt, ##_args)
    #define LOG_ERROR(_fmt, _args...) LOG("ERROR", _fmt, ##_args)
    #define LOG_ABORT(_fmt, _args...) LOG("ABORT", _fmt, ##_args)

public:
    my_logger();
    template <typename... Args> 
    void info(const char* format, Args... args);

    template <typename string_type>
    void error(string_type input);

    template <typename string_type>
    void warn(string_type input);

    template <typename string_type>
    void abort(string_type input);

    void print();
};