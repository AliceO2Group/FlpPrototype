include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/include   # add the binary tree to the search path so that we will find Version.h
)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)