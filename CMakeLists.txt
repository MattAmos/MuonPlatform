cmake_minimum_required(VERSION 2.8)

project(MuonPlatform)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_PROJECT_DIR}/cmake/Modules")

find_package(OpenCV REQUIRED)

INCLUDE_DIRECTORIES(SYSTEM ${OpenCV_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-a53")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

#add_compile_options(-mcpu=cortex-a53)
add_compile_options(-std=c++14)
add_compile_options(-fdiagnostics-color=always)

# Build source files
add_executable( Runner 			src/Runner.cpp )

# Build test files
add_executable( GPIO_test 		    test/GPIO_test.cpp )
add_executable( Controller 		    test/Controller.cpp )
add_executable( ServoKeyboard       test/ServoKeyboard.cpp )
add_executable( ServoController     test/ServoController.cpp )
add_executable( WiringPiController  test/WiringPiController.cpp )

# Link External Library Dependencies
target_link_libraries( Runner ${OpenCV_LIBRARIES} )
target_link_libraries( ServoKeyboard -lcurses)
target_link_libraries( GPIO_test -lcurses)
target_link_libraries( WiringPiController -lcurses -lwiringPi -lpthread -lrt -lcrypt)
