# Define Catch2 as a header-only library, mimicking the interface produced by
# Catch2 cmake files when Catch2 is installed system-wide
add_library(Catch2 INTERFACE)
target_compile_definitions(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(Catch2::Catch2 ALIAS Catch2)

add_subdirectory(singleapplication)
