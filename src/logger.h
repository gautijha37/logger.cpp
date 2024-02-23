#pragma once

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <thread>
#include <utility>
#include <vector>

namespace TIME
{
void get_time(char*);
} // namespace TIME

class my_logger
{
	static constexpr size_t TIME_BUF_SIZE = 19;
	static constexpr size_t MESSAGE_BUFFER_SIZE = 300;
	static constexpr size_t BUFFER_SIZE = 500;
	// static constexpr size_t MESSAGE_PRINT_THRESHOLD = 50;
	static constexpr std::chrono::microseconds LOGGING_DELAY = std::chrono::microseconds{50};
	// std::deque<std::pair<std::string, log_type>> m_lines;
	// std::mutex mu_lines;
	std::array<char, TIME_BUF_SIZE> m_time_buf;
	std::vector<std::array<char, MESSAGE_BUFFER_SIZE>> m_to_print;
	size_t m_head;
	size_t m_tail;

	size_t m_lost_msg_count;

	std::mutex mu_buffer;
	std::condition_variable m_cond_var;
	std::atomic<bool> m_continue_logging;

	std::thread m_printing_thread;

public:
	my_logger();
	~my_logger();

	void logging_thread();

	template <typename... Args>
	void log(const char* format, const char msg_type[], Args... args)
	{
		std::unique_lock lg(mu_buffer);
		m_cond_var.wait(lg, [this] { return (m_tail + 1) % BUFFER_SIZE != m_head; });

		auto buf_ptr = m_to_print[m_tail].data();
		const size_t N = std::strlen(msg_type);
		TIME::get_time(buf_ptr);
		std::snprintf(buf_ptr + TIME_BUF_SIZE - 1, N + 1, "%s", msg_type);
		std::snprintf(buf_ptr + TIME_BUF_SIZE + N - 1,
					  MESSAGE_BUFFER_SIZE - N - TIME_BUF_SIZE + 1,
					  format,
					  std::forward<Args>(args)...);
		m_tail = (m_tail + 1) % BUFFER_SIZE;

		lg.unlock();
		m_cond_var.notify_one();
	}

	template <typename... Args>
	void info(const char* format, Args... args)
	{
		log(format, " [INFO] ", std::forward<Args>(args)...);
	}

	// template <typename... Args>
	// void log(const char* format, Args... args)
	// {
	// 	std::unique_lock lg(mu_buffer);
	// 	m_cond_var.wait(lg, [this] { return (m_tail + 1) % BUFFER_SIZE != m_head; });

	// 	auto buf_ptr = m_to_print[m_tail].data();
	// 	const size_t N = std::strlen(" [INFO] ");
	// 	TIME::get_time(buf_ptr);
	// 	std::snprintf(buf_ptr + TIME_BUF_SIZE - 1, N + 1, "%s", " [INFO] ");
	// 	std::snprintf(buf_ptr + TIME_BUF_SIZE + N - 1,
	// 				  MESSAGE_BUFFER_SIZE - N - TIME_BUF_SIZE + 1,
	// 				  format,
	// 				  std::forward<Args>(args)...);
	// 	m_tail = (m_tail + 1) % BUFFER_SIZE;
	// 	lg.unlock();
	// 	m_cond_var.notify_one();
	// }

	template <typename... Args>
	void warn(const char* format, Args... args)
	{
		log(format, " [WARN] ", std::forward<Args>(args)...);
	}

	template <typename... Args>
	void error(const char* format, Args... args)
	{
		log(format, " [ERROR] ", std::forward<Args>(args)...);
	}

	template <typename... Args>
	void abort(const char* format, Args... args)
	{
		log(format, " [ABORT] ", std::forward<Args>(args)...);
	}
};