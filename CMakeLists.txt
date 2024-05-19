cmake_minimum_required(VERSION 3.28)
project(json)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(.)

add_executable(json
        json.hpp)

# Add the test executable
add_executable(JSONTests tests/easytest.cpp)

# Link Google Test libraries
target_link_libraries(JSONTests ${GTEST_LIBRARIES} pthread)

# Add a test for CTest (optional)
add_test(NAME JSONTests COMMAND JSONTests)
