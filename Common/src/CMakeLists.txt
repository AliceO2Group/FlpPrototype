# @author  Barthélémy von Haller
set(LIB_HEADERS
  "${CMAKE_CURRENT_BINARY_DIR}/../include/Common/Version.h" # Generated header
        # Normal header
  ../include/Common/signalUtilities.h # Normal header
  ../include/Common/Timer.h # Normal header
  ../include/Common/Exceptions.h # Normal header
  ../include/Common/Thread.h # Normal header
  ../include/Common/Fifo.h # Normal header
)

set(LIB_SOURCES
  World.cxx
  Timer.cxx
  Thread.cxx
)

# Define two variables in order not to repeat ourselves.
set(LIBRARY_NAME Common)

# Define the library
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})
target_link_libraries(${LIBRARY_NAME} InfoLogger-static)

# Set the build version. It will be used in the name of the lib, with corresponding
# symlinks created. SOVERSION could also be specified for api version. 
set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
  FRAMEWORK FALSE
  PUBLIC_HEADER "${LIB_HEADERS}" # specify the public headers
)

# Links the target exe (projArunner) against the library (libprojA) and other required libraries
target_link_libraries(${LIBRARY_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY})

# Says how and where to install software
install(TARGETS ${LIBRARY_NAME}
  EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
  ARCHIVE DESTINATION lib COMPONENT libs   # static lib
  RUNTIME DESTINATION bin COMPONENT libs   # binaries
  LIBRARY DESTINATION lib COMPONENT libs   # shared lib
  FRAMEWORK DESTINATION bin COMPONENT libs # for mac
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION include             # headers
)

