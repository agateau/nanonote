#include "IndentExtension.h"

#include <QMainWindow>
#include <QTest>

#include <catch2/catch.hpp>

#include "TestUtils.h"

TEST_CASE("textedit") {
    QMainWindow window;
    TextEdit *edit = new TextEdit;
    window.setCentralWidget(edit);
    edit->addExtension(new IndentExtension(edit));
    SECTION("indent insert spaces") {
        edit->setPlainText("Hello");

        SECTION("indent from beginning of line") {
            QTest::keyClick(edit, Qt::Key_Tab);
            REQUIRE(edit->toPlainText() == QString("    Hello"));
        }

        SECTION("indent from middle of word") {
            QTest::keyClick(edit, Qt::Key_Right);
            QTest::keyClick(edit, Qt::Key_Tab);
            REQUIRE(edit->toPlainText() == QString("H   ello"));
        }
    }

    SECTION("indent from middle of word") {
        edit->setPlainText("Hello");
        QTest::keyClick(edit, Qt::Key_Right);
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(edit->toPlainText() == QString("H   ello"));
    }

    /* broken ATM
    SECTION("indent with ctrl+I") {
        edit->setPlainText("Hello");
        QTest::keyClick(edit, Qt::Key_I, Qt::ControlModifier);
        REQUIRE(edit->toPlainText() == QString("    Hello"));
    }
    */
}
