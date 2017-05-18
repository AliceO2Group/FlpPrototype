set(MODULE_NAME "ControlFairMQ")

if(NOT FAIRROOT_FOUND)
    message(WARNING "FairRoot not found, module ControlFairMQ will not be compiled.")
    return()
endif(NOT FAIRROOT_FOUND)

O2_SETUP(NAME ${MODULE_NAME})

set(SRCS
        src/Sender.cxx
        src/Receiver.cxx
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
set(BUCKET_NAME o2_fairmq_control_bucket)

O2_GENERATE_LIBRARY()

O2_GENERATE_EXECUTABLE(
        EXE_NAME launchSender
        SOURCES src/launchSender.cxx
        MODULE_LIBRARY_NAME ${LIBRARY_NAME}
        BUCKET_NAME ${BUCKET_NAME}
)

O2_GENERATE_EXECUTABLE(
        EXE_NAME launchReceiver
        SOURCES src/launchReceiver.cxx
        MODULE_LIBRARY_NAME ${LIBRARY_NAME}
        BUCKET_NAME ${BUCKET_NAME}
)