# Translations

Translations are stored in .ts files in the [src/translations/](../src/translations/) directory.

## Adding a new language

- Add a .ts file to the `TS_FILES` variable in [src/CMakeLists.txt](../src/CMakeLists.txt)
- Run `make lupdate` in the build directory to create the .ts file
- Translate the content of the .ts file with Linguist

## Testing a translation

- Build and install the app, preferably in a directory where you don't need to be root to install. This example assumes you install it to `$HOME/tmp/nanonote-inst`:
    ```
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=$HOME/tmp/nanonote-inst <path/to/nanonote/tree>
    make
    make install
    ```

- Run the app, possibly with the `LANGUAGE` variable set to force the language:
    ```
    LANGUAGE=<the_lang_to_test> $HOME/tmp/nanonote-inst/bin/nanonote
    ```

- If something is wrong: fix the translation, run `make install` and try again
