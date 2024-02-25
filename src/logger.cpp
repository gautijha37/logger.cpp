#include "logger.h"
#include <algorithm>
#include <mutex>

namespace TIME
{
void get_time(char* buf)
{
	auto currentTime = std::chrono::system_clock::now();
	auto nanoSeconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime);

	auto nanoSecondsCount = nanoSeconds.time_since_epoch().count();

	// Convert nanoseconds to seconds and fractional seconds
	auto fracSeconds = nanoSecondsCount % 1'000'000'000;

	// Convert seconds to std::time_t
	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	// Convert time to struct tm in local time zone
	std::tm* tm = std::localtime(&time);

	std::strftime(buf, 9, "%H:%M:%S.", std::localtime(&time));
	std::snprintf(buf + 9, 10, "%09lld", fracSeconds);
}
} // namespace TIME

my_logger::my_logger()
	: m_continue_logging(true)
{
	m_printing_thread = std::thread(&my_logger::logging_thread, this);
}

void my_logger::logging_thread()
{
	while(m_continue_logging)
	{
		std::unique_lock lg(mu_buffer);
		m_consumer_cond_var.wait(lg, [this] { return !m_to_print.empty() || !m_continue_logging; });
		while(!m_to_print.empty() && m_continue_logging)
		{
			std::printf("%s\n", m_to_print.front().get());
			m_to_print.pop();
		}

		// lg.unlock();
		// m_consumer_cond_var.notify_one();
		// m_producer_cond_var.notify_all(); //notify all producers
	}
}

my_logger::~my_logger()
{
	std::unique_lock ul(mu_buffer);
	m_continue_logging = false;
	ul.unlock();
	m_consumer_cond_var.notify_all();

	m_printing_thread.join();
	while(!m_to_print.empty())
	{
		std::printf("%s\n", m_to_print.front().get());
		m_to_print.pop();
	}
}