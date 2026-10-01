add_library(nbiotmisc STATIC
    src/BitUtils.cpp src/BitUtils.h
    src/ComplexSequenceReader.cpp src/ComplexSequenceReader.h
    src/Correlator.cpp src/Correlator.h
    src/NpssFileReader.cpp src/NpssFileReader.h
)

target_include_directories(nbiotmisc PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)