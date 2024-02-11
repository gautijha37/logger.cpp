cmake_minimum_required(VERSION 3.0)
project(Logger)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
# Add logger.cpp as a library
add_library(logger logger.cpp)

# Add the executable and link it with the logger library
add_executable(test_logger test_logger.cpp)
target_link_libraries(test_logger logger)
