# @author  Barthélémy von Haller
set(LIB_HEADERS
  "${CMAKE_CURRENT_BINARY_DIR}/../include/ProjA/Version.h" # Generated header
  ../include/ProjA/Sender.h # Normal header
  ../include/ProjA/Receiver.h # Normal header
)

set(LIB_SOURCES
  Sender.cxx
  Receiver.cxx
)

set(APP_HEADERS
)

set(APP_SENDER_SOURCES
        mainSender.cxx
)

set(APP_RECEIVER_SOURCES
        mainReceiver.cxx
)

# Define two variables in order not to repeat ourselves.
set(LIBRARY_NAME ProjA)

# Define the library
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})

# Set the build version. It will be used in the name of the lib, with corresponding
# symlinks created. SOVERSION could also be specified for api version. 
set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
  FRAMEWORK FALSE
  PUBLIC_HEADER "${LIB_HEADERS}" # specify the public headers
)

target_link_libraries(
        ${LIBRARY_NAME}
        ${Boost_LOG_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${FAIRROOT_LIBRARIES}
        ${ROOT_LIBRARIES}
)

# This is a make target, so you can do a "make projArunner"
set(APPLICATION_NAME_SENDER sender)
add_executable(${APPLICATION_NAME_SENDER} ${APP_SENDER_SOURCES})
set(APPLICATION_NAME_RECEIVER receiver)
add_executable(${APPLICATION_NAME_RECEIVER} ${APP_RECEIVER_SOURCES})

# Links the target exe (projArunner) against the library (libprojA) and other required libraries
target_link_libraries(${APPLICATION_NAME_SENDER} ${LIBRARY_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY}
        ${FAIRROOT_LIBRARIES}
        pthread
)
target_link_libraries(${APPLICATION_NAME_RECEIVER} ${LIBRARY_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY}
        ${FAIRROOT_LIBRARIES}
        pthread
)
# Says how and where to install software
install(TARGETS ${LIBRARY_NAME} ${APPLICATION_NAME_SENDER} ${APPLICATION_NAME_RECEIVER}
  EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
  ARCHIVE DESTINATION lib COMPONENT libs   # static lib
  RUNTIME DESTINATION bin COMPONENT libs   # binaries
  LIBRARY DESTINATION lib COMPONENT libs   # shared lib
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers (note the different component -> different package)
  FRAMEWORK DESTINATION bin COMPONENT libs # for mac
)

