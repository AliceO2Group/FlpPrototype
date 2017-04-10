find_package(Boost COMPONENTS unit_test_framework program_options REQUIRED)
find_package(Configuration REQUIRED)
find_package(FairRoot)
if(FAIRROOT_FOUND)
    link_directories(${FAIRROOT_LIBRARY_DIR})
    include_directories(${FAIRROOT_INCLUDE_DIR})
else(FAIRROOT_FOUND)
    message(WARNING "FairRoot not found, corresponding classes will not be compiled.")
endif(FAIRROOT_FOUND)

find_package(ROOT)
if(ROOT_FOUND)
    message(STATUS "ROOT ${ROOT_VERSION} found")
    include(RootNewMacros)
else()
    message(WARNING "ROOT not found, we won't compile the QC modules (skip, no error)")
endif()

o2_define_bucket(
  NAME
  o2_qcmodules_common

  DEPENDENCIES
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  QCCore
  InfoLogger

  SYSTEMINCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIRS}
)
