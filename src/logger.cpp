#include "logger.h"
#include <algorithm>
#include <mutex>

namespace TIME
{
void get_time(char* buf)
{
	auto currentTime = std::chrono::high_resolution_clock::now();
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
	: m_head(0)
	, m_tail(0)
	, m_lost_msg_count(0)
	, m_continue_logging(true)
{
	m_to_print.assign(BUFFER_SIZE, {});
	m_printing_thread = std::thread(&my_logger::logging_thread, this);
}

void my_logger::logging_thread()
{
	// auto curr_time = std::chrono::steady_clock::now();
	// auto next_start_time = curr_time + LOGGING_DELAY;
	while(m_continue_logging)
	{
		std::unique_lock lg(mu_buffer);
		m_cond_var.wait(lg, [this] { return m_head != m_tail; });
		while(m_head != m_tail)
		{
			std::printf("%s\n", m_to_print[m_head].data());
			m_head = (m_head + 1) % BUFFER_SIZE;
		}

		lg.unlock();
		m_cond_var.notify_one();
	}

	// std::this_thread::sleep_until(next_start_time);
}

my_logger::~my_logger()
{
	m_continue_logging = false;
	if(m_printing_thread.joinable())
		m_printing_thread.join();
	// std::cerr << "Lost " << m_lost_msg_count << " messages\n";
}
// template <typename string_type>
// void my_logger::warn(string_type input)
// {
// 	// std::lock_guard<std::mutex> lg(mu_lines);
// 	// m_lines.push_back(std::make_pair(std::forward<string_type>(input), my_logger::log_type::WARN));
// }