include(CMakeParseArguments)

# Internal
# Generate a .qrc file to hold generated .qm files.
# Takes .ts files as optional parameters
function(_CREATE_TRANSLATIONS_QRC TRANSLATIONS_QRC)
    set(TS_FILES=${ARGN})

    file(WRITE ${TRANSLATIONS_QRC} "<RCC>\n  <qresource prefix='/translations'>\n")
    foreach(ts_file ${TS_FILES})
        get_filename_component(name_we ${ts_file} NAME_WE)
        file(APPEND ${TRANSLATIONS_QRC} "    <file>${name_we}.qm</file>\n")
    endforeach()
    file(APPEND ${TRANSLATIONS_QRC} "  </qresource>\n</RCC>\n")
endfunction()

# Create a target to build a source file containing all the translations.
# Takes .ts files as optional parameters
function(ADD_TRANSLATIONS_BUILD_TARGET RES_SRCS_VARNAME)
    set(TS_FILES=${ARGN})

    # Add targets to build .ts files into .qm files
    qt5_add_translation(QM_FILES ${TS_FILES})
    add_custom_target(build_qm DEPENDS ${QM_FILES})

    set(TRANSLATIONS_QRC ${CMAKE_CURRENT_BINARY_DIR}/translations.qrc)
    _create_translations_qrc(${TRANSLATIONS_QRC} ${TS_FILES})

    set(RES_SRCS ${CMAKE_CURRENT_BINARY_DIR}/qrc_translations.cpp)

    # Build the translations qrc. Declare the command ourselves instead of using
    # CMake rcc automation so that we can tell CMake it depends on the "build_qm"
    # target
    add_custom_command(OUTPUT ${RES_SRCS}
        COMMAND Qt5::rcc -o ${RES_SRCS} --name translations ${TRANSLATIONS_QRC}
        DEPENDS build_qm
    )

    set(${RES_SRCS_VARNAME} "${RES_SRCS}" PARENT_SCOPE)
endfunction()

# Add an "lupdate" target. Use this target to update the .ts files from the
# latest source files.
# Syntax: add_lupdate_target(SOURCES <source files> TS_FILES <ts_files> OPTIONS <lupdate options>)
function(ADD_LUPDATE_TARGET)
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
