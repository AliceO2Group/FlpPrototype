find_package(Boost COMPONENTS unit_test_framework program_options log thread system REQUIRED)
find_package(Git QUIET)
find_package(ZeroMQ)
# Alfa
find_package(FairRoot)
if(FAIRROOT_FOUND)
    # this should go away when fairrot provides a proper Find script or proper config scripts
    # See : http://www.cmake.org/cmake/help/v3.0/command/link_directories.html
    link_directories(${FAIRROOT_LIBRARY_DIR})
    set(FAIRROOT_LIBRARIES Base FairMQ BaseMQ fairmq_logger)
else(FAIRROOT_FOUND)
    message(WARNING "FairRoot not found, example-3 will not be compiled.")
endif(FAIRROOT_FOUND)

o2_define_bucket(
        NAME
        o2_gui_bucket

        DEPENDENCIES
        ${ZEROMQ_LIBRARIES}
        InfoLogger

        SYSTEMINCLUDE_DIRECTORIES
        ${Boost_INCLUDE_DIRS}
        ${ZEROMQ_INCLUDE_DIR}
)

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

        SYSTEMINCLUDE_DIRECTORIES
        ${FAIRROOT_INCLUDE_DIR}
)