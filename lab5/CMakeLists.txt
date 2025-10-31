cmake_minimum_required(VERSION 4.0)
project(lab5)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)

message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")
message(STATUS "OpenCV include directories: ${OpenCV_INCLUDE_DIRS}")

add_executable(lab5 main.cpp)

target_link_libraries(lab5 ${OpenCV_LIBS})

target_include_directories(lab5 PRIVATE ${OpenCV_INCLUDE_DIRS})
