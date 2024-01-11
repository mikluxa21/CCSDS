cmake_minimum_required(VERSION 3.16.3)
project(Tests)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(Test tests.cpp)
target_link_libraries(Test
    gtest gtest_main
    pthread
    )
