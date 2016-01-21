# REFERENCE PROJECT CMAKELISTS.TXT
#
# @author  bvonhall

cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

enable_testing() # needed on top-level CMakeLists.txt

# Target doc definition
# The challenge here is that the sub projects must all define a custom target but with different names to avoid
# conflicts. We also want to be able to issue "make doc" from the top dir while being able to issue "make doc"
# in the subdirs *when* we compile only a subproject.
# We define as well a function AddDocTarget in cmake/DoxygenTarget.cmake
add_custom_target(doc) # DEPENDS docProjA docProjB) Note: with CMake 3.X the DEPENDS would work
ADD_DEPENDENCIES(doc docInfoLogger docProjA docConfiguration docDataFormat docDataSampling docControl
        docMonitoring docQualityControl)

# Target uninstall definition
include(UninstallTarget)
# Same comments as for doc.
#add_custom_target(uninstall) # DEPENDS docProjA docProjB) Note: with CMake 3.X the DEPENDS would work
#ADD_DEPENDENCIES(uninstall uninstallInfoLogger uninstallProjA uninstallConfiguration uninstallDataFormat
#        uninstallDataSampling uninstallControl uninstallMonitoring uninstallQualityControl)

message(InfoLogger)
add_subdirectory(InfoLogger)
message(projectTemplate)
add_subdirectory(projectTemplate)
message(Configuration)
add_subdirectory(Configuration)
message(DataFormat)
add_subdirectory(DataFormat)
message(DataSampling)
add_subdirectory(DataSampling)
message(Control)
add_subdirectory(Control)
message(ControlFairMQ)
add_subdirectory(ControlFairMQ)
message(Monitoring)
add_subdirectory(Monitoring)
message(QualityControl)
add_subdirectory(QualityControl)
#message(Readout)
#add_subdirectory(Readout)
