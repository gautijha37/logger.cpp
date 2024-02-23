#include "logger.h"
#include <chrono>
#include <fstream>
#include <string>

int main()
{
	constexpr unsigned RUNS = 33333;
	static constexpr auto benchmark_file_path =
		"/Users/gautam/Documents/vscode/logger.cpp/benchmarks";
	std::fstream file(benchmark_file_path, std::ios::out | std::ios::app);

	auto start = std::chrono::high_resolution_clock::now();
	auto finish1 = start;
	{
		my_logger logger;

		for(size_t i = 0; i < RUNS; i++)
		{
			logger.info("this is a %s string with id: %u and double: %f",
						"sundar",
						i,
						static_cast<double>(i + 0.5));
			// logger.warn("this is a %s string with id: %u and double: %f",
			// 			"WARN",
			// 			2 * i,
			// 			static_cast<double>(2 * i));
			// logger.error("this is a %s string with id: %u and double: %f",
			// 			 "ERROR",
			// 			 4 * i,
			// 			 static_cast<double>(4 * i));
		}
		finish1 = std::chrono::high_resolution_clock::now();
	}
	auto finish = std::chrono::high_resolution_clock::now();
	auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
	auto time1_finish =
		std::chrono::duration_cast<std::chrono::milliseconds>(finish1 - start).count();
	std::cout << "Time taken multithreaded logger: " << time1 << " ms\n";
	file << "RUNS: " << 3 * RUNS << " TIME_MULTITHREADED_PRINTING: " << time1_finish << " ms\n";
	file << "RUNS: " << 3 * RUNS << " TIME_MULTITHREADED: " << time1 << " ms\n";

	start = std::chrono::high_resolution_clock::now();
	{
		char time_buf[19];
		for(size_t i = 0; i < RUNS; i++)
		{
			TIME::get_time(time_buf);
			fprintf(stdout,
					"%s"
					" [INFO] "
					"this is a %s string with id: %zu and double: %f\n",
					time_buf,
					"sundar",
					i,
					static_cast<double>(i + 0.5));
			// logger.warn("this is a %s string with id: %u and double: %f",
			// 			"WARN",
			// 			2 * i,
			// 			static_cast<double>(2 * i));
			// logger.error("this is a %s string with id: %u and double: %f",
			// 			 "ERROR",
			// 			 4 * i,
			// 			 static_cast<double>(4 * i));
		}
	}
	finish = std::chrono::high_resolution_clock::now();
	auto time2 = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
	std::cout << "Time taken multithreaded logger: " << time2 << " ms\n";

	file << "RUNS: " << 3 * RUNS << " TIME_PLAIN_LOGGER: " << time2 << " ms\n";
	return 0;
}