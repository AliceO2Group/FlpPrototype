# - Try to find Zookeeper
# Once done this will define
#  Zookeeper_FOUND        - System has Zookeeper
#  Zookeeper_INCLUDE_DIRS - The Zookeeper include directories
#  Zookeeper_LIBRARIES    - The libraries needed to use Zookeeper
#  Zookeeper_DEFINITIONS  - Compiler switches required for using LibZookeeper

find_package(PkgConfig)

#set(Zookeeper_PREFIX "/usr/local" CACHE PATH "A path where to look for zookeeper in addition to default paths.")

find_path(Zookeeper_INCLUDE_DIR zookeeper/zookeeper.h	PATHS /opt/zookeeper-3.4.6-install/include)
find_library(Zookeeper_LIBRARY NAMES zookeeper_mt PATHS  /opt/zookeeper-3.4.6-install/lib)

set(Zookeeper_LIBRARIES ${Zookeeper_LIBRARY} )
set(Zookeeper_INCLUDE_DIRS ${Zookeeper_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set Zookeeper_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Zookeeper DEFAULT_MSG Zookeeper_LIBRARY Zookeeper_INCLUDE_DIR)

# Because case is a problem some times
set(Zookeeper_FOUND ${ZOOKEEPER_FOUND})

mark_as_advanced(Zookeeper_INCLUDE_DIR Zookeeper_LIBRARY )