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

    // https://github.com/agateau/nanonote/issues/6
    SECTION("indent upward selection") {
        edit->setPlainText("a\nb\n");
        // Go down, then select the first line by going up
        edit->moveCursor(QTextCursor::Down);
        edit->moveCursor(QTextCursor::Up, QTextCursor::KeepAnchor);
        // Indent twice, only the first line should be indented
        QTest::keyClick(edit, Qt::Key_Tab);
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(edit->toPlainText() == QString("        a\nb\n"));
    }

    SECTION("indent partially selected lines") {
        edit->setPlainText("aa\nbb\n");
        // Select first line, then first char of second line
        edit->moveCursor(QTextCursor::Down, QTextCursor::KeepAnchor);
        edit->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
        // Indent, both lines should be indented
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(edit->toPlainText() == QString("    aa\n    bb\n"));
    }

    SECTION("indent upward partially selected lines") {
        edit->setPlainText("aa\nbb\n");
        // Go to second line, move one char right then select up (so the last 'a' and the first 'b' are selected)
        // Select first line, then first char of second line
        edit->moveCursor(QTextCursor::Down);
        edit->moveCursor(QTextCursor::Right);
        edit->moveCursor(QTextCursor::Up, QTextCursor::KeepAnchor);
        // Indent, both lines should be indented
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(edit->toPlainText() == QString("    aa\n    bb\n"));
    }
}
