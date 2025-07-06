cmake_minimum_required(VERSION 3.30)
project(ResourceLoader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


add_library(${PROJECT_NAME} src/resource_loader.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PRIVATE ResourceCommon)