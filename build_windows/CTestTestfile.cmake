# CMake generated Testfile for 
# Source directory: Z:/code/usb_share
# Build directory: Z:/code/usb_share/build_windows
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test_usb_export "Z:/code/usb_share/build_windows/test_usb_export.exe")
set_tests_properties(test_usb_export PROPERTIES  _BACKTRACE_TRIPLES "Z:/code/usb_share/CMakeLists.txt;52;add_test;Z:/code/usb_share/CMakeLists.txt;55;add_qt_test;Z:/code/usb_share/CMakeLists.txt;0;")
add_test(test_network_transport "Z:/code/usb_share/build_windows/test_network_transport.exe")
set_tests_properties(test_network_transport PROPERTIES  _BACKTRACE_TRIPLES "Z:/code/usb_share/CMakeLists.txt;52;add_test;Z:/code/usb_share/CMakeLists.txt;56;add_qt_test;Z:/code/usb_share/CMakeLists.txt;0;")
add_test(test_udp_discovery "Z:/code/usb_share/build_windows/test_udp_discovery.exe")
set_tests_properties(test_udp_discovery PROPERTIES  _BACKTRACE_TRIPLES "Z:/code/usb_share/CMakeLists.txt;52;add_test;Z:/code/usb_share/CMakeLists.txt;57;add_qt_test;Z:/code/usb_share/CMakeLists.txt;0;")
add_test(test_gui "Z:/code/usb_share/build_windows/test_gui.exe")
set_tests_properties(test_gui PROPERTIES  _BACKTRACE_TRIPLES "Z:/code/usb_share/CMakeLists.txt;67;add_test;Z:/code/usb_share/CMakeLists.txt;0;")
