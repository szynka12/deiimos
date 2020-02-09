# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Project's name
project(hello)

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# set(HEADER_FILES "${PROJECT_SOURCE_DIR}/XMLHandler.h")
set(SOURCES "${PROJECT_SOURCE_DIR}/XMLHandler.cpp")

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}",  
                    "${PROJECT_SOURCE_DIR}/../external/src/tinyxml2")

add_executable(hello ${PROJECT_SOURCE_DIR}/main.cpp ${SOURCES})


find_library(TINYXML2 names tinyxml2
             paths "${PROJECT_SOURCE_DIR}/../external/lib/tinyxml2")

target_include_directories(hello PRIVATE "${PROJECT_SOURCE_DIR}")
target_link_libraries(hello ${TINYXML2})