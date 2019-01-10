#include "LinkSyntaxHighlighter.h"

#include <catch2/catch.hpp>

TEST_CASE("getLinkAt") {
    QString text = "link to http://google.fr. The end.";
    QUrl expected("http://google.fr");
    SECTION("before link") {
        REQUIRE(LinkSyntaxHighlighter::getLinkAt(text, 0) == QUrl());
    }
    SECTION("at link") {
        REQUIRE(LinkSyntaxHighlighter::getLinkAt(text, 10) == expected);
    }
    SECTION("after link") {
        REQUIRE(LinkSyntaxHighlighter::getLinkAt(text, 25) == QUrl());
    }
}
