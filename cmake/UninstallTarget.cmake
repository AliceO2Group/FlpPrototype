# Note: we use a script cmake_uninstall.cmake.in. It is a bit circumvoluted. We have to, though,
# because we can't call a cmake function from add_custom_target.

set(UNINSTALL_TARGET_NAME "uninstall")
if (TARGET uninstall)
    set(UNINSTALL_TARGET_NAME "uninstall${PROJECT_NAME}")
endif ()
configure_file(
        "${CMAKE_CURRENT_LIST_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)
add_custom_target(${UNINSTALL_TARGET_NAME}
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)