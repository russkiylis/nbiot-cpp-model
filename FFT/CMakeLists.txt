cmake_minimum_required(VERSION 3.21)

project(FFT VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(Qt6_DIR "C:/Qt/6.11.1/mingw_64/lib/cmake/Qt6")

find_package(Qt6 6.11.1 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Graphs)

qt_standard_project_setup()
qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

set(CORE_SOURCES
    src/core/BitUtils.cpp
    src/core/FourierTransform.cpp
    src/core/Modulation.cpp
    src/core/OfdmSignal.cpp
    src/core/SignalStats.cpp
)

set(CORE_HEADERS
    src/core/BitUtils.h
    src/core/ComplexNumber.h
    src/core/FourierTransform.h
    src/core/OfdmSignal.h
    src/core/Modulation.h
    src/core/SignalStats.h
)

set(UI_SOURCES
    src/ui/GraphDataProvider.cpp
)

set(UI_HEADERS
    src/ui/GraphDataProvider.h
)

qt_add_executable(FFT
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    ${UI_SOURCES}
    ${UI_HEADERS}
    src/app/main.cpp
)

add_executable(OfdmDebug
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    src/debug/OfdmDebug.cpp
)

qt_add_qml_module(FFT
    URI FFTApp
    VERSION 1.0
    QML_FILES
        src/ui/qml/Main.qml
)

target_include_directories(FFT
    PRIVATE
        src/core
        src/ui
)

target_include_directories(OfdmDebug
    PRIVATE
        src/core
)

target_link_libraries(FFT
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Graphs
)

if (MSVC)
    target_compile_options(FFT PRIVATE /utf-8)
endif()
