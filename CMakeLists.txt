cmake_minimum_required(VERSION 4.0)
project(ResourceManagementCPP LANGUAGES C CXX)

#include_directories("F:/Engines/nlohman_json-3.12.0/single_include")

add_library(raylib STATIC IMPORTED)
set_target_properties(
    raylib PROPERTIES
    IMPORTED_LOCATION "F:/Engines/raylib/build_a/raylib/Debug/libraylib.a"
    INTERFACE_INCLUDE_DIRECTORIES "F:/Engines/raylib/build_a/raylib/include"
    INTERFACE_LINK_LIBRARIES winmm
)

add_subdirectory(ResourceCommon)
add_subdirectory(ResourcePacker)
add_subdirectory(ResourceLoader)
add_subdirectory(Example_Raylib)