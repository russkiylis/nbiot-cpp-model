add_library(nbiotbackend STATIC
        src/consolebackend.cpp
        src/consolebackend.h
        src/configbackend.h
        src/backendcontroller.h
)

target_include_directories(nbiotbackend PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(nbiotbackend
        PUBLIC Qt5::Core
        PRIVATE nbiotsync nbiotmisc nbiotencoding
)

set_target_properties(nbiotbackend PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
)