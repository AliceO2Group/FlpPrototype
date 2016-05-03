# @author  Barthélémy von Haller
set(LIB_HEADERS
  ../include/Example/ExampleTask.h # Normal header
)

set(LIB_SOURCES
  QcExampleDict.cxx # generated
  ExampleTask.cxx
)

# Define two variables in order not to repeat ourselves.
set(LIBRARY_NAME QcExample)

# DICTIONARIES
list(APPEND DICTINC ${CMAKE_CURRENT_SOURCE_DIR}/../include/Example/ExampleTask.h)
set(CMAKE_ROOTTEST_DICT 1)
ROOT_GENERATE_DICTIONARY("${LIBRARY_NAME}Dict" ${DICTINC} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/../include/Example/LinkDef.h)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}Dict_rdict.pcm DESTINATION lib)

# Define the library
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})
target_link_libraries(${LIBRARY_NAME} QCCore InfoLogger-static)

# Set the build version. It will be used in the name of the lib, with corresponding
# symlinks created. SOVERSION could also be specified for api version. 
set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
  FRAMEWORK FALSE
  PUBLIC_HEADER "${LIB_HEADERS}" # specify the public headers
)

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

