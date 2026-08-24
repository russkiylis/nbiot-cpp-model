# CMakeLists.txt для проекта NBIoT_CPP_Model.

# Для используемых возможностей проекта достаточно CMake 3.20.
cmake_minimum_required(VERSION 3.20)

# Определение проекта с его названием, версией, описанием и языком программирования.
project(NBIoT_CPP_Model
    VERSION 0.2.0
    DESCRIPTION "C++ реализация модели нисходящего канала NB-IoT."
    LANGUAGES CXX
)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Добавление флагов компиляции
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Для Visual Studio
    add_compile_options(/W4)  # Уровень предупреждений 4
    add_compile_options(/utf-8)  # Этого достаточно, он сам включает оба
else()
    # Для GCC/Clang
    add_compile_options(-Wall -Wextra)
endif()

# Опция о том, собирать ли тесты
option(ENABLE_GTEST "Включить юнит-тестировнание в сборку" ON)

# Явный список помогает не смешать библиотеку с двумя точками входа и показывает
# начинающему разработчику, из каких модулей состоит проект.
set(NBIOT_LIBRARY_SOURCES
    src/code/RateMatcher.cpp
    src/code/Scrambler.cpp
    src/code/crc16.cpp
    src/code/crc16_bitwise.cpp
    src/code/qpsk.cpp
    src/code/tbcc.cpp
    src/misc/BitUtils.cpp
    src/misc/ComplexSequenceReader.cpp
    src/misc/Correlator.cpp
    src/misc/NpssFileReader.cpp
    src/model/DownlinkManager.cpp
    src/model/Frame.cpp
    src/model/ModelConfig.cpp
    src/model/NrsOverlay.cpp
    src/model/ResourceGrid.cpp
    src/model/Subframe.cpp
    src/model/SubframeFactory.cpp
    src/model/Types.cpp
    src/model/subframes/EmptySubframe.cpp
    src/model/subframes/NpbchSubframe.cpp
    src/model/subframes/NpdcchSubframe.cpp
    src/model/subframes/NpdschSubframe.cpp
    src/model/subframes/NpssSubframe.cpp
    src/model/subframes/NsssSubframe.cpp
    src/sync/NpssGenerator.cpp
    src/sync/NsssGenerator.cpp
)

add_library(nbiot_lib STATIC ${NBIOT_LIBRARY_SOURCES})
target_include_directories(nbiot_lib PUBLIC "include")
target_compile_definitions(nbiot_lib PUBLIC
    INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/input/"
)

# Основное приложение показывает новую архитектурную топологию без запуска алгоритмов.
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE nbiot_lib)

# Старый вычислительный pipeline сохраняется как отдельный демонстрационный executable.
add_executable(NBIoT_Pipeline_Demo src/pipeline.cpp)
target_link_libraries(NBIoT_Pipeline_Demo PRIVATE nbiot_lib)

# Тесты
if (ENABLE_GTEST)
    enable_testing()
    add_subdirectory(tests)
    add_test(NAME NBIoT_CPP_Model.Smoke COMMAND ${PROJECT_NAME})
endif()
