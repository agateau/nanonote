#include "SearchWidget.h"
#include "TextEdit.h"

#include "TextUtils.h"

#include <QAbstractButton>
#include <QLabel>
#include <QTest>

#include <catch2/catch_all.hpp>

struct CursorSpan {
    CursorSpan(const QTextCursor& cursor)
            : start(cursor.selectionStart()), length(cursor.selectionEnd() - start) {
    }
    CursorSpan(int start, int length) : start(start), length(length) {
    }

    const int start;
    const int length;
};

bool operator==(const CursorSpan& c1, const CursorSpan& c2) {
    return c1.start == c2.start && c1.length == c2.length;
}

TEST_CASE("searchwidget") {
    TextEdit edit;
    SearchWidget searchWidget(&edit);
    auto* nextButton = searchWidget.findChild<QAbstractButton*>("nextButton");
    REQUIRE(nextButton);
    auto* previousButton = searchWidget.findChild<QAbstractButton*>("previousButton");
    REQUIRE(nextButton);

    SECTION("forward search") {
        edit.setPlainText("a b b b");
        searchWidget.initialize("b");
        REQUIRE(dumpTextEditContent(&edit) == "a *b| b b");
        SECTION("search again") {
            QTest::mouseClick(nextButton, Qt::LeftButton);
            REQUIRE(dumpTextEditContent(&edit) == "a b *b| b");

            QTest::mouseClick(nextButton, Qt::LeftButton);
            REQUIRE(dumpTextEditContent(&edit) == "a b b *b|");

            SECTION("wrap around") {
                QTest::mouseClick(nextButton, Qt::LeftButton);
                REQUIRE(dumpTextEditContent(&edit) == "a *b| b b");
            }
        }
    }

    SECTION("backward search") {
        edit.setPlainText("a b b b");
        searchWidget.initialize("b");
        REQUIRE(dumpTextEditContent(&edit) == "a *b| b b");

        QTest::mouseClick(previousButton, Qt::LeftButton);
        REQUIRE(dumpTextEditContent(&edit) == "a b b *b|");

        QTest::mouseClick(previousButton, Qt::LeftButton);
        REQUIRE(dumpTextEditContent(&edit) == "a b *b| b");

        QTest::mouseClick(previousButton, Qt::LeftButton);
        REQUIRE(dumpTextEditContent(&edit) == "a *b| b b");
    }

    SECTION("no hit") {
        setupTextEditContent(&edit, "h|ello");

        searchWidget.initialize("b");
        REQUIRE(dumpTextEditContent(&edit) == "h|ello");
    }

    SECTION("count label") {
        edit.setPlainText("hello");
        auto* countLabel = searchWidget.findChild<QLabel*>("countLabel");
        auto isVisible = [countLabel, &searchWidget]() -> bool {
            return countLabel->isVisibleTo(&searchWidget);
        };

        searchWidget.initialize("");
        REQUIRE(!isVisible());

        searchWidget.initialize("llo");
        REQUIRE(isVisible());

        searchWidget.initialize("no match");
        REQUIRE(!isVisible());
    }

    SECTION("highlights") {
        edit.setPlainText("a bb bb");
        searchWidget.initialize("bb");
        auto selections = edit.extraSelections();
        CHECK(selections.count() == 2);
        CHECK(CursorSpan(selections.at(0).cursor) == CursorSpan{2, 2});
        CHECK(CursorSpan(selections.at(1).cursor) == CursorSpan{5, 2});

        SECTION("uninitialize must remove highlights") {
            searchWidget.uninitialize();
            selections = edit.extraSelections();
            REQUIRE(selections.count() == 0);

            SECTION("initializing again with the same text must bring back highlights") {
                searchWidget.initialize("bb");
                selections = edit.extraSelections();
                CHECK(selections.count() == 2);
                CHECK(CursorSpan(selections.at(0).cursor) == CursorSpan{2, 2});
                CHECK(CursorSpan(selections.at(1).cursor) == CursorSpan{5, 2});
            }
        }
    }

    SECTION("search, change selection") {
        // GIVEN a search with multiple matches
        edit.setPlainText("a bb bb cc bb");
        searchWidget.initialize("bb");
        REQUIRE(dumpTextEditContent(&edit) == "a *bb| bb cc bb");

        // AND cursor has been moved after the 2nd match
        auto cursor = edit.textCursor();
        cursor.setPosition(cursor.position() + 4);
        edit.setTextCursor(cursor);
        REQUIRE(dumpTextEditContent(&edit) == "a bb bb |cc bb");

        SECTION("then search forward") {
            // WHEN I search for the next match
            QTest::mouseClick(nextButton, Qt::LeftButton);

            // THEN the first match *after the cursor* is selected
            REQUIRE(dumpTextEditContent(&edit) == "a bb bb cc *bb|");
        }
        SECTION("then search backward") {
            // WHEN I search for the previous match
            QTest::mouseClick(previousButton, Qt::LeftButton);

            // THEN the first match *before the cursor* is selected
            REQUIRE(dumpTextEditContent(&edit) == "a bb *bb| cc bb");
        }
    }
}
