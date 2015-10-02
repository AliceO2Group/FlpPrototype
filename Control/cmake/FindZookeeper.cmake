# - Try to find Zookeeper
# Once done this will define
#  Zookeeper_FOUND - System has Zookeeper
#  Zookeeper_INCLUDE_DIRS - The Zookeeper include directories
#  Zookeeper_LIBRARIES - The libraries needed to use Zookeeper
#  Zookeeper_DEFINITIONS - Compiler switches required for using LibZookeeper

find_package(PkgConfig)

find_path(Zookeeper_INCLUDE_DIR zookeeper.h
        PATH_SUFFIXES zookeeper)
find_library(Zookeeper_LIBRARY NAMES zookeeper_mt)

set(Zookeeper_LIBRARIES ${Zookeeper_LIBRARY} )
set(Zookeeper_INCLUDE_DIRS ${Zookeeper_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set Zookeeper_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Zookeeper  DEFAULT_MSG
        Zookeeper_LIBRARY Zookeeper_INCLUDE_DIR)

mark_as_advanced(Zookeeper_INCLUDE_DIR Zookeeper_LIBRARY )