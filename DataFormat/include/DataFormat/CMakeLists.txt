# @author  Barthélémy von Haller

# Produce the final Version.h using template Version.h.in and substituting variables.
# We don't want to polute our source tree with it, thus putting it in binary tree.
configure_file("Version.h.in" "${CMAKE_CURRENT_BINARY_DIR}/Version.h" @ONLY)

set(LIB_HEADERS
        "${CMAKE_CURRENT_BINARY_DIR}/Version.h" # Generated header
        DataBlock.h # Normal header
        )

# Define two variables in order not to repeat ourselves.
set(LIBRARY_NAME DataFormat)

# Define the library
add_library(${LIBRARY_NAME} INTERFACE)

# Says how and where to install software (header only library here)
install(TARGETS ${LIBRARY_NAME}
        EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
        )
install(FILES
        ${LIB_HEADERS}
        DESTINATION include/${PROJECT_NAME} COMPONENT devel
        )