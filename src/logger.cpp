#include "logger.h"

std::string get_time() {
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

void test_time() {
  timespec ts;
  // clock_gettime(CLOCK_MONOTONIC, &ts); // Works on FreeBSD
  clock_gettime(CLOCK_REALTIME, &ts);
  fprintf(stdout, "%ld.%.9ld", (long)ts.tv_sec, ts.tv_nsec);
}

template <typename string_type> void my_logger::info(string_type input) {
  std::lock_guard<std::mutex> lg(mu_lines);
  m_lines.emplace_back(std::forward<string_type>(input),
                       my_logger::log_type::INFO);
}

template <typename string_type> void my_logger::warn(string_type input) {
  std::lock_guard<std::mutex> lg(mu_lines);
  m_lines.push_back(std::make_pair(std::forward<string_type>(input),
                                   my_logger::log_type::WARN));
}