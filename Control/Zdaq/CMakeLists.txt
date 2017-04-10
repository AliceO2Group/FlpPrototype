set(MODULE_NAME "ControlZdaq")

O2_SETUP(NAME ${MODULE_NAME})

set(SRCS
        src/zdaq.cxx
        src/zdaq_ctrl.cxx
        )

set(LIBRARY_NAME ${MODULE_NAME})
set(BUCKET_NAME o2_zdaq_bucket)
O2_GENERATE_LIBRARY()

set(TEST_SRCS
        test/testZdaq.cxx
        )

O2_GENERATE_TESTS(
        MODULE_LIBRARY_NAME ${LIBRARY_NAME}
        BUCKET_NAME ${BUCKET_NAME}
        TEST_SRCS ${TEST_SRCS}
)


# -------
# ZDAQ
# -------

#set(LIBRARY_ZDAQ_NAME zdaq)
#add_library(${LIBRARY_ZDAQ_NAME} SHARED ${LIB_ZDAQ_SOURCES})
#target_link_libraries(
#        ${LIBRARY_ZDAQ_NAME}
#        ${Boost_SYSTEM_LIBRARY}
#        ${Boost_THREAD_LIBRARY}
#        pthread
#        ${Zookeeper_LIBRARIES}
#        ${ZMQ_LIB})

# Set the build version. It will be used in the name of the lib, with corresponding
# symlinks created. SOVERSION could also be specified for api version.
#set_target_properties(${LIBRARY_ZDAQ_NAME} PROPERTIES
#        VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
#        FRAMEWORK FALSE
#        PUBLIC_HEADER "${LIB_ZDAQ_HEADERS}" # specify the public headers
#        )

