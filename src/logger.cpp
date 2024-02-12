#include "logger.h"

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