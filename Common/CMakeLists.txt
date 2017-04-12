####################################
# General project definition
####################################

CMAKE_MINIMUM_REQUIRED(VERSION 3.5.2 FATAL_ERROR)

### CMP0025   Compiler id for Apple Clang is now AppleClang.
### CMP0042   MACOSX_RPATH is enabled by default.

FOREACH (p
        CMP0025 # CMake 3.0
        CMP0042 # CMake 3.0
        )
    IF (POLICY ${p})
        cmake_policy(SET ${p} NEW)
    ENDIF ()
endforeach ()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # project specific cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)

project(Common)

# Load some basic macros which are needed later on
include(O2Utils)
include(CommonDependencies)

# Set the default build type to "RelWithDebInfo"
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo"
            CACHE
            STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel Coverage."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Set the version number of your project here (format is MAJOR.MINOR.PATCHLEVEL - e.g. 1.0.0)
set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# C++14
IF (CMAKE_VERSION VERSION_LESS 3.1)
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG(-std=c++14 COMPILER_SUPPORTS_CXX14)
    if (COMPILER_SUPPORTS_CXX14)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    else ()
        message(ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
    endif ()
ELSE ()
    set(CMAKE_CXX_STANDARD 14)  # proper way in CMake >= 3.1
ENDIF ()

# Add compiler flags for warnings and (more importantly) fPIC and debug symbols
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -fPIC")


####################################
# Module, library and executable definition
####################################

set(MODULE_NAME "Common")

O2_SETUP(NAME ${MODULE_NAME})

set(SRCS
  src/Daemon.cxx
  src/Exception.cxx
  src/Iommu.cxx
  src/LineBuffer.cxx
  src/Program.cxx
  src/SimpleLog.cxx
  src/SuffixNumber.cxx
  src/System.cxx
  src/Thread.cxx
  src/Timer.cxx
  src/Configuration.cxx
  )

# Produce the final Version.h using template Version.h.in and substituting variables.
# We don't want to polute our source tree with it, thus putting it in binary tree.
configure_file("include/${MODULE_NAME}/Version.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/include/${MODULE_NAME}/Version.h"
        @ONLY
        )

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}/include
)

set(LIBRARY_NAME ${MODULE_NAME})

set(BUCKET_NAME o2_common_bucket)

O2_GENERATE_LIBRARY()

add_subdirectory(doc)

set(TEST_SRCS
  test/TestBasicThread.cxx
  test/testFifo.cxx
  test/TestIommu.cxx
  test/TestSuffixNumber.cxx
  test/TestSuffixOption.cxx
  test/TestSystem.cxx
  test/testTimer.cxx
)

O2_GENERATE_TESTS(
 MODULE_LIBRARY_NAME ${LIBRARY_NAME}
 BUCKET_NAME ${BUCKET_NAME}
 TEST_SRCS ${TEST_SRCS}
)

# Library for Tcl
# Usually we don't allow multiple libraries in O2 modules. This is an exception.
if (TCL_FOUND)
  set(SRCS
      src/Configuration.cxx
      src/Configuration_tcl_wrap.cxx
  )
  set(LIBRARY_NAME tclConfiguration)
  O2_GENERATE_LIBRARY()
endif()

# TODO RECONSIDER THE THINGS BELOW AFTER EXTRACTION TO ITS OWN REPO
# Make sure we tell the topdir CMakeLists that we exist (if build from topdir)
get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
    set(PROJECT_${PROJECT_NAME} true PARENT_SCOPE)
endif()
include(PackageConfigurator)
