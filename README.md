[![Build Status](https://github.com/agateau/nanonote/actions/workflows/main.yml/badge.svg)](https://github.com/agateau/nanonote/actions/workflows/main.yml)

# Nanonote

Nanonote is a minimalist note taking application.

![Screenshot](screenshot.png)

It automatically saves anything you type. Being minimalist means it has no synchronisation, does not support multiple documents, images or any advanced formatting (the only formatting is highlighting URLs and Markdown-like headings).

It is developed and tested on Linux but also works on macOS and Windows as well.

## Packages

Binary packages for Linux, macOS and Windows are available on the [release page][].

[release page]: https://github.com/agateau/nanonote/releases

## Tips and tricks

Even if Nanonote has a minimalist user interface, it comes with some handy shortcuts. Learn more about them from the [tips and tricks page](docs/tips.md).

## Building it

Nanonote requires Qt 5 and CMake. To build it, do the following:

Get the source:

    git clone https://github.com/agateau/nanonote
    cd nanonote
    git submodule update --init

Build Nanonote:

    mkdir build
    cd build
    cmake ..
    make
    sudo make install

You can also build rpm and deb files using `make package` after `make`.
