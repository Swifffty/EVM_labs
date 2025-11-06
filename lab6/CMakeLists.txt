cmake_minimum_required(VERSION 4.0)
project(lab6)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

add_executable(lab6 main.cpp)

target_include_directories(lab6 PRIVATE ${LIBUSB_INCLUDE_DIRS})
target_link_libraries(lab6 ${LIBUSB_LIBRARIES})
