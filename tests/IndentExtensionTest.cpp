#include "IndentExtension.h"

#include <QMainWindow>
#include <QTest>
#include <QTextCursor>

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

    SECTION("indent whole lines") {
        edit->setPlainText("1\n2\n3\n");
        edit->moveCursor(QTextCursor::Down, QTextCursor::KeepAnchor);
        edit->moveCursor(QTextCursor::Down, QTextCursor::KeepAnchor);
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(edit->toPlainText() == QString("    1\n    2\n3\n"));
    }

    SECTION("unindent whole lines") {
        edit->setPlainText("    1\n    2\n3\n");
        edit->moveCursor(QTextCursor::Down, QTextCursor::KeepAnchor);
        edit->moveCursor(QTextCursor::Down, QTextCursor::KeepAnchor);
        QTest::keyClick(edit, Qt::Key_Backtab);
        REQUIRE(edit->toPlainText() == QString("1\n2\n3\n"));
    }

    /* broken ATM
    SECTION("indent with ctrl+I") {
        edit->setPlainText("Hello");
        QTest::keyClick(edit, Qt::Key_I, Qt::ControlModifier);
        REQUIRE(edit->toPlainText() == QString("    Hello"));
    }
    */
}
