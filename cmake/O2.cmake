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
        add_definitions(-std=c++11) # for CMake < 3.1
    else ()
        message(ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif ()
ELSE ()
    set(CMAKE_CXX_STANDARD 11)  # proper way in CMake >= 3.1
ENDIF ()

# Add compiler flags for warnings and (more importantly) fPIC
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -fPIC")

# Uninstall target (is it really a good idea ?)
set(UNINSTALL_TARGET_NAME "uninstall")
if (TARGET uninstall)
    set(UNINSTALL_TARGET_NAME "uninstall${PROJECT_NAME}")
endif ()
configure_file(
        "../cmake/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)
add_custom_target(${UNINSTALL_TARGET_NAME}
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

function(link_o2_subproject subproject_name)
    if (NOT TARGET ${subproject_name}) # target unknown because we build only this subproject, ProjA must have been built AND installed
        find_package(${subproject_name} CONFIG REQUIRED)
    else () # we know the target thus we are doing a build from the top directory
        include_directories(../${subproject_name}/include)
    endif ()
endfunction(link_o2_subproject)
