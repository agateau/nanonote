#include "LinkSyntaxHighlighter.h"

#include <catch2/catch.hpp>

#include "TestUtils.h"

TEST_CASE("getLinkAt") {
    QString uglyUrl = "http://foo.com/~arg;foo+bar%20#";
    QString text = QString("link to %1. The end.").arg(uglyUrl);
    QUrl expected(uglyUrl);
    SECTION("before link") {
        REQUIRE(LinkSyntaxHighlighter::getLinkAt(text, 0) == QUrl());
    }
    SECTION("at link") {
        REQUIRE(LinkSyntaxHighlighter::getLinkAt(text, 10) == expected);
    }
    SECTION("after link") {
        REQUIRE(LinkSyntaxHighlighter::getLinkAt(text, text.length() - 4) == QUrl());
    }
}
