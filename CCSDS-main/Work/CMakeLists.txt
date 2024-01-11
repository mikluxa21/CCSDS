cmake_minimum_required(VERSION 3.16.3)
cmake_policy(SET CMP0016 NEW)

project(CCSDS VERSION 1.0.0 DESCRIPTION "Project")

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(tests)
enable_testing()
