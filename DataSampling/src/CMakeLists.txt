# @author  Barthélémy von Haller
set(LIB_HEADERS
  "${CMAKE_CURRENT_BINARY_DIR}/../include/DataSampling/Version.h" # Generated header
  ../include/DataSampling/DataBlockProducer.h
  ../include/DataSampling/FileSampler.h
  ../include/DataSampling/MockSampler.h
        ../include/DataSampling/FairSampler.h
  ../include/DataSampling/SamplerInterface.h
        ../include/DataSampling/SamplerFactory.h
        ../include/DataSampling/InjectorInterface.h
        ../include/DataSampling/MockInjector.h
        ../include/DataSampling/InjectorFactory.h
        ../include/DataSampling/DsInfoLogger.h
)

set(LIB_SOURCES
  DataBlockProducer.cxx
  FileSampler.cxx
  MockSampler.cxx
        FairSampler.cxx
  SamplerInterface.cxx
        SamplerFactory.cxx
        MockInjector.cxx
        InjectorFactory.cxx
)

if (FAIRROOT_FOUND)
    set(LIB_HEADERS ${LIB_HEADERS} ../include/DataSampling/FairInjector.h)
    set(LIB_SOURCES ${LIB_SOURCES} FairInjector.cxx)
endif (FAIRROOT_FOUND)

# LIBRARY

# Define a variable in order not to repeat ourselves.
set(LIBRARY_NAME DataSampling)

# Define the library
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})
target_link_libraries(${LIBRARY_NAME} ${FAIRMQ_LIBRARIES} Common InfoLogger)

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

set(APPLICATION_mockFairInjector mockFairInjector)
add_executable(${APPLICATION_mockFairInjector} mockFairInjector.cxx)
target_link_libraries(${APPLICATION_mockFairInjector} ${LIBRARY_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_SYSTEM_LIBRARY_DEBUG} DataFormat)

set(APPLICATION_mockFairSampler mockFairSampler)
add_executable(${APPLICATION_mockFairSampler} mockFairSampler.cxx)
target_link_libraries(${APPLICATION_mockFairSampler} ${LIBRARY_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_SYSTEM_LIBRARY_DEBUG} DataFormat)

# Says how and where to install software
install(TARGETS ${LIBRARY_NAME} ${APPLICATION_dataDump} ${APPLICATION_fileProducer} ${APPLICATION_mockFairInjector} ${APPLICATION_mockFairSampler}
  EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
  ARCHIVE DESTINATION lib COMPONENT libs   # static lib
  RUNTIME DESTINATION bin COMPONENT libs   # binaries
  LIBRARY DESTINATION lib COMPONENT libs   # shared lib
  FRAMEWORK DESTINATION bin COMPONENT libs # for mac
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION include             # headers
)
