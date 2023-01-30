include(CMakeParseArguments)

# Add an "lupdate" target. Use this target to update the .ts files from the
# latest source files.
# Syntax: add_lupdate_target(SOURCES <source files> TS_FILES <ts_files> OPTIONS <lupdate options>)
function(add_lupdate_target)
    set(options)
    set(one_value_args)
    set(multi_value_args SOURCES TS_FILES OPTIONS)

    cmake_parse_arguments(lupdate "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    # List all sources in a file and call lupdate with this file as argument
    # instead of adding all sources on the command line, to ensure the command
    # line does not get too long
    set(content)
    foreach(src_file ${lupdate_SOURCES})
        set(content "${src_file}\n${content}")
    endforeach()

    # Add include directories
    get_directory_property(include_dirs INCLUDE_DIRECTORIES)
    foreach(include_dir ${include_dirs})
        get_filename_component(include_dir "${include_dir}" ABSOLUTE)
        set(content "-I${include_dir}\n${content}")
    endforeach()

    set(list_file "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/sources.lst")
    file(WRITE ${list_file} "${content}")

    # Add the lupdate target
    add_custom_target(lupdate
        COMMAND Qt5::lupdate ${lupdate_options} "@${list_file}" -ts ${lupdate_TS_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endfunction()
