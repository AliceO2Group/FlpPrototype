# Set default installation path for O2
set(O2_INSTALL_PREFIX "/usr/local")

# Overwrite CMAKE_INSTALL_PREFIX
IF (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET(CMAKE_INSTALL_PREFIX
            ${O2_INSTALL_PREFIX} CACHE PATH "O2 install prefix" FORCE
            )
ENDIF (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# C++11
IF (CMAKE_VERSION VERSION_LESS 3.1)
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG(-std=c++11 COMPILER_SUPPORTS_CXX11)
    if (COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else ()
        message(ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif ()
ELSE ()
    set(CMAKE_CXX_STANDARD 11)  # proper way in CMake >= 3.1
ENDIF ()

# Add compiler flags for warnings and (more importantly) fPIC and debug symbols
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -fPIC -std=c99")

# Set the default build type to "RelWithDebInfo"
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel Coverage."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Code coverage
set(CODE_COVERAGE FALSE CACHE BOOL "Turn on code coverage (build with not optimization!)")
if(CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE)
    message(STATUS "Code coverage turned on, force the build type to Coverage (no optimization)")
    set(CMAKE_BUILD_TYPE "Coverage" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel Coverage." FORCE)
#    SET(CODE_COVERAGE_EXCLUSION_LIST "'include/*' 'build/*' 'test/*'" PARENT_SCOPE) # this does not work yet
  
    # Set the name of the target : "doc" if it doesn't already exist and "doc<projectname>" if it does.
    # This way we make sure to have a single "doc" target. Either it is the one of the top directory or
    # it is the one of the subproject that we are compiling alone.
    set(COVERAGE_TARGET_NAME "coverage")
    if(TARGET coverage)
      set(COVERAGE_TARGET_NAME "coverage${PROJECT_NAME}")
    endif()
    
    include(CodeCoverage)
    setup_target_for_coverage(${COVERAGE_TARGET_NAME} "ctest -j4" coverage)
endif()

# Uninstall target (is it really a good idea ?)
include(UninstallTarget)

# Function to link between sub-projects
function(link_o2_subproject subproject_name)
    #if (NOT TARGET ${subproject_name}) # target unknown
    if(NOT PROJECT_${subproject_name}) # var unknown because we build only this subproject, ProjA must have been built AND installed
        find_package(${subproject_name} CONFIG REQUIRED)
        if(${subproject_name}_FOUND) 
            message(STATUS "${subproject_name} found : ${${subproject_name}_FOUND}, ${${subproject_name}_INCLUDE_DIRS}")
        endif()
    else () # we know the target thus we are doing a build from the top directory
        include_directories(../${subproject_name}/include)
    endif ()
endfunction(link_o2_subproject)

# Make sure we tell the topdir CMakeLists that we exist (if build from topdir)
get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
    set(PROJECT_${PROJECT_NAME} true PARENT_SCOPE)
endif()