# @author  Barthélémy von Haller
# @author  Vasco Barroso
set(LIB_HEADERS
  "${CMAKE_CURRENT_BINARY_DIR}/../include/ControlFairMQ/Version.h" # Generated header
  ../include/ControlFairMQ/Sender.h 
  ../include/ControlFairMQ/Receiver.h 
)

set(LIB_SOURCES
  Sender.cxx
  Receiver.cxx
)

set(APP_HEADERS
)

set(APP_SOURCES
)

# Define the library
set(LIBRARY_NAME ControlFairMQ)
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})
target_link_libraries(${LIBRARY_NAME} 
	InfoLogger-static
	${Boost_LOG_LIBRARY}
	${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${FAIRROOT_LIBRARIES}    
    pthread    		
)

# Set the build version. It will be used in the name of the lib, with corresponding
# symlinks created. SOVERSION could also be specified for api version. 
set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
  FRAMEWORK FALSE
  PUBLIC_HEADER "${LIB_HEADERS}" # specify the public headers
)

# BINARIES

# sender example
set(APPLICATION_SENDER_NAME launchSender)
add_executable(${APPLICATION_SENDER_NAME} launchSender)
target_link_libraries(${APPLICATION_SENDER_NAME} ${LIBRARY_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY})

# receiver example
set(APPLICATION_RECEIVER_NAME launchReceiver)
add_executable(${APPLICATION_RECEIVER_NAME} launchReceiver)
target_link_libraries(${APPLICATION_RECEIVER_NAME} ${LIBRARY_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY})

# Says how and where to install software
install(TARGETS ${LIBRARY_NAME} ${APPLICATION_SENDER_NAME} ${APPLICATION_RECEIVER_NAME}
  EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
  ARCHIVE DESTINATION lib COMPONENT libs   # static lib
  RUNTIME DESTINATION bin COMPONENT libs   # binaries
  LIBRARY DESTINATION lib COMPONENT libs   # shared lib
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers (note the different component -> different package)
  FRAMEWORK DESTINATION bin COMPONENT libs # for mac
)

