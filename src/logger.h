#pragma once

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <stdio.h>
#include <string>
#include <string_view>
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
	static constexpr size_t MESSAGE_PRINT_THRESHOLD = 450;
	static_assert(MESSAGE_PRINT_THRESHOLD < BUFFER_SIZE,
				  "Message print threshold must be smaller than msg buffer size");

	std::queue<std::unique_ptr<char[]>> m_to_print;

	std::mutex mu_buffer;
	std::condition_variable m_consumer_cond_var;
	std::condition_variable m_producer_cond_var;
	bool m_continue_logging;

	std::thread m_printing_thread;
	// std::atomic_int m_log_cnt;

public:
	my_logger();
	~my_logger();

	void logging_thread();

	template <typename... Args>
	void log(const char* format, const char msg_type[], const size_t N, Args... args)
	{
		std::unique_ptr<char[]> buf_ptr(new char[MESSAGE_BUFFER_SIZE]);
		TIME::get_time(buf_ptr.get());
		std::snprintf(buf_ptr.get() + TIME_BUF_SIZE - 1, N + 1, "%s", msg_type);
		std::snprintf(buf_ptr.get() + TIME_BUF_SIZE + N - 1,
					  MESSAGE_BUFFER_SIZE - N - TIME_BUF_SIZE + 1,
					  format,
					  std::forward<Args>(args)...);

		// std::string_view printing_str{buf_ptr};
		std::unique_lock lg(mu_buffer);
		// m_consumer_cond_var.wait(lg, [] { return true; });
		m_to_print.emplace(std::move(buf_ptr));

		if(m_to_print.size() > MESSAGE_PRINT_THRESHOLD)
		{
			lg.unlock();
			m_consumer_cond_var.notify_one();
		}
		// m_producer_cond_var.wait(lg, [this] { return msg_count() <= MESSAGE_PRINT_THRESHOLD; });
	}

	template <typename... Args>
	void info(const char* format, Args... args)
	{
		log(format, " [INFO] ", 8, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void warn(const char* format, Args... args)
	{
		log(format, " [WARN] ", 8, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void error(const char* format, Args... args)
	{
		log(format, " [ERROR] ", 9, std::forward<Args>(args)...);
	}
};