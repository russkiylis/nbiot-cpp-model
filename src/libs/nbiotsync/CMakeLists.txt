add_library(nbiotsync STATIC
    src/NpssGenerator.cpp src/NpssGenerator.h
    src/NsssGenerator.cpp src/NsssGenerator.h
)

target_include_directories(nbiotsync PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if(ENABLE_GTEST)
    add_subdirectory(tests)
endif()