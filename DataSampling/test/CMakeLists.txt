# @author  Barthélémy von Haller

# Compile C++ tests (here only one, but it will grab whatever cxx is in the test directory)
file(GLOB_RECURSE TEST_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cxx)

# Iterate over all tests found. For each, declare an executable and add it to the tests list.
set(ALL_TESTS)
foreach(FILE ${TEST_FILES})
    set_test_name(${FILE} NAME)
    add_executable(${NAME} ${FILE})
    target_link_libraries(${NAME} DataSampling ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${Boost_SYSTEM_LIBRARY_DEBUG} DataFormat )
    add_test(NAME ${NAME} COMMAND ${NAME})                              # this is how to add tests to CMake
    set_tests_properties(${NAME} PROPERTIES TIMEOUT 5)
endforeach(FILE ${TEST_FILES})

set(CTEST_TEST_TIMEOUT 10)
include(CTest)
