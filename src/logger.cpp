#include "logger.h"

std::string get_time(){
    std::cout << "in get_time()\n";
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto nanoSeconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime);

    std::ostringstream ss;
    ss << nanoSeconds.time_since_epoch().count();
    std::string timestampString = ss.str();
    std::cout << timestampString;

    return timestampString;
}

template<typename string_type>
void my_logger::info(string_type input){
    std::lock_guard<std::mutex> lg(mu_lines);
    m_lines.emplace_back(std::forward<string_type>(input), my_logger::log_type::INFO);
}

template<typename string_type>
void my_logger::warn(string_type input){
    std::lock_guard<std::mutex> lg(mu_lines);
    m_lines.push_back(std::make_pair(std::forward<string_type>(input), my_logger::log_type::WARN));
}