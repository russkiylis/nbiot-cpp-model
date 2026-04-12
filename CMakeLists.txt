# CMakeLists.txt для проекта NBIoT_CPP_Model.

# Требуется CMake версии 4.3.0 или выше.
cmake_minimum_required(VERSION 4.3.0) 

# Определение проекта с его названием, версией, описанием и языком программирования.
project(NBIoT_CPP_Model
    VERSION 0.1.0
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

# Сбор файлов исходного кода
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Создание исполняемого файла из исходного кода
add_executable(${PROJECT_NAME} ${SOURCES})

# Указание места поиска заголовочных файлов
target_include_directories(${PROJECT_NAME} PRIVATE "include")

# Путь к директории с входными данными
target_compile_definitions(${PROJECT_NAME} PRIVATE
    INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/input/"
)

