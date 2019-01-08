#include "IndentExtension.h"

#include <QMainWindow>
#include <QTest>

#include <catch2/catch.hpp>

#include "TestUtils.h"

TEST_CASE("textedit") {
    QMainWindow window;
    TextEdit *edit = new TextEdit;
    window.setCentralWidget(edit);
    edit->addExtension(new IndentExtension(edit));

    SECTION("indent with tab") {
        edit->setPlainText("Hello");
        QTest::keyClick(edit, Qt::Key_Tab);
        REQUIRE(edit->toPlainText() == QString("    Hello"));
    }

    /* broken ATM
    SECTION("indent with ctrl+I") {
        edit->setPlainText("Hello");
        QTest::keyClick(edit, Qt::Key_I, Qt::ControlModifier);
        REQUIRE(edit->toPlainText() == QString("    Hello"));
    }
    */
}
