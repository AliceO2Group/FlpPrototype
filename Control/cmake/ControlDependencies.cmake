find_package(Boost 1.58 COMPONENTS unit_test_framework thread system program_options log REQUIRED)
find_package(Git QUIET)
find_package(Zookeeper)
find_package(FairRoot)

## TODO this should relaly not be needed
#FIND_LIBRARY(ZMQ_LIB zmq)
#find_package(Boost 1.58 COMPONENTS unit_test_framework program_options log thread system)
# TODO is it needed ?
if(BOOST_FOUND OR Boost_FOUND)
    set(BOOST_FOUND 1)
    set(Boost_FOUND 1)
endif()

if(FAIRROOT_FOUND)
    # this should go away when fairrot provides a proper Find script or proper config scripts
    # See : http://www.cmake.org/cmake/help/v3.0/command/link_directories.html
    link_directories(${FAIRROOT_LIBRARY_DIR})
    set(FAIRROOT_LIBRARIES Base FairMQ BaseMQ fairmq_logger)
endif(FAIRROOT_FOUND)

o2_define_bucket(
        NAME
        o2_control_bucket

        DEPENDENCIES
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        pthread
        ${Zookeeper_LIBRARIES}
        Common

        SYSTEMINCLUDE_DIRECTORIES
        ${Boost_INCLUDE_DIRS}
        ${Zookeeper_INCLUDE_DIRS}
)

o2_define_bucket(
        NAME
        o2_zdaq_bucket

        DEPENDENCIES
        ${Boost_PROGRAM_OPTIONS_LIBRARY}

        SYSTEMINCLUDE_DIRECTORIES
        ${Boost_INCLUDE_DIRS}
        ${Zookeeper_INCLUDE_DIRS}
)

o2_define_bucket(
        NAME
        o2_fairmq_control_bucket

        DEPENDENCIES
        Common
        InfoLogger
        ${Boost_LOG_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${FAIRROOT_LIBRARIES}
        pthread

        SYSTEMINCLUDE_DIRECTORIES
        ${Boost_INCLUDE_DIRS}
        ${Zookeeper_INCLUDE_DIRS}
        ${FAIRROOT_INCLUDE_DIR}

)
