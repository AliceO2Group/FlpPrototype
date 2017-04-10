set(MODULE_NAME "ControlStateMachine")

O2_SETUP(NAME ${MODULE_NAME})

set(SRCS
        src/StateMachine.cxx
        src/BaseControl.cxx
        )

# Produce the final Version.h using template Version.h.in and substituting variables.
# We don't want to polute our source tree with it, thus putting it in binary tree.
configure_file("include/${MODULE_NAME}/Version.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/include/${MODULE_NAME}/Version.h"
        @ONLY
        )
include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}/include
)

set(LIBRARY_NAME ${MODULE_NAME})
set(BUCKET_NAME o2_control_bucket)
O2_GENERATE_LIBRARY()

set(TEST_SRCS
        test/testStateMachine.cxx
        test/testSendCommand.cxx
        test/testClient.cxx
        test/testBaseControl.cxx
        )

O2_GENERATE_TESTS(
        MODULE_LIBRARY_NAME ${LIBRARY_NAME}
        BUCKET_NAME ${BUCKET_NAME}
        TEST_SRCS ${TEST_SRCS}
)

