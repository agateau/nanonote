#include "SearchWidget.h"
#include "TextEdit.h"

#include "TextUtils.h"

#include <QAbstractButton>
#include <QLabel>
#include <QTest>

#include <catch2/catch.hpp>

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
}
