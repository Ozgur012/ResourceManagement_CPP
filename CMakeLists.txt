cmake_minimum_required(VERSION 4.0)
project(ResourceManagementCPP LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(ResourceCommon)
add_subdirectory(ResourcePacker)
add_subdirectory(ResourceLoader)
add_subdirectory(Example_Raylib)