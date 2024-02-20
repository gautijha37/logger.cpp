#include "logger.h"

namespace TIME{
    void get_time(char **buf)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto nanoSeconds =
            std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime);

        auto nanoSecondsCount = nanoSeconds.time_since_epoch().count();

        // Convert nanoseconds to seconds and fractional seconds
        auto fracSeconds = nanoSecondsCount % 1'000'000'000;

        // Convert seconds to std::time_t
        std::time_t time =
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        // Convert time to struct tm in local time zone
        std::tm *tm = std::localtime(&time);

        std::strftime(*buf, 9, "%H:%M:%S.", std::localtime(&time));
        std::snprintf(*buf + 9, 10, "%09lld", fracSeconds);
    }
}

my_logger::my_logger()
{
    m_buf = new char[TIME_BUF_SIZE];
}

template <typename... Args> 
void my_logger::info(const char* format, Args... args) {
    // TIME::get_time();
    // m_to_print.emplace_back("[%s] [%s] " format "\n", m_buf, "INFO", _args...); 
//   std::lock_guard<std::mutex> lg(mu_lines);
//   m_lines.emplace_back(std::forward<string_type>(input),
                    //    my_logger::log_type::INFO);
}

template <typename string_type>
void my_logger::warn(string_type input) {
  std::lock_guard<std::mutex> lg(mu_lines);
  m_lines.push_back(std::make_pair(std::forward<string_type>(input),
                                   my_logger::log_type::WARN));
}