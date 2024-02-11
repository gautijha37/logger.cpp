#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<deque>
#include<mutex>
#include<chrono>
#include<sstream>
#include <utility>


// void test();
std::string get_time();

#define LOG(_type, _fmt, _args...) \
    do{                     \
        fprintf(stdout, "[%s] " _fmt "\n", get_time(), ##_args);  \
    } while (0)                 \

class my_logger{
    enum class log_type: uint8_t{
        INFO,
        ERROR,
        WARN,
        ABORT
    };

    std::deque<std::pair<std::string, log_type> > m_lines;
    std::mutex mu_lines;
public:
    my_logger();
    template<typename string_type>
    void info(string_type input);

    template<typename string_type>
    void error(string_type input);

    template<typename string_type>
    void warn(string_type input);

    template<typename string_type>
    void abort(string_type input);
};