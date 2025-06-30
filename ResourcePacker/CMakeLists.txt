cmake_minimum_required(VERSION 4.0)
project(ResourcePacker)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(${PROJECT_NAME} 
    src/command_line_interface.cpp
    src/environment_validator.cpp
    src/pack_maker.cpp
    src/resource_packer.cpp
)

target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
)


target_include_directories(${PROJECT_NAME} PRIVATE _internal)
target_include_directories(${PROJECT_NAME} PRIVATE "F:/Engines/nlohman_json-3.12.0/single_include")

target_link_libraries(${PROJECT_NAME} PRIVATE ResourceCommon)
target_link_libraries(${PROJECT_NAME} PRIVATE ResourceLoader)
