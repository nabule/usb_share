cmake_minimum_required(VERSION 3.16)

project(usb_share LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Ensure Qt6 is found
find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets Test)

# Core Library
add_library(core_lib STATIC
    src/core/UsbDeviceManager.h
    src/core/UsbDeviceManager.cpp
    src/core/WindowsUsbDeviceManager.h
    src/core/WindowsUsbDeviceManager.cpp
    src/core/NetworkTransport.h
    src/core/NetworkTransport.cpp
    src/core/UdpBroadcaster.h
    src/core/UdpBroadcaster.cpp
    src/core/UdpListener.h
    src/core/UdpListener.cpp
)

# Use PUBLIC to propagate include directories and libraries to dependents
target_link_libraries(core_lib PUBLIC Qt6::Core Qt6::Network)
target_include_directories(core_lib PUBLIC src/core)

# Main Executable
add_executable(usb_share WIN32
    src/main.cpp
    src/gui/MainWindow.h
    src/gui/MainWindow.cpp
)
# Note: linking core_lib should bring in Qt6::Network headers
target_link_libraries(usb_share PRIVATE core_lib Qt6::Widgets)
target_include_directories(usb_share PRIVATE src/gui)

# Tests
enable_testing()

# Function to add tests
function(add_qt_test test_name source_file)
    add_executable(${test_name} ${source_file})
    # Explicitly link Qt6::Network just in case propagation is failing
    target_link_libraries(${test_name} PRIVATE core_lib Qt6::Test Qt6::Network)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_qt_test(test_usb_export tests/test_usb_export.cpp)
add_qt_test(test_network_transport tests/test_network_transport.cpp)
add_qt_test(test_reproduce_issue tests/test_reproduce_issue.cpp)
add_qt_test(test_udp_discovery tests/test_udp_discovery.cpp)

# Manual GUI test
add_executable(test_gui
    tests/test_gui.cpp
    src/gui/MainWindow.h
    src/gui/MainWindow.cpp
)
target_link_libraries(test_gui PRIVATE core_lib Qt6::Test Qt6::Widgets Qt6::Network)
target_include_directories(test_gui PRIVATE src/gui)
add_test(NAME test_gui COMMAND test_gui)
