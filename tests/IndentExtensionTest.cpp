#include "IndentExtension.h"

#include <QMainWindow>
#include <QTest>
#include <QTextCursor>

#include <catch2/catch_all.hpp>

#include "Catch2QtUtils.h"
#include "TextUtils.h"

TEST_CASE("textedit") {
    QMainWindow window;
    TextEdit* edit = new TextEdit;
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
        setupTextEditContent(edit,
                             "*1\n"
                             "2\n"
                             "|3\n");
        auto cursor = edit->textCursor();
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(dumpTextEditContent(edit)
                == QString("    *1\n"
                           "    2\n"
                           "|3\n"));
    }

    SECTION("unindent whole lines") {
        setupTextEditContent(edit,
                             "*    1\n"
                             "    2\n"
                             "|3\n");
        QTest::keyClick(edit, Qt::Key_Backtab);
        REQUIRE(dumpTextEditContent(edit)
                == QString("*1\n"
                           "2\n"
                           "|3\n"));
    }

    // https://github.com/agateau/nanonote/issues/6
    SECTION("indent upward selection") {
        setupTextEditContent(edit,
                             "|a\n"
                             "*b\n");
        // Indent twice, only the first line should be indented
        QTest::keyClick(edit, Qt::Key_Tab);
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(dumpTextEditContent(edit)
                == QString("        |a\n"
                           "*b\n"));
    }

    SECTION("indent partially selected lines") {
        setupTextEditContent(edit,
                             "*aa\n"
                             "b|b\n");
        // Indent, both lines should be indented
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(dumpTextEditContent(edit)
                == QString("    *aa\n"
                           "    b|b\n"));
    }

    SECTION("indent upward partially selected lines") {
        setupTextEditContent(edit,
                             "a|a\n"
                             "b*b\n");
        // Indent, both lines should be indented
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(dumpTextEditContent(edit)
                == QString("    a|a\n"
                           "    b*b\n"));
    }

    SECTION("indent at start of unindented list") {
        setupTextEditContent(edit,
                             "- item\n"
                             "- |\n");
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(dumpTextEditContent(edit)
                == QString("- item\n"
                           "    - |\n"));
    }

    SECTION("indent at start of unindented list, no trailing newline") {
        setupTextEditContent(edit,
                             "- item\n"
                             "- |");
        edit->moveCursor(QTextCursor::Down);
        edit->moveCursor(QTextCursor::Right);
        edit->moveCursor(QTextCursor::Right);
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(dumpTextEditContent(edit)
                == QString("- item\n"
                           "    - |"));
    }

    SECTION("indent at start of indented list") {
        setupTextEditContent(edit,
                             "    - item\n"
                             "    - |");
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(dumpTextEditContent(edit)
                == QString("    - item\n"
                           "        - |"));
    }

    SECTION("Return on empty bullet line removes the bullet") {
        setupTextEditContent(edit, "- |");
        QTest::keyClick(edit, Qt::Key_Return);
        REQUIRE(edit->toPlainText() == QString());
    }

    SECTION("Return on empty indented bullet line unindents") {
        setupTextEditContent(edit, "    - |");
        QTest::keyClick(edit, Qt::Key_Return);
        REQUIRE(dumpTextEditContent(edit) == QString("- |"));
    }

    SECTION("Return on empty line inserts a new line") {
        setupTextEditContent(edit, "|");
        QTest::keyClick(edit, Qt::Key_Return);
        REQUIRE(dumpTextEditContent(edit) == QString("\n|"));
    }

    SECTION("Return on selected text replaces it with a new line") {
        setupTextEditContent(edit,
                             "*a\n"
                             "b|c");
        QTest::keyClick(edit, Qt::Key_Return);
        REQUIRE(dumpTextEditContent(edit)
                == QString("\n"
                           "|c"));
    }
}
