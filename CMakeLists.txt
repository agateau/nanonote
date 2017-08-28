project(nanonote)

cmake_minimum_required(VERSION 3.5)

# Dependencies
find_package(Qt5 CONFIG REQUIRED Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

# Sources
set(nanonote_SRCS
    main.cpp
    MainWindow.cpp
    TextEdit.cpp
)

qt5_add_resources(nanonote_RESOURCES nanonote.qrc)

# Building
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -std=c++11")
add_executable(nanonote ${nanonote_SRCS} ${nanonote_RESOURCES})

target_link_libraries(nanonote Qt5::Core Qt5::Widgets)

# Install
install(
    TARGETS nanonote
    DESTINATION bin
)
