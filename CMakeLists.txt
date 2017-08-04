cmake_minimum_required(VERSION 2.8)

project(MuonPlatform)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_PROJECT_DIR}/cmake/Modules")

find_package(OpenCV REQUIRED)

INCLUDE_DIRECTORIES(SYSTEM ${OpenCV_INCLUDE_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(WITH_TBB ON)

add_compile_options(-std=c++14)
add_compile_options(-march=native)
add_compile_options(-mtune=native)
add_compile_options(-fdiagnostics-color=always)

add_executable( Runner src/Runner.cpp )

target_link_libraries( Runner ${OpenCV_LIBRARIES} )
