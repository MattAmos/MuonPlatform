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

# Build independent source files
add_executable( Runner 			 src/Runner.cpp)

# Build independent test files
# add_executable( GPIOTest 		 test/GPIOTest.cpp)
# add_executable( Controller 		 test/ControllerTest.cpp )
# add_executable( ServoKeyboard    test/ServoKeyboardTest.cpp )
# add_executable( ServoController  test/ServoControllerTest.cpp )
# add_executable( DCMotorTest      test/DCMotorTest.cpp)
 add_executable( SonicTest test/SonicTest.cpp)
# add_executable( SenseTest test/Sensor.cpp)
# add_executable(RangeTest test/RangeTest.cpp)
# add_executable(GyroTest test/Gyrotest.cpp)
# add_executable( I2CTest	test/I2CTest.cpp)
add_executable( CVTest1	test/CVTest1.cpp)

# Link External Library Dependencies
target_link_libraries( Runner ${OpenCV_LIBRARIES} -lcurses -lpthread -lwiringPi ~/MuonPlatform/libVL53L0X_Rasp.a  )
target_link_libraries( CVTest1 ${OpenCV_LIBRARIES} )
# target_link_libraries( ServoKeyboard -lcurses)
# target_link_libraries( GPIOTest -lcurses)
# target_link_libraries( DCMotorTest -lcurses)
# target_link_libraries( I2CTest -lwiringPi)
 target_link_libraries(SonicTest	-lwiringPi)
# target_link_libraries(SenseTest /home/pi/MuonPlatform/libVL53L0X_Rasp.a)
# target_link_libraries(RangeTest /home/pi/MuonPlatform/libVL53L0X_Rasp.a)
#target_link_libraries(GyroTest -lwiringPi)
