# @author  Barthélémy von Haller
set(LIB_HEADERS
  "${CMAKE_CURRENT_BINARY_DIR}/../include/DataFormat/Version.h" # Generated header
  ../include/DataFormat/DataBlock.h
  ../include/DataFormat/DataBlockContainer.h
  ../include/DataFormat/DataSet.h
  ../include/DataFormat/MemPool.h
)

set(LIB_SOURCES
        DataBlock.cxx
        DataBlockContainer.cxx
        MemPool.cxx
)

# IMPORTANT NOTE :
# This is a dummy library as we have a header only library.
# Due to a limitation of CMake < 3.1 it is easier like that. Later we must use "library interface" feature.

# Define two variables in order not to repeat ourselves.
set(LIBRARY_NAME DataFormat)

#include(GenerateExportHeader)

# Define the library
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})

#generate_export_header(${LIBRARY_NAME})

# Set the build version. It will be used in the name of the lib, with corresponding
# symlinks created. SOVERSION could also be specified for api version. 
set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
  FRAMEWORK FALSE
  PUBLIC_HEADER "${LIB_HEADERS}" # specify the public headers
)

# Says how and where to install software
# Targets:
#   * <prefix>/lib/<libraries>
#   * header location after install: <prefix>/include/<project>/*.h
#   * headers can be included by C++ code `#<project>/Bar.hpp>`
install(TARGETS ${LIBRARY_NAME}
  EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
  ARCHIVE DESTINATION lib COMPONENT libs   # static lib
  RUNTIME DESTINATION bin COMPONENT libs   # binaries
  LIBRARY DESTINATION lib COMPONENT libs   # shared lib
  FRAMEWORK DESTINATION bin COMPONENT libs # for mac
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION include             # headers
)

