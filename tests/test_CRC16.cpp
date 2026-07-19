/**
 * @file test_CRC16.cpp
 * @author russkiylis x Tobyret101
 * @brief Тестирование CRC-16
 * @version 0.1
 * @date 07.06.2026
 * 
 */

#include <gtest/gtest.h>
#include "code/crc16.h"
#include "misc/BitUtils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

TEST(test_CRC16, All10000Sequences) {
    CRC16::reset();  // сбросить состояние перед тестом
    
    std::string path34 = std::string(INPUT_DIR) + "random_34bit_sequences.txt";
    std::string path50 = std::string(INPUT_DIR) + "random_34bit_with_crc.txt";
    
    std::ifstream f34(path34);
    std::ifstream f50(path50);
    
    ASSERT_TRUE(f34.is_open()) << "Файл не найден: " << path34;
    ASSERT_TRUE(f50.is_open()) << "Файл не найден: " << path50;
    
    std::string s34, s50;
    int failed = 0;
    int lineCount = 0;
    
    // Векторы для хранения первых 10 строк
    std::vector<std::string> first10_s34;
    std::vector<std::string> first10_s50;
    std::vector<std::string> first10_result;
    std::vector<uint16_t> first10_crc;
    std::vector<bool> first10_status;
    
    while (std::getline(f34, s34) && std::getline(f50, s50)) {
        lineCount++;
        
        // Удаляем \r для Windows-файлов
        if (!s34.empty() && s34.back() == '\r') s34.pop_back();
        if (!s50.empty() && s50.back() == '\r') s50.pop_back();
        
        ASSERT_EQ(s34.length(), 34) << "Строка " << lineCount << " имеет длину " << s34.length();
        ASSERT_EQ(s50.length(), 50) << "Строка " << lineCount << " имеет длину " << s50.length();
        
        auto bytes = BitUtils::stringToBytes(s34);
        CRC16::append(bytes);
        std::string result = BitUtils::bytesToString(bytes, 50);
        
        // Сохраняем первые 10 строк для вывода после итога
        if (lineCount <= 10) {
            first10_s34.push_back(s34);
            first10_s50.push_back(s50);
            first10_result.push_back(result);
            first10_crc.push_back(CRC16::lastCRC());
            first10_status.push_back(result == s50);
        }
        
        if (result != s50) {
            failed++;
        }
    }
    
    f34.close();
    f50.close();
    
    // ========== СНАЧАЛА ВЫВОДИМ ИТОГ ==========
    std::cout << "\n=== ИТОГ ===\n";
    std::cout << "Всего строк: " << lineCount << "\n";
    std::cout << "Не прошло: " << failed << "\n";
    std::cout << "Прошло: " << (lineCount - failed) << "\n";
    if (lineCount > 0) {
        std::cout << "Процент: " << (100.0 * (lineCount - failed) / lineCount) << "%\n";
    }
    
    // ========== ПОТОМ ПЕРВЫЕ 10 СТРОК ==========
    std::cout << "\n=== ПЕРВЫЕ 10 РЕЗУЛЬТАТОВ CRC ===\n";
    for (size_t i = 0; i < first10_s34.size(); i++) {
        std::cout << "\n[" << i+1 << "]\n";
        std::cout << "  34 бита:   " << first10_s34[i] << "\n";
        std::cout << "  Ожидается: " << first10_s50[i] << "\n";
        std::cout << "  Вычислено: " << first10_result[i] << "\n";
        std::cout << "  CRC: 0x" << std::hex << first10_crc[i] << std::dec << "\n";
        std::cout << "  Статус:  " << (first10_status[i] ? "OK" : "FAIL") << "\n";
        
        if (!first10_status[i]) {
            for (size_t j = 0; j < 50; j++) {
                if (first10_result[i][j] != first10_s50[i][j]) {
                    std::cout << "  Первая ошибка на CRC-позиции " << j - 33 << "\n";
                    break;
                }
            }
        }
    }
}