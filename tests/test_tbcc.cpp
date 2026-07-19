
#include <gtest/gtest.h>
#include "code/tbcc.h"
#include "misc/BitUtils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

TEST(test_TBCC, All10000Sequences) {
    std::string path50 = std::string(INPUT_DIR) + "random_34bit_with_crc.txt";
    std::string path150 = std::string(INPUT_DIR) + "random_34bit_tbcc.txt";
    
    std::ifstream f50(path50);
    std::ifstream f150(path150);
    
    ASSERT_TRUE(f50.is_open()) << "Файл не найден: " << path50;
    ASSERT_TRUE(f150.is_open()) << "Файл не найден: " << path150;
    
    std::string s50, s150;
    int failed = 0;
    int lineCount = 0;
    
    // Создаём объект TBCC
    TBCC tbcc;
    
    // Векторы для хранения первых 5 строк
    std::vector<std::string> first5_s50;
    std::vector<std::string> first5_s150;
    std::vector<std::string> first5_result;
    std::vector<bool> first5_status;
    
    while (std::getline(f50, s50) && std::getline(f150, s150)) {
        lineCount++;
        
        // Удаляем \r для Windows-файлов
        if (!s50.empty() && s50.back() == '\r') s50.pop_back();
        if (!s150.empty() && s150.back() == '\r') s150.pop_back();
        
        // Проверяем длину
        ASSERT_EQ(s50.length(), 50) << "Строка " << lineCount << " имеет длину " << s50.length();
        ASSERT_EQ(s150.length(), 150) << "Строка " << lineCount << " имеет длину " << s150.length();
        
        // Конвертируем 50 бит в вектор
        auto bits = BitUtils::stringToBitVector(s50);
        
        // Вычисляем TBCC
        auto result = tbcc.encode(bits);
        
        // Конвертируем результат в строку
        std::string result_str = BitUtils::bitVectorToString(result);
        
        // Проверка размера
        EXPECT_EQ(result_str.length(), 150) << "Строка " << lineCount << " имеет длину " << result_str.length();
        
        // Сохраняем первые 5 строк для вывода после итога
        if (lineCount <= 5) {
            first5_s50.push_back(s50);
            first5_s150.push_back(s150);
            first5_result.push_back(result_str);
            first5_status.push_back(result_str == s150);
        }
        
        if (result_str != s150) {
            failed++;
        }
    }
    
    f50.close();
    f150.close();
    
    // ========== СНАЧАЛА ВЫВОДИМ ИТОГ ==========
    std::cout << "\n=== ИТОГ ===\n";
    std::cout << "Всего строк: " << lineCount << "\n";
    std::cout << "Не прошло: " << failed << "\n";
    std::cout << "Прошло: " << (lineCount - failed) << "\n";
    if (lineCount > 0) {
        std::cout << "Процент: " << (100.0 * (lineCount - failed) / lineCount) << "%\n";
    }
    
    // ========== ПОТОМ ПЕРВЫЕ 5 СТРОК ==========
    std::cout << "\n=== ПЕРВЫЕ 5 РЕЗУЛЬТАТОВ TBCC ===\n";
    for (size_t i = 0; i < first5_s50.size(); i++) {
        std::cout << "\n[" << i+1 << "]\n";
        std::cout << "  50 бит (вход): " << first5_s50[i] << "\n";
        
        std::string first10_expected = first5_s150[i].substr(0, 10);
        std::string last10_expected = first5_s150[i].substr(first5_s150[i].length() - 10);
        std::string first10_result = first5_result[i].substr(0, 10);
        std::string last10_result = first5_result[i].substr(first5_result[i].length() - 10);
        
        std::cout << "  Ожидается: " << first10_expected << "..." << last10_expected << "\n";
        std::cout << "  Вычислено: " << first10_result << "..." << last10_result << "\n";
        std::cout << "  Статус:  " << (first5_status[i] ? "OK" : "FAIL") << "\n";
        
        if (!first5_status[i]) {
            for (size_t j = 0; j < 150; j++) {
                if (first5_result[i][j] != first5_s150[i][j]) {
                    std::cout << "  Первая ошибка на позиции " << j << "\n";
                    std::cout << "  Ожидалось: " << first5_s150[i][j] << ", получено: " << first5_result[i][j] << "\n";
                    break;
                }
            }
        }
    }
    
    EXPECT_EQ(failed, 0) << "Не прошло " << failed << " из " << lineCount;
}

// Дополнительный тест: проверка одной конкретной строки
TEST(test_TBCC, SingleSequence) {
    // Вход: 50 бит (первая строка из файла)
    std::string input = "10010111110000011011011001110010111001000001101110";
    
    // Ожидаемый выход (из эталонного файла)
    std::string expected = "000000101010101100101010001011111011100010011000100100110011001101100001010111001000101000100001011101010110111100001100001001100000100100110011110001";
    
    TBCC tbcc;
    auto bits = BitUtils::stringToBitVector(input);
    auto result = tbcc.encode(bits);
    
    std::string result_str = BitUtils::bitVectorToString(result);
    
    ASSERT_EQ(result_str.length(), 150);
    EXPECT_EQ(result_str, expected);
}