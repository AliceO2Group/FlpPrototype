file(GLOB_RECURSE ALL_SOURCE_FILES "${CMAKE_SOURCE_DIR}/../*.cpp" "${CMAKE_SOURCE_DIR}/../*.h")

function(call_clang_format file_name) 

    message(${file_name})
#    execute_process(
#        clang-format
#        -style=LLVM
#        -i
#        ${ALL_SOURCE_FILES}
#        )

endfunction()

message("execute_clang_format ${CMAKE_SOURCE_DIR}/..")
# We want to say one message by file, thus the ugly loop
foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
#        string(FIND ${SOURCE_FILE} ${PROJECT_TRDPARTY_DIR} PROJECT_TRDPARTY_DIR_FOUND)
#        if (NOT ${PROJECT_TRDPARTY_DIR_FOUND} EQUAL -1)
#            list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
#        endif ()
#    message(${SOURCE_FILE})
    call_clang_format(${SOURCE_FILE})
endforeach ()
