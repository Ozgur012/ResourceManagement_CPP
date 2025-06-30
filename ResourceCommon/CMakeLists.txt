cmake_minimum_required(VERSION 4.0)
project(ResourceCommon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library(${PROJECT_NAME} STATIC
    src/error_checker_environment.cpp
    src/error_checker_packing.cpp
    src/error_checker_utils.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE "F:/Engines/nlohman_json-3.12.0/single_include")
