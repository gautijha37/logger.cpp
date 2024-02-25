cmake_minimum_required(VERSION 3.5)
project(Logger)

# SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")
# cmake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++
# set(DCMAKE_C_COMPILER "/usr/bin/gcc")
set(DCMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Add logger.cpp as a library
add_library(logger src/logger.cpp)
# Add the executable and link it with the logger library
add_executable(test_logger src/test_logger.cpp)
target_link_libraries(test_logger logger)
