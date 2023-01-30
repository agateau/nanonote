find_package(Python3)

# Use pip to check if a Python module is installed.
#
# ${module}_FOUND is set to ON if the module is found and OFF if not.
function(find_python_module module)
    if (NOT Python3_FOUND)
        message(STATUS "Could not find Python 3 module ${module}: no Python 3 interpreter")
        set(${module}_FOUND OFF PARENT_SCOPE)
        return()
    endif()
    execute_process(
        COMMAND ${Python3_EXECUTABLE} -m pip show -q ${module}
        RESULT_VARIABLE result
    )
    if (result EQUAL 0)
        message(STATUS "Found Python 3 module ${module}")
        set(${module}_FOUND ON PARENT_SCOPE)
    else()
        message(STATUS "Could not find Python 3 module ${module}")
        set(${module}_FOUND OFF PARENT_SCOPE)
    endif()
endfunction()
