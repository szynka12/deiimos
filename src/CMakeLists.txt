# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Project's name
project(hello)

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}")

add_executable(hello ${PROJECT_SOURCE_DIR}/main.cpp)