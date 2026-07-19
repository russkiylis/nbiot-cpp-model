
#include <gtest/gtest.h>
#include "code/RateMatcher.h"
#include "misc/BitUtils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

TEST(test_RateMatcher, All10000Sequences) {
    std::string path150 = std::string(INPUT_DIR) + "random_34bit_tbcc.txt";
    std::string path1600 = std::string(INPUT_DIR) + "random_34bit_ratematcher.txt";
    
    std::ifstream f150(path150);
    std::ifstream f1600(path1600);
    
    ASSERT_TRUE(f150.is_open()) << "Файл не найден: " << path150;
    ASSERT_TRUE(f1600.is_open()) << "Файл не найден: " << path1600;
    
    std::string s150, s1600;
    int failed = 0;
    int lineCount = 0;
    
    // Создаём объект RateMatcher
    RateMatcher rm;
    
    // Векторы для хранения первых 5 строк
    std::vector<std::string> first5_s150;
    std::vector<std::string> first5_s1600;
    std::vector<std::string> first5_result;
    std::vector<bool> first5_status;
    
    while (std::getline(f150, s150) && std::getline(f1600, s1600)) {
        lineCount++;
        
        // Удаляем \r для Windows-файлов
        if (!s150.empty() && s150.back() == '\r') s150.pop_back();
        if (!s1600.empty() && s1600.back() == '\r') s1600.pop_back();
        
        // Проверяем длину
        ASSERT_EQ(s150.length(), 150) << "Строка " << lineCount << " имеет длину " << s150.length();
        ASSERT_EQ(s1600.length(), 1600) << "Строка " << lineCount << " имеет длину " << s1600.length();
        
        // Конвертируем 150 бит в массив 3x50
        std::vector<std::vector<uint8_t>> input(3, std::vector<uint8_t>(50));
        for (int i = 0; i < 150; i++) {
            int stream = i % 3;
            int pos = i / 3;
            input[stream][pos] = (s150[i] == '1') ? 1 : 0;
        }
        
        // Применяем Rate Matcher
        auto result = rm.rateMatch(input, 1600);
        std::string result_str = BitUtils::bitVectorToString(result);
        
        // Проверка размера
        EXPECT_EQ(result_str.length(), 1600) << "Строка " << lineCount << " имеет длину " << result_str.length();
        
        // Сохраняем первые 5 строк для вывода после итога
        if (lineCount <= 5) {
            first5_s150.push_back(s150);
            first5_s1600.push_back(s1600);
            first5_result.push_back(result_str);
            first5_status.push_back(result_str == s1600);
        }
        
        if (result_str != s1600) {
            failed++;
        }
    }
    
    f150.close();
    f1600.close();
    
    // ========== СНАЧАЛА ВЫВОДИМ ИТОГ ==========
    std::cout << "\n=== ИТОГ ===\n";
    std::cout << "Всего строк: " << lineCount << "\n";
    std::cout << "Не прошло: " << failed << "\n";
    std::cout << "Прошло: " << (lineCount - failed) << "\n";
    if (lineCount > 0) {
        std::cout << "Процент: " << (100.0 * (lineCount - failed) / lineCount) << "%\n";
    }
    
    // ========== ПОТОМ ПЕРВЫЕ 5 СТРОК ==========
    std::cout << "\n=== ПЕРВЫЕ 5 РЕЗУЛЬТАТОВ RATE MATCHER ===\n";
    for (size_t i = 0; i < first5_s150.size(); i++) {
        std::cout << "\n[" << i+1 << "]\n";
        std::cout << "  150 бит (вход): " << first5_s150[i] << "\n";
        
        std::string first10_expected = first5_s1600[i].substr(0, 10);
        std::string last10_expected = first5_s1600[i].substr(first5_s1600[i].length() - 10);
        std::string first10_result = first5_result[i].substr(0, 10);
        std::string last10_result = first5_result[i].substr(first5_result[i].length() - 10);
        
        std::cout << "  Ожидается: " << first10_expected << "..." << last10_expected << "\n";
        std::cout << "  Вычислено: " << first10_result << "..." << last10_result << "\n";
        std::cout << "  Статус:  " << (first5_status[i] ? "OK" : "FAIL") << "\n";
        
        if (!first5_status[i]) {
            for (size_t j = 0; j < 1600; j++) {
                if (first5_result[i][j] != first5_s1600[i][j]) {
                    std::cout << "  Первая ошибка на позиции " << j << "\n";
                    std::cout << "  Ожидалось: " << first5_s1600[i][j] << ", получено: " << first5_result[i][j] << "\n";
                    break;
                }
            }
        }
    }
    
    EXPECT_EQ(failed, 0) << "Не прошло " << failed << " из " << lineCount;
}

// Дополнительный тест: проверка одной конкретной строки
TEST(test_RateMatcher, SingleSequence) {
    // Вход: 150 бит (первая строка из TBCC-файла)
    std::string input = "000000101010101100101010001011111011100010011000100100110011001101100001010111001000101000100001011101010110111100001100001001100000100100110011110001";
    
    // Ожидаемый выход (первые 100 бит из random_34bit_ratematcher.txt)
    std::string expected = "0010000010001110011000110110011110101011111000001111101001000100001011000001010100000110010001000101";
    
    RateMatcher rm;
    
    // Конвертируем входную строку в массив 3x50
    std::vector<std::vector<uint8_t>> arr(3, std::vector<uint8_t>(50));
    for (int i = 0; i < 150; i++) {
        int stream = i % 3;
        int pos = i / 3;
        arr[stream][pos] = (input[i] == '1') ? 1 : 0;
    }
    
    // Применяем Rate Matcher
    auto result = rm.rateMatch(arr, 1600);
    std::string result_str = BitUtils::bitVectorToString(result);
    
    // Проверка длины
    ASSERT_EQ(result_str.length(), 1600) << "Ожидается 1600 бит, получено " << result_str.length();
    
    // Сравниваем первые 100 бит (для наглядности)
    std::string result_first100 = result_str.substr(0, 100);
    
    // Вывод первых 100 бит для отладки
    std::cout << "\n=== SingleSequence Test ===\n";
    std::cout << "Первые 100 бит вычислено: " << result_first100 << "\n";
    std::cout << "Первые 100 бит ожидается: " << expected << "\n";
    
    EXPECT_EQ(result_first100, expected) << "Первые 100 бит не совпадают";
}