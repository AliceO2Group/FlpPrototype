# @author  Barthélémy von Haller
set(LIB_HEADERS
  "${CMAKE_CURRENT_BINARY_DIR}/../include/Control/Version.h" # Generated header
  ../include/Control/StateMachine.h # Normal header
  ../include/Control/zdaq.h # Normal header
  ../include/Control/zdaq_ctrl.h # Normal header
)

set(LIB_SOURCES
  StateMachine.cxx
        zdaq.cxx
        zdaq_ctrl.cxx
)

# Define two variables in order not to repeat ourselves.
set(LIBRARY_NAME zdaq)

# Define the library
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})

target_link_libraries(
        ${LIBRARY_NAME}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        pthread
        zookeeper_mt
        zmq)

# Set the build version. It will be used in the name of the lib, with corresponding
# symlinks created. SOVERSION could also be specified for api version. 
set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
  FRAMEWORK FALSE
  PUBLIC_HEADER "${LIB_HEADERS}" # specify the public headers
)

# Says how and where to install software
install(TARGETS ${LIBRARY_NAME} ${APPLICATION_NAME}
  EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
  ARCHIVE DESTINATION lib COMPONENT libs   # static lib
  RUNTIME DESTINATION bin COMPONENT libs   # binaries
  LIBRARY DESTINATION lib COMPONENT libs   # shared lib
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers (note the different component -> different package)
  FRAMEWORK DESTINATION bin COMPONENT libs # for mac
)

