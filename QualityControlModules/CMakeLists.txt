# REFERENCE PROJECT CMAKELISTS.TXT
#
# @author Barthélémy von Haller

cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake) # main (top) cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # project specific cmake dir

# CMake useful variables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib") 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Set the name of your project here
project("QualityControlModules")

# Set the version number of your project here (format is MAJOR.MINOR.PATCHLEVEL - e.g. 1.0.0)
set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# Macros and variables for all O2 subprojects
include(O2)

## Git (and its revision)
find_package(Git QUIET) # if we don't find git or FindGit.cmake is not on the system we ignore it.
## GetGitRevisionDescription module to retreive branch and revision information from Git
## Starting with Git 1.9 the module will be part of official cMake distribution, until then it has to be
## part of the application
## The Git module will trigger a reconfiguration for each pull that will bring a new revision on the local repository
set (VCS_REVISION "-1")
if(GIT_FOUND)
    include(GetGitRevisionDescription)
    get_git_head_revision(GIT_REFSPEC GIT_SHA1)
    message(STATUS "GIT branch ${GIT_REFSPEC}")
    message(STATUS "GIT revision ${GIT_SHA1}")
    set (VCS_REVISION ${GIT_SHA1})
endif()

# This line finds the boost lib and headers. 
set(Boost_NO_BOOST_CMAKE ON) #  Don't do a find_package in config mode before searching for a regular boost install.
find_package(Boost COMPONENTS unit_test_framework program_options REQUIRED)

find_package(FairRoot)
if(FAIRROOT_FOUND)
    # this should go away when fairrot provides a proper Find script or proper config scripts
    # See : http://www.cmake.org/cmake/help/v3.0/command/link_directories.html
    link_directories(${FAIRROOT_LIBRARY_DIR})
    include_directories(
      ${FAIRROOT_INCLUDE_DIR}
    )
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

# Dependency on another subproject (InfoLogger)
link_o2_subproject(InfoLogger)
link_o2_subproject(Common)
link_o2_subproject(Configuration)
link_o2_subproject(DataFormat)
link_o2_subproject(DataSampling)
link_o2_subproject(Monitoring)
link_o2_subproject(QualityControl)

# This sets the include directory for the reference project. This is the -I flag in gcc.
# All the includes should be in this variable To add new directories to the include path,
# just add them in this list (after a space), or add an include_directories statement in a
# subdirectory's CMakeLists.
include_directories(
    ${Boost_INCLUDE_DIRS}
  ${ROOT_INCLUDE_DIRS}
)

# Mac needed variables (adapt according to your needs)
set(CMAKE_MACOSX_RPATH ON)
#set(CMAKE_SKIP_BUILD_RPATH FALSE)
#set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
#set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(ROOT_FOUND AND FAIRROOT_FOUND)
  enable_testing()

  add_subdirectory(Common)
  add_subdirectory(Example)
  #add_subdirectory(ITS)

  #
  # SYSTEM PACKAGING (RPM, TGZ, ...)
  # _____________________________________________________________________________

  include(CPackConfig)

  #
  # CMAKE PACKAGING (for other CMake projects to use this one easily)
  # _____________________________________________________________________________

  include(PackageConfigurator)
endif()
