find_package(Boost 1.59 COMPONENTS thread system timer program_options random filesystem chrono exception regex serialization log log_setup unit_test_framework date_time REQUIRED)
find_package(FairRoot)
find_package(Common REQUIRED)

if(FAIRROOT_FOUND)
    # this should go away when fairrot provides a proper Find script or proper config scripts
    # See : http://www.cmake.org/cmake/help/v3.0/command/link_directories.html
    link_directories(${FAIRROOT_LIBRARY_DIR})
    set(FAIRROOT_LIBRARIES Base FairMQ BaseMQ)
    ADD_DEFINITIONS(-DWITH_FAIRROOT)
else(FAIRROT_FOUND)
    message(STATUS "FairRoot not found, corresponding classes will be skipped")
endif(FAIRROOT_FOUND)

find_package(Git QUIET)

o2_define_bucket(
  NAME
  o2_datasampling_bucket

  DEPENDENCIES
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_SYSTEM_LIBRARY_DEBUG}
  ${Boost_LOG_LIBRARY_DEBUG}
  ${Boost_FILESYSTEM_LIBRARY}
  InfoLogger
  ${Common_LIBRARIES}
  DataFormat

  SYSTEMINCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIRS}
  ${Common_INCLUDE_DIRS}
)

o2_define_bucket(
  NAME
  o2_datasampling_bucket_with_fair

  DEPENDENCIES
  o2_datasampling_bucket
  ${FAIRROOT_LIBRARIES}

  SYSTEMINCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIR}
  ${FAIRROOT_INCLUDE_DIR}
  ${FAIRROOT_INCLUDE_DIR}/fairmq
)
