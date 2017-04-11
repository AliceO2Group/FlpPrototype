find_package(Boost COMPONENTS unit_test_framework program_options REQUIRED)
find_package(Git QUIET)
find_package(TCL)


o2_define_bucket(
  NAME
  o2_common_bucket

  DEPENDENCIES
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}

  SYSTEMINCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIRS}
)
