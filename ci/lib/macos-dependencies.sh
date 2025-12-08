main() {
    install_qt
    install_cmake
    install_ecm
    python -m venv .venv
    . .venv/bin/activate
    python -m pip install ds_store==1.1.2
}
