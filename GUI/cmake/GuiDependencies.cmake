find_package(Boost COMPONENTS unit_test_framework program_options log thread system REQUIRED)
find_package(Git QUIET)
find_package(ZeroMQ REQUIRED)
# because FindZeroMQ.cmake differs from system to system.
if(NOT ZeroMQ_INCLUDE_DIR)
    set(ZeroMQ_INCLUDE_DIR ${ZEROMQ_INCLUDE_DIR})
    set(ZeroMQ_LIBRARIES ${ZEROMQ_LIBRARIES})
endif()
# Alfa
find_package(FairRoot)
if(FAIRROOT_FOUND)
    # this should go away when fairrot provides a proper Find script or proper config scripts
    # See : http://www.cmake.org/cmake/help/v3.0/command/link_directories.html
    link_directories(${FAIRROOT_LIBRARY_DIR})
    set(FAIRROOT_LIBRARIES Base FairMQ BaseMQ)
else(FAIRROOT_FOUND)
    message(WARNING "FairRoot not found, example-3 will not be compiled.")
endif(FAIRROOT_FOUND)

o2_define_bucket(
        NAME
        o2_gui_bucket

        DEPENDENCIES
        ${ZeroMQ_LIBRARIES}
        InfoLogger

        SYSTEMINCLUDE_DIRECTORIES
        ${Boost_INCLUDE_DIRS}
        ${ZeroMQ_INCLUDE_DIR}
)

message("ZeroMQ_INCLUDE_DIR : ${ZeroMQ_INCLUDE_DIR}")

o2_define_bucket(
        NAME
        o2_gui_with_fairroot_bucket

        DEPENDENCIES
        o2_gui_bucket
        ${FAIRROOT_LIBRARIES}
        ${Boost_THREAD_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        pthread
        dl

        SYSTEMINCLUDE_DIRECTORIES
        ${FAIRROOT_INCLUDE_DIR}
        ${FAIRROOT_INCLUDE_DIR}/fairmq
)