AQTINSTALL_VERSION=3.3.0
# qtdeclarative is not used by the app itself, but lupdate links against QtQml
AQTINSTALL_ARCHIVES="qtbase qtdeclarative qtimageformats qtsvg qttranslations qttools"

check_pipx() {
    echo_title "Looking for pipx"
    if command -v pipx 2> /dev/null ; then
        echo "Found pipx"
        return
    fi
    die "Can't find pipx."
}

install_qt() {
    echo_title "Installing Qt"
    local qt_install_dir=$INST_DIR/qt
    local aqt_args
    if is_windows ; then
        aqt_args="windows desktop $QT_VERSION $QT_ARCH_WINDOWS"
    fi
    if is_macos ; then
        aqt_args="mac desktop $QT_VERSION $QT_ARCH_MACOS"
    fi
    pipx install aqtinstall==$AQTINSTALL_VERSION

    aqt install-qt \
        $aqt_args \
        --outputdir $qt_install_dir \
        --archives $AQTINSTALL_ARCHIVES

    if is_windows ; then
        # Add Qt bin dir to $PATH so that tests can find Qt dlls
        prepend_path $(find $qt_install_dir -type d -a -name bin)
    fi
    # Add Qt plugins dir to $QT_PLUGIN_PATH because the official Qt installer
    # patches QtCore dll so that it finds its plugins, but aqt does not.
    # Not being able to find plugins causes tests to not run on macOS and
    # Windows because they can't find the matching platform plugin.
    add_env_var QT_PLUGIN_PATH $(find $qt_install_dir -type d -a -name plugins)
    add_env_var Qt6_DIR $(find $qt_install_dir -path '*/lib/cmake')
}

install_cmake() {
    echo_title "Installing CMake"
    pipx install cmake==$CMAKE_VERSION
}

install_ecm() {
    echo_title "Installing ECM"
    git clone --depth 1 https://invent.kde.org/frameworks/extra-cmake-modules.git -b v$ECM_VERSION
    (
        cd extra-cmake-modules
        mkdir build
        cd build
        cmake \
            -DCMAKE_INSTALL_PREFIX=$INST_DIR \
            -DBUILD_HTML_DOCS=OFF \
            -DBUILD_MAN_DOCS=OFF \
            -DBUILD_QTHELP_DOCS=OFF \
            -DBUILD_TESTING=OFF \
            -G "$CMAKE_GENERATOR" \
            ..
        cmake --build .
        cmake --build . --target install
    )
}
