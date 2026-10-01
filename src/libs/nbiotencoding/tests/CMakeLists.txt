add_executable(nbiotencoding_tests
        test_CRC16.cpp
        test_crc16_bitwise.cpp
        test_qpsk.cpp
        test_RateMatcher.cpp
        test_Scrambler.cpp
        test_ScramblerSingle.cpp
        test_tbcc.cpp
)

target_link_libraries(nbiotencoding_tests PRIVATE
        nbiotencoding
        nbiotmisc
        GTest::gtest_main
)

target_compile_definitions(nbiotencoding_tests PRIVATE
        INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/input/"
)

include(GoogleTest)
gtest_discover_tests(nbiotencoding_tests)