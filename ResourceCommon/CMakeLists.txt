cmake_minimum_required(VERSION 3.30)
project(ResourceCommon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library(${PROJECT_NAME} STATIC
    src/rm_config.cpp
    src/error_checker_environment.cpp
    src/error_checker_packing.cpp
    src/error_checker_utils.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
