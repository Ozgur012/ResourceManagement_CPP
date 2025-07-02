cmake_minimum_required(VERSION 4.0)
project(ResourceCommon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library(${PROJECT_NAME} STATIC
    src/error_checker_environment.cpp
    src/error_checker_packing.cpp
    src/error_checker_utils.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
