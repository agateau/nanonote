add_executable(tests
    tests.cpp
    IndentExtensionTest.cpp
    LinkSyntaxHighlighterTest.cpp
    TestUtils.cpp
)

target_link_libraries(tests
    nanonotelib
    Qt5::Test
    Catch2::Catch2
)
