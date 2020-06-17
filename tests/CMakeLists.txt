add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose --build-config "$<CONFIG>"
)

add_executable(tests
    tests.cpp
    IndentExtensionTest.cpp
    LinkSyntaxHighlighterTest.cpp
    SearchWidgetTest.cpp
    Catch2QtUtils.cpp
    TextUtils.cpp
)

target_link_libraries(tests
    ${APPLIB_NAME}
    Qt5::Test
    Catch2::Catch2
)

add_test(NAME tests COMMAND tests)

# Before running tests, make sure tests they are built if necessary
add_dependencies(check tests)
