# CMakeLists.txt для проекта NBIoT_CPP_Model.

# Требуется CMake версии 3.16 или выше.
cmake_minimum_required(VERSION 3.16)

# Определение проекта с его названием, версией, описанием и языком программирования.
project(NBIoT_CPP_Model
    VERSION 0.4.0
    DESCRIPTION "C++ реализация модели нисходящего канала NB-IoT."
    LANGUAGES CXX
)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}
        pipeline.cpp
        qml.qrc
)


find_package(Qt5 5.15 REQUIRED COMPONENTS
        Core Gui Quick Qml
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        nbiotbackend
        nbiotencoding
        nbiotmisc
        nbiotsync
        Qt5::Gui
        Qt5::Quick
        Qt5::Qml
)

# Добавление флагов компиляции
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Для Visual Studio
    add_compile_options(/W4)  # Уровень предупреждений 4
    add_compile_options(/utf-8)  # Этого достаточно, он сам включает оба
else()
    # Для GCC/Clang
    add_compile_options(
            -Wall
            -Wextra
            -Wimplicit-fallthrough
            -Wshadow
            -Wformat=2
            -Wundef
            -Wold-style-cast
    )
endif()

# Генерация Qt-кода нужна только приложению; на macOS собираем .app.
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
    MACOSX_BUNDLE TRUE
)

# Опция о том, собирать ли тесты
option(ENABLE_GTEST "Включить юнит-тестирование в сборку" ON)

if(ENABLE_GTEST)
    enable_testing()
    add_subdirectory(
        "${CMAKE_CURRENT_SOURCE_DIR}/../third-party/googletest"
        "${CMAKE_CURRENT_BINARY_DIR}/third-party/googletest"
        EXCLUDE_FROM_ALL
    )

    # Подавляем предупреждения только при сборке сторонних библиотек.
    # Их заголовки уже экспортируются GoogleTest как SYSTEM.
    foreach(_gtest_target IN ITEMS gtest gtest_main gmock gmock_main)
        if(TARGET ${_gtest_target})
            target_compile_options(${_gtest_target} PRIVATE
                "$<$<CXX_COMPILER_ID:MSVC>:/w>"
                "$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-w>"
            )
        endif()
    endforeach()
    unset(_gtest_target)
endif()

add_subdirectory(libs)
