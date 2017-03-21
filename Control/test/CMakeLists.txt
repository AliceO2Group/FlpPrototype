# @author  Barthélémy von Haller

# Compile C++ tests (here only one, but it will grab whatever cxx is in the test directory)
#file(GLOB_RECURSE TEST_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cxx)
#
## Iterate over all tests found. For each, declare an executable and add it to the tests list.
#set(ALL_TESTS)
#foreach(FILE ${TEST_FILES})
#    message(${FILE})
#    set_test_name(${FILE} NAME)
#        message(${NAME})
#    add_executable(${NAME} ${FILE})
#    target_link_libraries(${NAME} zdaq StateMachine ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}) # link against our libs and boost test
#    add_test(NAME ${NAME} COMMAND ${NAME})                              # this is how to add tests to CMake
#endforeach(FILE ${TEST_FILES})

#add_executable(testZdaq_cxx testZdaq.cxx)
#target_link_libraries(testZdaq_cxx zdaq ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
#add_test(NAME testZdaq_cxx COMMAND testZdaq_cxx)

add_executable(testStateMachine_cxx testStateMachine.cxx)
target_link_libraries(testStateMachine_cxx StateMachine ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(NAME testStateMachine_cxx COMMAND testStateMachine_cxx)

add_executable(testSendCommand_cxx testSendCommand.cxx)
target_link_libraries(testSendCommand_cxx StateMachine ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(NAME testSendCommand_cxx COMMAND testSendCommand_cxx)

add_executable(testClient_cxx testClient.cxx)
target_link_libraries(testClient_cxx StateMachine ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(NAME testClient_cxx COMMAND testClient_cxx)

add_executable(testBaseControl_cxx testBaseControl.cxx)
target_link_libraries(testBaseControl_cxx StateMachine)

include(CTest)
