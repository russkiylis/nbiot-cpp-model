add_library(nbiotencoding STATIC
    src/crc16.cpp src/crc16.h
    src/crc16_bitwise.cpp src/crc16_bitwise.h
    src/qpsk.cpp src/qpsk.h
    src/RateMatcher.cpp src/RateMatcher.h
    src/Scrambler.cpp src/Scrambler.h
    src/tbcc.cpp src/tbcc.h
)

target_include_directories(nbiotencoding PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if(ENABLE_GTEST)
    add_subdirectory(tests)
endif()