add_library(nbiotarchitecture STATIC
    src/FrameGen.cpp src/FrameGen.h
    src/SubframeGen.cpp src/SubframeGen.h
    src/ResourceGrid.h
    src/Config.h
)

target_include_directories(nbiotarchitecture PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(nbiotarchitecture
        PRIVATE nbiotsync nbiotmisc nbiotencoding
)