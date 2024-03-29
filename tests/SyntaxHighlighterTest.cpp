#include "SyntaxHighlighter.h"

#include <catch2/catch_all.hpp>

#include "Catch2QtUtils.h"

TEST_CASE("getLinkAt") {
    QString uglyUrl = "http://foo.com/~arg;foo+bar%20#";
    QString text = QString("link to %1. The end.").arg(uglyUrl);
    QUrl expected(uglyUrl);
    SECTION("before link") {
        REQUIRE(SyntaxHighlighter::getLinkAt(text, 0) == QUrl());
    }
    SECTION("at link") {
        REQUIRE(SyntaxHighlighter::getLinkAt(text, 10) == expected);
    }
    SECTION("after link") {
        REQUIRE(SyntaxHighlighter::getLinkAt(text, text.length() - 4) == QUrl());
    }
}

TEST_CASE("getTaskCheckmarkPosAt") {
    QString text = QString("    - [x] test task");
    SECTION("before task") {
        REQUIRE(SyntaxHighlighter::getTaskCheckmarkPosAt(text, 5) == -1);
    }
    SECTION("start of task") {
        REQUIRE(SyntaxHighlighter::getTaskCheckmarkPosAt(text, 6) == 7);
    }
    SECTION("inside task") {
        REQUIRE(SyntaxHighlighter::getTaskCheckmarkPosAt(text, 8) == 7);
    }
    SECTION("end of task") {
        REQUIRE(SyntaxHighlighter::getTaskCheckmarkPosAt(text, 9) == -1);
    }
    SECTION("after task") {
        REQUIRE(SyntaxHighlighter::getTaskCheckmarkPosAt(text, 10) == -1);
    }
}
