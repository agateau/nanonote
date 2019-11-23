#include "SearchWidget.h"
#include "TextEdit.h"

#include <QAbstractButton>
#include <QLabel>
#include <QTest>

#include <catch2/catch.hpp>

struct SelectionRange {
    int start;
    int end;
    SelectionRange(int s, int e) : start(s), end(e) {}
};

bool operator ==(const SelectionRange& r1, const SelectionRange& r2) {
    return r1.start == r2.start && r1.end == r2.end;
}

std::ostream& operator<<(std::ostream& ostr, const SelectionRange& range) {
    ostr << range.start << ", " << range.end;
    return ostr;
}

static SelectionRange getSelectionRange(QPlainTextEdit* edit) {
    int start = edit->textCursor().selectionStart();
    int end = edit->textCursor().selectionEnd();
    return {start, end};
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
        REQUIRE(getSelectionRange(&edit) == SelectionRange{2, 3});
        SECTION("search again") {
            QTest::mouseClick(nextButton, Qt::LeftButton);
            REQUIRE(getSelectionRange(&edit) == SelectionRange{4, 5});

            QTest::mouseClick(nextButton, Qt::LeftButton);
            REQUIRE(getSelectionRange(&edit) == SelectionRange{6, 7});

            SECTION("wrap around") {
                QTest::mouseClick(nextButton, Qt::LeftButton);
                REQUIRE(getSelectionRange(&edit) == SelectionRange{2, 3});
            }
        }
    }

    SECTION("backward search") {
        edit.setPlainText("a b b b");
        searchWidget.initialize("b");
        REQUIRE(getSelectionRange(&edit) == SelectionRange{2, 3});

        QTest::mouseClick(previousButton, Qt::LeftButton);
        REQUIRE(getSelectionRange(&edit) == SelectionRange{6, 7});

        QTest::mouseClick(previousButton, Qt::LeftButton);
        REQUIRE(getSelectionRange(&edit) == SelectionRange{4, 5});

        QTest::mouseClick(previousButton, Qt::LeftButton);
        REQUIRE(getSelectionRange(&edit) == SelectionRange{2, 3});
    }

    SECTION("no hit") {
        edit.setPlainText("hello");
        edit.moveCursor(QTextCursor::Right);
        REQUIRE(getSelectionRange(&edit) == SelectionRange{1, 1});

        searchWidget.initialize("b");
        REQUIRE(getSelectionRange(&edit) == SelectionRange{1, 1});
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
