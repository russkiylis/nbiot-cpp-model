add_executable(nbiotsync_tests
        test_NpssGenerator.cpp
        test_NsssGenerator.cpp
)

target_link_libraries(nbiotsync_tests PRIVATE
        nbiotsync
        nbiotmisc
        GTest::gtest_main
)

target_compile_definitions(nbiotsync_tests PRIVATE
        INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/input/"
)

include(GoogleTest)
gtest_discover_tests(nbiotsync_tests)