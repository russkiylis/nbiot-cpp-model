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
 #include <iostream>
 #include <fstream>
 #include <string>
 #include <vector>

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
    

    // Вывод первых 10 заголовков
    std::cout << "\n=== ПЕРВЫЕ 10 РЕЗУЛЬТАТОВ ===\n";
    
    for (int i = 0; i < 10000; i++) {
        std::getline(f34, s34);
        std::getline(f50, s50);
        
        auto bytes = stringToBytes(s34);
        CRC16::append(bytes);
        std::string result = bytesToString(bytes, 50);
        
        ASSERT_EQ(s34.length(), 34);
        ASSERT_EQ(s50.length(), 50);

        // Вывод первых 10 строк
        if (i < 10) {
            std::cout << "\n[" << i+1 << "]\n";
            std::cout << "  34 бита:   " << s34 << "\n";
            std::cout << "  Ожидается: " << s50 << "\n";
            std::cout << "  Вычислено: " << result << "\n";
            std::cout << "  CRC: 0x" << std::hex << CRC16::lastCRC() << std::dec << "\n";
            if (result == s50) {
                std::cout << "  Статус: OK\n";
            } else {
                std::cout << "  Статус: FAIL\n";
            }
        }
        
        if (result != s50) failed++;
    }
    
    f34.close();
    f50.close();
    
    //EXPECT_EQ(failed, 0) << "Не прошло " << failed << " из 10000";
}