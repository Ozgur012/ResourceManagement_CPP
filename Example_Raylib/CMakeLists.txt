cmake_minimum_required(VERSION 4.0)
project(ExampleRaylib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library(raylib STATIC IMPORTED)
set_target_properties(
    raylib PROPERTIES 
    IMPORTED_CONFIGURATIONS "DEBUG;RELEASE"
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/raylib/include
    INTERFACE_LINK_LIBRARIES winmm
)
 

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set_target_properties(
    raylib PROPERTIES
    IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/raylib/Debug/libraylib.a
    IMPORTED_LOCATION_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/raylib/Release/libraylib.a)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_target_properties(
    raylib PROPERTIES
    IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/raylib/Debug/libraylib.lib
    IMPORTED_LOCATION_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/raylib/Release/libraylib.lib)
endif()


add_executable(${PROJECT_NAME} src/example_raylib.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib ResourceLoader)