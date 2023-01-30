include(ECMInstallIcons)

if (UNIX AND NOT APPLE)
    ecm_install_icons(ICONS sc-apps-${APP_NAME}.svg
        DESTINATION share/icons
    )
endif()
