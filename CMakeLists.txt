cmake_minimum_required(VERSION 3.10)
project(nanonote
    VERSION 1.2.0
    DESCRIPTION "Minimalist note taking application for short-lived notes"
)

# Dependencies
find_package(Qt5 CONFIG REQUIRED Core Widgets Test LinguistTools)
include(3rdparty/qpropgen/cmake/qpropgen.cmake)
add_subdirectory(3rdparty/singleapplication)
add_subdirectory(3rdparty/catch2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual")
set(CMAKE_CXX_STANDARD 17)
enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

# Packaging
set(PROJECT_URL "https://github.com/agateau/nanonote")

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "Aurélien Gâteau <mail@agateau.com>")

set(CPACK_GENERATOR "DEB;RPM")
set(CPACK_SOURCE_GENERATOR "TBZ2")

## DEB config
set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${PROJECT_URL})
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_RELEASE "0agc1")

## RPM config
set(CPACK_RPM_FILE_NAME "RPM-DEFAULT")
set(CPACK_RPM_PACKAGE_URL ${PROJECT_URL})

## Source config
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_SOURCE_IGNORE_FILES "/build/;/.git/;/__pycache__/;~$")
include(CPack)
