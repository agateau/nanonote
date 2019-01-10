# Sources
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config.h)

set(nanonote_SRCS
    IndentExtension.cpp
    LinkSyntaxHighlighter.cpp
    LinkExtension.cpp
    MainWindow.cpp
    Settings.cpp
    SettingsDialog.cpp
    SettingsDialog.ui
    TextEdit.cpp
)

qt5_add_resources(nanonote_RESOURCES nanonote.qrc)

qpropgen(QPROPGEN_SRCS
    BaseSettings.yaml
)

# Building
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

# Lib
add_library(nanonotelib STATIC ${nanonote_SRCS} ${nanonote_RESOURCES} ${QPROPGEN_SRCS})
target_include_directories(nanonotelib PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(nanonotelib
    Qt5::Core
    Qt5::Widgets
)

# Main
add_executable(nanonote main.cpp)
target_link_libraries(nanonote
    nanonotelib
    singleapplication
)

# Install
install(
    TARGETS nanonote
    DESTINATION bin
)

install(FILES icons/nanonote.desktop
    DESTINATION share/applications
    RENAME com.agateau.nanonote.desktop
)

install(FILES icons/appicon.svg
    DESTINATION share/icons/hicolor/scalable/apps
    RENAME nanonote.svg
)
