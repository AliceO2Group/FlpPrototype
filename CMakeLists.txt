# REFERENCE PROJECT CMAKELISTS.TXT
#
# @author  bvonhall

cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

enable_testing() # needed on top-level CMakeLists.txt

# Target doc definition
# The challenge here is that the sub projects must all define a custom target but with different names to avoid
# conflicts. We also want to be able to issue "make doc" from the top dir while being able to issue "make doc"
# in the subdirs *when* we compile only a subproject.
# We define as well a function AddDocTarget in cmake/DoxygenTarget.cmake
add_custom_target(doc DEPENDS docInfoLogger docCommon docControl docDataFormat docDataSampling docQualityControl docReadout ReadoutCard)

# Target coverage
set(CODE_COVERAGE FALSE CACHE BOOL "Turn on code coverage (build with not optimization!)")
if(CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE)
    set(CMAKE_BUILD_TYPE "Coverage" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel Coverage." FORCE)
    include(CodeCoverage)
    setup_target_for_coverage(coverage ctest coverage)
endif()

# Target uninstall definition
include(UninstallTarget)
# Same comments as for doc.
#add_custom_target(uninstall) # DEPENDS docProjA docProjB) Note: with CMake 3.X the DEPENDS would work
#ADD_DEPENDENCIES(uninstall uninstallInfoLogger uninstallProjA uninstallConfiguration uninstallDataFormat
#        uninstallDataSampling uninstallControl uninstallMonitoring uninstallQualityControl)

# Target clang-format
#include(clang-format)

macro(PrintModuleName name)
  message(STATUS \n-------- ${name} --------)
endmacro()

#PrintModuleName("Control")
#add_subdirectory(Control)
PrintModuleName("QualityControl")
add_subdirectory(QualityControl)
PrintModuleName("QualityControlModules")
add_subdirectory(QualityControlModules)
#PrintModuleName("GUI")
#add_subdirectory(GUI)
