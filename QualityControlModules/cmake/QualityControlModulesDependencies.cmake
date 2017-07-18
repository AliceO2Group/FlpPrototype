find_package(Boost COMPONENTS unit_test_framework program_options REQUIRED)
find_package(Configuration REQUIRED)
find_package(Monitoring REQUIRED)
find_package(FairRoot)
if(FAIRROOT_FOUND)
    link_directories(${FAIRROOT_LIBRARY_DIR})
    include_directories(${FAIRROOT_INCLUDE_DIR})
else(FAIRROOT_FOUND)
    message(WARNING "FairRoot not found, corresponding classes will not be compiled.")
endif(FAIRROOT_FOUND)

find_package(ROOT 6.06.02 COMPONENTS RHTTP RMySQL Gui)
if(ROOT_FOUND)
    message(STATUS "ROOT ${ROOT_VERSION} found")
else()
    return()
endif()

# todo just a quick fix to get the dictionary working . to be revisited when extracting
list(APPEND GLOBAL_ALL_MODULES_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../QualityControl/include)
list(APPEND GLOBAL_ALL_MODULES_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../DataFormat/include)

o2_define_bucket(
  NAME
  o2_qcmodules_common

  DEPENDENCIES
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  InfoLogger
  QualityControl
  ${Monitoring_LIBRARIES}

  SYSTEMINCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIRS}
  ${Monitoring_INCLUDE_DIRS}
)

o2_define_bucket(
    NAME
    o2_qcmodules_example

    DEPENDENCIES
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    InfoLogger
    QualityControl
    DataFormat

    SYSTEMINCLUDE_DIRECTORIES
    ${Boost_INCLUDE_DIRS}
    ${Configuration_INCLUDE_DIRS}

)

o2_define_bucket(
    NAME
    o2_qcmodules_template

    DEPENDENCIES
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    InfoLogger
    QualityControl
    DataFormat

    SYSTEMINCLUDE_DIRECTORIES
    ${Boost_INCLUDE_DIRS}
    ${Configuration_INCLUDE_DIRS}
)