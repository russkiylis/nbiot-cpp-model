# CMakeLists.txt для юнит-тестов проекта NBIoT_CPP_Model (Google Test).

# ============================== Установка Google Test ==============================
include(FetchContent)   # Включение модуля FetchContent для загрузки внешних зависимостей

# Загрузка Google Test из репозитория GitHub
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)

# Принудительное использование динамической библиотеки CRT для Google Test (важно для совместимости с MSVC)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest) # Загрузка и добавление Google Test в проект

# ============================== Создание тестов ==============================
file(GLOB_RECURSE TEST_SOURCES "test_*.cpp")  # Сбор всех файлов, начинающихся на test_*.cpp
add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})  # Создание исполня
target_link_libraries(${PROJECT_NAME}_tests PRIVATE
    nbiot_lib # Логика проекта
    GTest::gtest_main # Google Test
)

# ============================== Регистрация тестов в ctest==============================
include(GoogleTest) # Включение модуля GoogleTest для регистрации тестов
gtest_discover_tests(${PROJECT_NAME}_tests) # Автоматическое обнаружение и регистрация тестов в ctest