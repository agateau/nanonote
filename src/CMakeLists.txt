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
    WheelZoomExtension.cpp
    Search.cpp
    SearchForm.ui
)

qt5_add_resources(nanonote_RESOURCES nanonote.qrc)

qpropgen(QPROPGEN_SRCS
    BaseSettings.yaml
)

# Translations
# NOTE: Add an entry to translations.qrc as well!
set(TS_FILES
    translations/nanonote_de.ts
    translations/nanonote_es.ts
    translations/nanonote_fr.ts
)

# To update .ts (also builds .qm files)
# Use "-locations none" so that .ts files do not change every time lines move
# in source files: that results in noisy diffs
qt5_create_translation(_TRANSLATION_UPDATE ${nanonote_SRCS} ${TS_FILES}
    OPTIONS -locations none)
add_custom_target(lupdate DEPENDS ${_TRANSLATION_UPDATE})

set(TRANSLATIONS_QRC translations.qrc)
configure_file(${TRANSLATIONS_QRC} ${TRANSLATIONS_QRC} COPYONLY)
qt5_add_resources(TRANSLATIONS_RESOURCES ${CMAKE_CURRENT_BINARY_DIR}/${TRANSLATIONS_QRC})

# Building
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

# Lib
add_library(nanonotelib STATIC
    ${nanonote_SRCS}
    ${nanonote_RESOURCES}
    ${QPROPGEN_SRCS}
    ${TRANSLATIONS_RESOURCES}
)
target_include_directories(nanonotelib PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(nanonotelib
    Qt5::Core
    Qt5::Widgets
)

# Main
add_executable(nanonote MACOSX_BUNDLE main.cpp)
set_target_properties(nanonote PROPERTIES MACOSX_BUNDLE_INFO_PLIST
    ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in)
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
