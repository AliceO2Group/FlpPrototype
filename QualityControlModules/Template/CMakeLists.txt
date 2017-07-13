set(MODULE_NAME "QcTemplate")

O2_SETUP(NAME ${MODULE_NAME})

set(SRCS
        src/TemplateTask.cxx
        src/TemplateCheck.cxx
        )

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}/include
)

set(HEADERS
        include/Template/TemplateTask.h
        include/Template/TemplateCheck.h
        )

set(LIBRARY_NAME ${MODULE_NAME})
set(BUCKET_NAME o2_qcmodules_template)
set(LINKDEF include/Template/LinkDef.h)

O2_GENERATE_LIBRARY()

set(TEST_SRCS
        test/testQcTemplate.cxx
        )

O2_GENERATE_TESTS(
        MODULE_LIBRARY_NAME ${LIBRARY_NAME}
        BUCKET_NAME ${BUCKET_NAME}
        TEST_SRCS ${TEST_SRCS}
)
