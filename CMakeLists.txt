# CMakeLists.txt для проекта NBIoT_CPP_Model.

# Требуется CMake версии 4.3.0 или выше.
cmake_minimum_required(VERSION 4.3.0) 

# Определение проекта с его названием, версией, описанием и языком программирования.
project(NBIoT_CPP_Model
    VERSION 0.2.0
    DESCRIPTION "C++ реализация модели нисходящего канала NB-IoT."
    LANGUAGES CXX
)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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

# Создание библиотеки с логикой проекта но без pipeline.cpp
file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")                                      # Сбор файлов проекта
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/pipeline.cpp")    # Удаление pipeline.cpp
add_library(nbiot_lib STATIC ${LIB_SOURCES})                                    # Создание библиотеки
target_include_directories(nbiot_lib PUBLIC "include")                          # Указание заголовочных файлов
target_compile_definitions(nbiot_lib PUBLIC                                     # Указание папки с внешними файлами
    INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/input/"
)


# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/pipeline.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE nbiot_lib)


# Тесты
if (ENABLE_GTEST)
    enable_testing()
    add_subdirectory(tests)
endif()

