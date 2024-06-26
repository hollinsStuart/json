cmake_minimum_required(VERSION 3.27)
project(json)

set(CMAKE_CXX_STANDARD 20)

# Include FetchContent module to download Google Test
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Download and add Google Test
FetchContent_MakeAvailable(googletest)

# Include the directory containing the header file
include_directories(include)

# Create the test executable
add_executable(JSONTests
        tests/json_easy_test.cpp
        tests/json_tests.cpp
        tests/json_parsing_tests.cpp
        tests/json_serialization_tests.cpp
        tests/json_parsing_tests.cpp
        tests/json_serialization_tests.cpp
        tests/json_tests.cpp
        tests/json_pretty_print_test.cpp
        tests/json_list_test.cpp
)

add_executable(JSONProject
        src/main.cpp
)

# Link Google Test libraries
target_link_libraries(JSONTests gtest gtest_main pthread)

# Enable testing and add tests
enable_testing()
add_test(NAME JSONTests COMMAND JSONTests)

# Link Google Test libraries
target_link_libraries(JSONTests gtest gtest_main pthread)

# Find Google Benchmark
find_package(benchmark REQUIRED)

# Add the benchmark executable
add_executable(JSONBenchmark benchmarks/json_benchmarks.cpp)

# Link Google Benchmark libraries
target_link_libraries(JSONBenchmark benchmark::benchmark benchmark::benchmark_main pthread)

