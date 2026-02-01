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
    src/core/NetworkTransport.h
    src/core/NetworkTransport.cpp
)

target_link_libraries(core_lib PRIVATE Qt6::Core Qt6::Network)

# Define mock/test build logic
add_executable(usb_share
    src/main.cpp
    src/gui/MainWindow.h
    src/gui/MainWindow.cpp
)
target_link_libraries(usb_share PRIVATE core_lib Qt6::Widgets)

enable_testing()

add_executable(test_usb_export
    tests/test_usb_export.cpp
)
target_link_libraries(test_usb_export PRIVATE core_lib Qt6::Test)
add_test(NAME test_usb_export COMMAND test_usb_export)

add_executable(test_network_transport
    tests/test_network_transport.cpp
)
target_link_libraries(test_network_transport PRIVATE core_lib Qt6::Test)
add_test(NAME test_network_transport COMMAND test_network_transport)

add_executable(test_gui
    tests/test_gui.cpp
    src/gui/MainWindow.h
    src/gui/MainWindow.cpp
)
target_link_libraries(test_gui PRIVATE core_lib Qt6::Test Qt6::Widgets)
add_test(NAME test_gui COMMAND test_gui)
