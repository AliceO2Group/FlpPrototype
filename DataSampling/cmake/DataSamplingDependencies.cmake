find_package(Boost 1.41 COMPONENTS unit_test_framework program_options system REQUIRED)
find_package(FairRoot)
find_package(Git QUIET)

o2_define_bucket(
  NAME
  o2_datasampling_bucket

  DEPENDENCIES
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_SYSTEM_LIBRARY_DEBUG}
  InfoLogger
  Common
  DataFormat

  SYSTEMINCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIRS}
)

if(FAIRROOT_FOUND)
    find_library(FAIRMQ_LIBRARIES_1 NAMES Base PATHS ${FAIRROOT_LIBRARY_DIR})
    find_library(FAIRMQ_LIBRARIES_2 NAMES FairMQ PATHS ${FAIRROOT_LIBRARY_DIR})
    find_library(FAIRMQ_LIBRARIES_3 NAMES BaseMQ PATHS ${FAIRROOT_LIBRARY_DIR})
    set(FAIRMQ_LIBRARIES ${FAIRMQ_LIBRARIES_1} ${FAIRMQ_LIBRARIES_2} ${FAIRMQ_LIBRARIES_3})
    message(INFO "FAIRMQ_LIBRARIES : ${FAIRMQ_LIBRARIES}")
    include_directories(${FAIRROOT_INCLUDE_DIR})
else(FAIRROOT_FOUND)
    message(WARNING "FairRoot not found, corresponding classes will not be compiled.")
endif(FAIRROOT_FOUND)

o2_define_bucket(
  NAME
  o2_datasampling_bucket_with_fair

  DEPENDENCIES
  o2_datasampling_bucket
  ${FAIRMQ_LIBRARIES}

  SYSTEMINCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIR}
)
