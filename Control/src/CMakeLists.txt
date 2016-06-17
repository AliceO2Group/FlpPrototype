# @author  Barthélémy von Haller

# -------
# Files listing
# -------

set(LIB_ZDAQ_HEADERS
        "${CMAKE_CURRENT_BINARY_DIR}/../include/Control/Version.h" # Generated header
        ../include/Control/zdaq.h # Normal header
        ../include/Control/zdaq_ctrl.h # Normal header
        )

set(LIB_ZDAQ_SOURCES
        zdaq.cxx
        zdaq_ctrl.cxx
        )

set(LIB_STATE_MACHINE_HEADERS
        "${CMAKE_CURRENT_BINARY_DIR}/../include/Control/Version.h" # Generated header
        ../include/Control/StateMachine.h # Normal header
        )

set(LIB_STATE_MACHINE_SOURCES
        StateMachine.cxx
        BaseControl.cxx
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


# -------
# StateMachine
# -------

set(LIBRARY_STATE_MACHINE_NAME StateMachine)
add_library(${LIBRARY_STATE_MACHINE_NAME} SHARED ${LIB_STATE_MACHINE_SOURCES})
target_link_libraries(
        ${LIBRARY_STATE_MACHINE_NAME}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        pthread
        ${Zookeeper_LIBRARIES}
        Common)

# Set the build version. It will be used in the name of the lib, with corresponding
# symlinks created. SOVERSION could also be specified for api version.
set_target_properties(${LIBRARY_STATE_MACHINE_NAME} PROPERTIES
        VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
        FRAMEWORK FALSE
        PUBLIC_HEADER "${LIB_STATE_MACHINE_HEADERS}" # specify the public headers
        )

# -------
# Targets export for all libraries and executables
# -------

# Says how and where to install software
install(TARGETS ${LIBRARY_ZDAQ_NAME} ${LIBRARY_STATE_MACHINE_NAME}
  EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
  ARCHIVE DESTINATION lib COMPONENT libs   # static lib
  RUNTIME DESTINATION bin COMPONENT libs   # binaries
  LIBRARY DESTINATION lib COMPONENT libs   # shared lib
  FRAMEWORK DESTINATION bin COMPONENT libs # for mac
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION include             # headers
)

