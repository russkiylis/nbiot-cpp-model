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
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Сбор файлов исходного кода
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Создание исполняемого файла из исходного кода
add_executable(${PROJECT_NAME} ${SOURCES})

# Указание места поиска заголовочных файлов
target_include_directories(${PROJECT_NAME} PRIVATE "include")
