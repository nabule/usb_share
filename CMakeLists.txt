cmake_minimum_required(VERSION 3.16)

project(usb_share LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Test)

# Core Library
add_library(core_lib
    src/core/UsbDeviceManager.h
    src/core/UsbDeviceManager.cpp
    src/core/WindowsUsbDeviceManager.h
    src/core/WindowsUsbDeviceManager.cpp
)

target_link_libraries(core_lib PRIVATE Qt6::Core Qt6::Network)

# Define mock/test build logic
enable_testing()

add_executable(test_usb_export
    tests/test_usb_export.cpp
)

target_link_libraries(test_usb_export PRIVATE core_lib Qt6::Test)

add_test(NAME test_usb_export COMMAND test_usb_export)
