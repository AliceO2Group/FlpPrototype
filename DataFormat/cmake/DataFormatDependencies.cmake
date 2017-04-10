find_package(Boost COMPONENTS unit_test_framework program_options REQUIRED)
find_package(Git QUIET)

o2_define_bucket(
        NAME
        o2_dataformat_bucket

        SYSTEMINCLUDE_DIRECTORIES
        ${Boost_INCLUDE_DIRS}
)
