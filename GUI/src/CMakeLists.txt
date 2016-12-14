# @author  Barthélémy von Haller
set(LIB_HEADERS
  "${CMAKE_CURRENT_BINARY_DIR}/../include/GUI/Version.h" # Generated header
  ../include/GUI/zmq.hpp
  ../include/GUI/Publisher.h
  ../include/GUI/RequestNotifier.h
)

set(LIB_SOURCES
  Publisher.cxx
  RequestNotifier.cxx
)

set(APP_HEADERS
)

set(APP_SOURCES
)

FIND_PACKAGE(ZeroMQ)

# Define two variables in order not to repeat ourselves.
set(LIBRARY_NAME GUI)
set(INCLUDE include/GUI)

include_directories(${ZMQ_INCLUDE_DIR})

# Define the library
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})
target_link_libraries(${LIBRARY_NAME} Common InfoLogger-static ${ZEROMQ_LIBRARIES})

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
  FRAMEWORK DESTINATION bin COMPONENT libs # for mac
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION include             # headers
)

