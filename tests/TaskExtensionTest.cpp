#include "TaskExtension.h"

#include <QMainWindow>
#include <QTest>
#include <QTextCursor>

#include <catch2/catch_all.hpp>

#include "Catch2QtUtils.h"
#include "TextUtils.h"

TEST_CASE("taskextension") {
    QMainWindow window;
    TextEdit* edit = new TextEdit;
    window.setCentralWidget(edit);
    TaskExtension extension(edit);
    edit->addExtension(&extension);

    SECTION("Toggle unchecked task") {
        setupTextEditContent(edit, "- [ ] task|");
        extension.insertOrToggleTask();
        REQUIRE(dumpTextEditContent(edit) == QString("- [x] task|"));
    }

    SECTION("Toggle checked task") {
        setupTextEditContent(edit, "- [x] task|");
        extension.insertOrToggleTask();
        REQUIRE(dumpTextEditContent(edit) == QString("- [ ] task|"));
    }

    SECTION("Insert task") {
        setupTextEditContent(edit, "ta|sk");
        extension.insertOrToggleTask();
        REQUIRE(dumpTextEditContent(edit) == QString("- [ ] ta|sk"));
    }

    SECTION("Insert task in indented line") {
        setupTextEditContent(edit, "    |task");
        extension.insertOrToggleTask();
        REQUIRE(dumpTextEditContent(edit) == QString("    - [ ] |task"));
    }

    SECTION("Insert task in existing list") {
        setupTextEditContent(edit, "- task|");
        extension.insertOrToggleTask();
        REQUIRE(dumpTextEditContent(edit) == QString("- [ ] task|"));
    }

    SECTION("Insert task in existing indented list") {
        setupTextEditContent(edit, "    - task|");
        extension.insertOrToggleTask();
        REQUIRE(dumpTextEditContent(edit) == QString("    - [ ] task|"));
    }
}
