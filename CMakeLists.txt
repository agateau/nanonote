cmake_minimum_required(VERSION 3.5)
project(nanonote VERSION 1.0.1)


# Dependencies
find_package(Qt5 CONFIG REQUIRED Core Widgets)
include(qpropgen/cmake/qpropgen.cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

add_subdirectory(singleapplication)
add_subdirectory(src)
