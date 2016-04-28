include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}/include   # add the binary tree to the search path so that we will find Version.h
)
add_subdirectory(include)