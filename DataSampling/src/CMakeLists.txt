# @author  Barthélémy von Haller
set(LIB_HEADERS
  "${CMAKE_CURRENT_BINARY_DIR}/../include/DataSampling/Version.h" # Generated header
  ../include/DataSampling/DataBlockProducer.h
  ../include/DataSampling/FileSampler.h
  ../include/DataSampling/MockSampler.h
  ../include/DataSampling/SamplerInterface.h
)

set(LIB_SOURCES
  DataBlockProducer.cxx
  FileSampler.cxx
  MockSampler.cxx
  SamplerInterface.cxx
  InjectSamples.cxx
)



# LIBRARY

# Define a variable in order not to repeat ourselves.
set(LIBRARY_NAME DataSampling)

# Define the library
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})

# Set the build version. It will be used in the name of the lib, with corresponding
# symlinks created. SOVERSION could also be specified for api version. 
set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
  FRAMEWORK FALSE
  PUBLIC_HEADER "${LIB_HEADERS}" # specify the public headers
)



# BINARIES

set(APPLICATION_dataDump dataDump)
add_executable(${APPLICATION_dataDump} dataDump.cxx)
target_link_libraries(${APPLICATION_dataDump} ${LIBRARY_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY} DataFormat)

set(APPLICATION_fileProducer dataFileProducer)
add_executable(${APPLICATION_fileProducer} dataFileProducerMain.cxx)
target_link_libraries(${APPLICATION_fileProducer} ${LIBRARY_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY} DataFormat)



# Says how and where to install software
install(TARGETS ${LIBRARY_NAME} ${APPLICATION_dataDump} ${APPLICATION_fileProducer}
  EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
  ARCHIVE DESTINATION lib COMPONENT libs   # static lib
  RUNTIME DESTINATION bin COMPONENT libs   # binaries
  LIBRARY DESTINATION lib COMPONENT libs   # shared lib
  FRAMEWORK DESTINATION bin COMPONENT libs # for mac
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION include             # headers
)
