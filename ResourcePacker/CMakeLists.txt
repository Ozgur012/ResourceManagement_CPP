cmake_minimum_required(VERSION 4.0)
project(ResourcePacker)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(${PROJECT_NAME} 
    src/command_line_interface.cpp
    src/environment_validator.cpp
    src/error_checker_environment.cpp
    src/error_checker_packaging.cpp
    src/error_checker_utils.cpp
    src/pack_maker.cpp
    src/resource_packer.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE _internal)
target_link_libraries(${PROJECT_NAME} PRIVATE ResourceLoader)
