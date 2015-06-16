# @author  Barthélémy von Haller

set(HEADERS
)

set(SOURCES
  main.cpp
)

# Executable name then used to add an executable to the project.
# This is a make target, so you can do a "make hellorunner"
set(APPLICATION hellorunner)
add_executable(${APPLICATION} ${HEADERS} ${SOURCES})

# Links the target exe (hellorunner) against the library (libhello)
target_link_libraries(${APPLICATION} hello ${Boost_PROGRAM_OPTIONS_LIBRARY})

# Says how and where to install software
install(TARGETS ${APPLICATION} DESTINATION bin COMPONENT apps)
