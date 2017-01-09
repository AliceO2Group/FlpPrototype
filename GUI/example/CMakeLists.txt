add_executable(1-Publisher 1-Publisher.cxx)
target_link_libraries(1-Publisher GUI)

add_executable(2-RequestNotification 2-RequestNotification.cxx)
target_link_libraries(2-RequestNotification GUI)

add_executable(3-FairOnData 3-FairOnData.cxx)
target_link_libraries(3-FairOnData ${FAIRROOT_LIBRARIES} ${Boost_LOG_LIBRARY} ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${FAIRROOT_LIBRARIES}    
    pthread)
