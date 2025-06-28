cmake_minimum_required(VERSION 4.0)
project(ResourceLoader)

add_library(${PROJECT_NAME} src/resource_loader.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)