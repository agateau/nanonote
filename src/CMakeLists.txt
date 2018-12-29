# Sources
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config.h)

set(nanonote_SRCS
    main.cpp
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
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual")
set(CMAKE_CXX_STANDARD 17)
add_executable(nanonote ${nanonote_SRCS} ${nanonote_RESOURCES} ${QPROPGEN_SRCS})

target_link_libraries(nanonote
    singleapplication
    Qt5::Core
    Qt5::Widgets
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