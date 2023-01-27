project(singleapplication)

find_package(Qt5 CONFIG REQUIRED Core Widgets Network)

set(CMAKE_AUTOMOC ON)

set(singleapplication_SRCS
    src/singleapplication.cpp
    src/singleapplication_p.cpp
)

add_library(singleapplication STATIC ${singleapplication_SRCS})

target_compile_definitions(singleapplication
    PRIVATE -DQAPPLICATION_CLASS=QApplication
    INTERFACE -DQAPPLICATION_CLASS=QApplication)

target_include_directories(singleapplication
    INTERFACE ${singleapplication_SOURCE_DIR}/src)

target_link_libraries(singleapplication Qt5::Core Qt5::Widgets Qt5::Network)
