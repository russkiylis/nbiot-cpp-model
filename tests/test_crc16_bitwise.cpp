/**
 * @file test_crc16_bitwise.cpp
 * @author russkiylis x Tobyret101
 * @brief Тестирование CRC-16
 * @version 0.2
 * @date 04.07.2026
 * 
 */

#include <gtest/gtest.h>
#include "code/crc16_bitwise.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Вспомогательные функции
static std::vector<uint8_t> stringToBytes(const std::string& bits) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < bits.length(); i += 8) {
        uint8_t byte = 0;
        for (size_t b = 0; b < 8 && i + b < bits.length(); b++) {
            if (bits[i + b] == '1') byte |= (1 << (7 - b));
        }
        bytes.push_back(byte);
    }
    return bytes;
}

static std::string bytesToString(const std::vector<uint8_t>& bytes, size_t bitCount) {
    std::string bits;
    for (size_t i = 0; i < bitCount; i++) {
        bits.push_back(((bytes[i / 8] >> (7 - (i % 8))) & 1) ? '1' : '0');
    }
    return bits;
}

// Конвертация строки битов в вектор битов
std::vector<uint8_t> stringToBitVector(const std::string& bits) {
    std::vector<uint8_t> vec;
    vec.reserve(bits.length());
    for (char c : bits) {
        vec.push_back(c == '1' ? 1 : 0);
    }
    return vec;
}

TEST(test_CRC16_bitwise, All10000Sequences) {
    std::string path34 = std::string(INPUT_DIR) + "random_34bit_sequences.txt";
    std::string path50 = std::string(INPUT_DIR) + "random_34bit_with_crc.txt";
    
    std::ifstream f34(path34);
    std::ifstream f50(path50);
    
    ASSERT_TRUE(f34.is_open()) << "Файл не найден: " << path34;
    ASSERT_TRUE(f50.is_open()) << "Файл не найден: " << path50;
    
    std::string s34, s50;
    int failed = 0;
    
    // Создаём объект CRC
    CRC16Bitwise crc;
    
    // Вывод первых 10 заголовков
    std::cout << "\n=== ПЕРВЫЕ 10 РЕЗУЛЬТАТОВ ===\n";
    
    for (int i = 0; i < 10000; i++) {
        if (!std::getline(f34, s34)) break;
        if (!std::getline(f50, s50)) break;
        
        // Удаляем \r для Windows-файлов
        if (!s34.empty() && s34.back() == '\r') s34.pop_back();
        if (!s50.empty() && s50.back() == '\r') s50.pop_back();
        
        ASSERT_EQ(s34.length(), 34) << "Строка " << i << " имеет длину " << s34.length();
        ASSERT_EQ(s50.length(), 50) << "Строка " << i << " имеет длину " << s50.length();
        
        // Конвертируем строку в вектор битов
        auto bits = stringToBitVector(s34);
        
        // Вычисляем CRC через новый класс
        auto result = crc.calculate(bits);
        std::string result_str;
        for (uint8_t b : result) {
            result_str.push_back(b ? '1' : '0');
        }
        
        // Вывод первых 10 строк
        if (i < 10) {
            std::cout << "\n[" << i+1 << "]\n";
            std::cout << "  34 бита:   " << s34 << "\n";
            std::cout << "  Ожидается: " << s50 << "\n";
            std::cout << "  Вычислено: " << result_str << "\n";
            if (result_str == s50) {
                std::cout << "  Статус:  OK\n";
            } else {
                std::cout << "  Статус:  FAIL\n";
            }
        }
        
        if (result_str != s50) {
            failed++;
        }
    }
    
    f34.close();
    f50.close();
    
    std::cout << "\n=== ИТОГ ===\n";
    std::cout << "Не прошло: " << failed << " из 10000\n";
    
    EXPECT_EQ(failed, 0) << "Не прошло " << failed << " из 10000";
}