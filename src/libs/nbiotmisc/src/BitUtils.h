
#pragma once

#include <vector>
#include <string>
#include <cstdint>

/**
 * @brief Утилиты для работы с битами и байтами
 */
class BitUtils {
public:
    // ========== Строка ↔ Байты ==========
    static std::vector<uint8_t> stringToBytes(const std::string& bits);
    static std::string bytesToString(const std::vector<uint8_t>& bytes, size_t bitCount);
    
    // ========== Строка ↔ Биты (вектор 0/1) ==========
    static std::vector<uint8_t> stringToBits(const std::string& str);
    static std::string bitsToString(const std::vector<uint8_t>& bits);
    
    // ========== Вектор битов ↔ Байты ==========
    static std::vector<uint8_t> bitsToBytes(const std::vector<uint8_t>& bits);
    static std::vector<uint8_t> bytesToBits(const std::vector<uint8_t>& bytes, size_t bitCount);
    
    // ========== uint16_t ↔ Биты ==========
    static std::vector<uint8_t> uint16ToBits(uint16_t value);
    static uint16_t bitsToUint16(const std::vector<uint8_t>& bits);
    
    // ========== Синонимы для читаемости ==========
    static std::vector<uint8_t> stringToBitVector(const std::string& bits) {
        return stringToBits(bits);
    }
    
    static std::string bitVectorToString(const std::vector<uint8_t>& bits) {
        return bitsToString(bits);
    }
};