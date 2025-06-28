cmake_minimum_required(VERSION 4.0)
project(TestProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(${PROJECT_NAME} src/test_project.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib ResourceLoader)