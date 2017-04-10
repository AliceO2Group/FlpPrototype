find_package(Boost COMPONENTS unit_test_framework thread system REQUIRED)
find_package(Git QUIET)
find_package(Zookeeper)
## TODO this should relaly not be needed
#FIND_LIBRARY(ZMQ_LIB zmq)

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
