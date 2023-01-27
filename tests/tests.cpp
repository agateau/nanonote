#include <catch2/catch_all.hpp>

#include <QApplication>
#include <QtTest>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QTEST_SET_MAIN_SOURCE_PATH
    return Catch::Session().run(argc, argv);
}
