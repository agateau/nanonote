if (APPLE)
    include(find_python_module)

    # If the ds_store Python module is available, we can generate a
    # .DS_Store file to customize the layout of the Finder window for the
    # macOS DMG.
    find_python_module(ds_store)
    if (NOT ds_store_FOUND)
        message("Python module ds_store not found: layout of the Finder window for the macOS DMG won't be customized")
    endif()
endif()

set(CPACK_PACKAGE_VENDOR ${ORGANIZATION_NAME})

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

set(CPACK_PACKAGE_CONTACT "${AUTHOR_NAME} <${AUTHOR_EMAIL}>")

set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})

set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${PROJECT_VERSION})

if (EXISTS ${PROJECT_SOURCE_DIR}/LICENSE)
    set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE)
endif()

if (UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB;RPM")

    set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${APP_URL})
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

    set(CPACK_RPM_FILE_NAME "RPM-DEFAULT")
    set(CPACK_RPM_PACKAGE_URL ${APP_URL})
endif()

if (WIN32)
    set(CPACK_GENERATOR "NSIS")

    set(CPACK_NSIS_PACKAGE_NAME ${APP_HUMAN_NAME})
    set(CPACK_NSIS_DISPLAY_NAME ${APP_HUMAN_NAME})
    set(CPACK_NSIS_URL_INFO_ABOUT ${APP_URL})
    set(CPACK_NSIS_CREATE_ICONS_EXTRA
        "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${APP_HUMAN_NAME}.lnk' '$INSTDIR\\\\${APP_NAME}.exe'")
    set(CPACK_NSIS_DELETE_ICONS_EXTRA
        "Delete '$SMPROGRAMS\\\\$START_MENU\\\\${APP_HUMAN_NAME}.lnk'")

    # Setting CPACK_NSIS_EXECUTABLES_DIRECTORY is required for
    # CPACK_NSIS_MUI_FINISHPAGE_RUN to find the executable.
    #
    # If not set, the default path is $INSTDIR/bin
    #
    # cf http://cmake.3232098.n2.nabble.com/Problems-with-CPack-NSIS-and-CPACK-NSIS-MUI-FINISHPAGE-RUN-td7003656.html
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
    set(CPACK_NSIS_MUI_FINISHPAGE_RUN "${APP_NAME}.exe")
endif()

if (APPLE)
    set(CPACK_GENERATOR "DragNDrop")

    if (ds_store_FOUND)
        set(DS_STORE_FILE ${CMAKE_CURRENT_BINARY_DIR}/dot-DS_Store)
        set(GENERATE_DS_STORE_CMD ${CMAKE_CURRENT_SOURCE_DIR}/macos/generate-ds-store)
        add_custom_command(
            OUTPUT ${DS_STORE_FILE}
            COMMAND ${GENERATE_DS_STORE_CMD} ${APP_NAME} ${DS_STORE_FILE}
            DEPENDS ${GENERATE_DS_STORE_CMD}
        )
        add_custom_target(generate_dsstore DEPENDS ${DS_STORE_FILE})
        add_dependencies(${APP_NAME} generate_dsstore)

        set(CPACK_DMG_DS_STORE ${DS_STORE_FILE})
    endif()

    set(CPACK_DMG_VOLUME_NAME ${APP_HUMAN_NAME})
endif()

set(CPACK_SOURCE_GENERATOR "TBZ2")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_SOURCE_IGNORE_FILES "/build/;/.git/;/__pycache__/;~$")

include(CPack)
