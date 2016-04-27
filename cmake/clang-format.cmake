# additional target to perform clang-format run, requires clang-format
# Inspired by : https://arcanis.me/en/2015/10/17/cppcheck-and-clang-format/

file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)

add_custom_target(
        format
        COMMAND clang-format
        -style=file
        -i
        ${ALL_SOURCE_FILES}
)

# We need to call a function from a target. 
# The only way is to call the function via cmake as a command. 
add_custom_target(check_format
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/execute_clang_format.cmake
)