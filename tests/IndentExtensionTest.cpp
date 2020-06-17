#include "IndentExtension.h"

#include <QMainWindow>
#include <QTest>
#include <QTextCursor>

#include <catch2/catch.hpp>

#include "Catch2QtUtils.h"

#include <optional>

using std::optional;

/**
 * Take a TextEdit and a text and setup the TextEdit text and selection
 *
 * The text parameter contains the text to use, but it *must* contain also a '{' to indicate the
 * selection start. It *can* also contain a '}' to indicate the selection end.
 *
 * '{' can appear *after* '}' in case of an upward selection.
 */
static void setupText(TextEdit* edit, const QString& text) {
    QString realText;
    optional<int> begin, end;
    int current = 0;
    for (const auto& qChar : text) {
        char ch = qChar.toLatin1();
        switch (ch) {
        case '{':
            Q_ASSERT(!begin.has_value());
            begin = current;
            break;
        case '}':
            Q_ASSERT(!end.has_value());
            end = current;
            break;
        default:
            realText += ch;
            ++current;
        }
    }
    edit->setPlainText(realText);
    auto cursor = edit->textCursor();
    Q_ASSERT(begin.has_value());
    cursor.setPosition(begin.value());
    if (end.has_value()) {
        cursor.setPosition(end.value(), QTextCursor::KeepAnchor);
    }
    edit->setTextCursor(cursor);
}

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
        setupText(edit,
                  "{1\n"
                  "2\n"
                  "}3\n");
        auto cursor = edit->textCursor();
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(edit->toPlainText() == QString("    1\n    2\n3\n"));
    }

    SECTION("unindent whole lines") {
        setupText(edit,
                  "{    1\n"
                  "    2\n"
                  "}3\n");
        QTest::keyClick(edit, Qt::Key_Backtab);
        REQUIRE(edit->toPlainText() == QString("1\n2\n3\n"));
    }

    // https://github.com/agateau/nanonote/issues/6
    SECTION("indent upward selection") {
        setupText(edit,
                  "}a\n"
                  "{b\n");
        // Indent twice, only the first line should be indented
        QTest::keyClick(edit, Qt::Key_Tab);
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(edit->toPlainText() == QString("        a\nb\n"));
    }

    SECTION("indent partially selected lines") {
        setupText(edit,
                  "{aa\n"
                  "b}b\n");
        // Indent, both lines should be indented
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(edit->toPlainText() == QString("    aa\n    bb\n"));
    }

    SECTION("indent upward partially selected lines") {
        setupText(edit,
                  "a}a\n"
                  "b{b\n");
        // Indent, both lines should be indented
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(edit->toPlainText() == QString("    aa\n    bb\n"));
    }

    SECTION("indent at start of unindented list") {
        setupText(edit,
                  "- item\n"
                  "- {\n");
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(edit->toPlainText() == QString("- item\n    - \n"));
    }

    SECTION("indent at start of unindented list, no trailing newline") {
        setupText(edit,
                  "- item\n"
                  "- {");
        edit->moveCursor(QTextCursor::Down);
        edit->moveCursor(QTextCursor::Right);
        edit->moveCursor(QTextCursor::Right);
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(edit->toPlainText() == QString("- item\n    - "));
    }

    SECTION("indent at start of indented list") {
        setupText(edit,
                  "    - item\n"
                  "    - {");
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(edit->toPlainText() == QString("    - item\n        - "));
    }

    SECTION("Return on empty bullet line removes the bullet") {
        setupText(edit, "- {");
        QTest::keyClick(edit, Qt::Key_Return);
        REQUIRE(edit->toPlainText() == QString());
    }

    SECTION("Return on empty indented bullet line unindents") {
        setupText(edit, "    - {");
        QTest::keyClick(edit, Qt::Key_Return);
        REQUIRE(edit->toPlainText() == QString("- "));
    }

    SECTION("Return on empty line inserts a new line") {
        setupText(edit, "{");
        QTest::keyClick(edit, Qt::Key_Return);
        REQUIRE(edit->toPlainText() == QString("\n"));
    }

    SECTION("Return on selected text replaces it with a new line") {
        setupText(edit,
                  "{a\n"
                  "b}c");
        QTest::keyClick(edit, Qt::Key_Return);
        REQUIRE(edit->toPlainText() == QString("\nc"));
    }
}
