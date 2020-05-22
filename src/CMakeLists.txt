include(translations)

add_subdirectory(icons)

# Sources
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/BuildConfig.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/BuildConfig.h
)

set(APPLIB_SRCS
    IndentExtension.cpp
    LinkSyntaxHighlighter.cpp
    LinkExtension.cpp
    MainWindow.cpp
    Settings.cpp
    SettingsDialog.cpp
    SettingsDialog.ui
    TextEdit.cpp
    WheelZoomExtension.cpp
    SearchWidget.cpp
    SearchForm.ui
)

qt5_add_resources(APPLIB_RESOURCES_SRCS app.qrc)

qpropgen(QPROPGEN_SRCS
    BaseSettings.yaml
)
# Translations
set(TS_FILES
    translations/app_de.ts
    translations/app_es.ts
    translations/app_fr.ts
)
add_translations_build_target(TRANSLATIONS_RESOURCES_SRCS ${TS_FILES})
add_lupdate_target(SOURCES ${APPLIB_SRCS} TS_FILES ${TS_FILES} OPTIONS -no-obsolete)

# App library
add_library(${APPLIB_NAME} STATIC
    ${APPLIB_SRCS}
    ${QPROPGEN_SRCS}
    ${APPLIB_RESOURCES_SRCS}
    ${TRANSLATIONS_RESOURCES_SRCS}
)
target_include_directories(${APPLIB_NAME} PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${APPLIB_NAME}
    Qt5::Core
    Qt5::Widgets
)

# App executable
add_executable(${APP_NAME} WIN32 MACOSX_BUNDLE main.cpp)

if (APPLE)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/macos/Info.plist.in
        ${CMAKE_CURRENT_BINARY_DIR}/macos/Info.plist
    )
    set_target_properties(${APP_NAME} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/macos/Info.plist
    )
endif()

target_link_libraries(${APP_NAME}
    ${APPLIB_NAME}
    singleapplication
)

# Install
install(
    TARGETS ${APP_NAME}
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
)

if (UNIX AND NOT APPLE)
    install(FILES linux/${APP_NAME}.desktop
        DESTINATION share/applications
        RENAME ${INVERSE_ORGANIZATION_NAME}.${APP_NAME}.desktop
    )
endif()

if (WIN32)
    include(DeployQt)
    windeployqt(${APP_NAME})
endif()

if (APPLE)
    include(DeployQt)
    macdeployqt(${APP_NAME})
endif()
