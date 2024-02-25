#include "logger.h"
#include <chrono>
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		std::fprintf(stderr, "Usage: logger <file>");
		return 1;
	}

	constexpr unsigned RUNS = 33333;
	constexpr bool ENABLE_MULTITHREADING = true;
	const std::string benchmark_file_path = argv[1];
	std::fstream file(benchmark_file_path, std::ios::out | std::ios::app);
	if(!file.is_open())
	{
		std::fprintf(stderr, "Couldn't open %s\n", benchmark_file_path.c_str());
		return 1;
	}

	auto start = std::chrono::system_clock::now();
	{
		my_logger logger;

		std::thread t1, t2;

		if(ENABLE_MULTITHREADING)
		{
			t1 = std::thread([&logger] {
				for(size_t i = 0; i < RUNS; i++)
				{
					logger.info("this is a %s string with id: %u and double: %f",
								"sundar",
								i,
								static_cast<double>(i + 0.5));
				}
			});

			t2 = std::thread([&logger] {
				for(size_t i = 0; i < RUNS; i++)
				{
					logger.warn("this is a %s string with id: %u and double: %f",
								"ssundar",
								i,
								static_cast<double>(i + 0.5));
				}
			});
		}

		for(size_t i = 0; i < RUNS; i++)
		{
			logger.info("this is a %s string with id: %u and double: %f",
						"sundar",
						i,
						static_cast<double>(i + 0.5));
			logger.warn("this is a %s string with id: %u and double: %f",
						"WARN",
						2 * i,
						static_cast<double>(2 * i));
			logger.error("this is a %s string with id: %u and double: %f",
						 "ERROR",
						 4 * i,
						 static_cast<double>(4 * i));
		}

		if(ENABLE_MULTITHREADING)
		{
			t1.join();
			t2.join();
		}

		// runs = logger.get_log_cnt();
	}
	auto finish = std::chrono::system_clock::now();
	auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
	file << "RUNS: " << 5 * RUNS << " TIME_MULTITHREADED: " << time1 << " ms\n";

	start = std::chrono::system_clock::now();
	{
		char time_buf[19];
		for(size_t i = 0; i < 5 * RUNS; i++)
		{
			TIME::get_time(time_buf);
			fprintf(stdout,
					"%s"
					" [INFO] "
					"this is a %s string with id: %zu and double: %f\n",
					time_buf,
					"atisundar",
					i,
					static_cast<double>(i + 0.5));

			// TIME::get_time(time_buf);
			// fprintf(stdout,
			// 		"%s"
			// 		" [WARN] "
			// 		"this is a %s string with id: %zu and double: %f\n",
			// 		time_buf,
			// 		"atisundar",
			// 		i,
			// 		static_cast<double>(2 * i));

			// TIME::get_time(time_buf);
			// fprintf(stdout,
			// 		"%s"
			// 		" [ERROR] "
			// 		"this is a %s string with id: %zu and double: %f\n",
			// 		time_buf,
			// 		"atisundar",
			// 		i,
			// 		static_cast<double>(4 * i));
		}
	}
	finish = std::chrono::system_clock::now();
	auto time2 = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();

	file << "RUNS: " << 5 * RUNS << " TIME_PLAIN_LOGGER: " << time2 << " ms\n";
	return 0;
}