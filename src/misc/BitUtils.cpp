
#include "misc/BitUtils.h"
#include <cstddef>

// ========== Строка ↔ Байты ==========

std::vector<uint8_t> BitUtils::stringToBytes(const std::string& bits) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < bits.length(); i += 8) {
        uint8_t byte = 0;
        for (size_t b = 0; b < 8 && i + b < bits.length(); b++) {
            if (bits[i + b] == '1') {
                byte |= (1 << (7 - b));
            }
        }
        bytes.push_back(byte);
    }
    return bytes;
}

std::string BitUtils::bytesToString(const std::vector<uint8_t>& bytes, size_t bitCount) {
    std::string bits;
    bits.reserve(bitCount);
    for (size_t i = 0; i < bitCount; i++) {
        size_t byteIdx = i / 8;
        size_t bitIdx = 7 - (i % 8);
        bits.push_back(((bytes[byteIdx] >> bitIdx) & 1) ? '1' : '0');
    }
    return bits;
}

// ========== Строка ↔ Биты ==========

std::vector<uint8_t> BitUtils::stringToBits(const std::string& str) {
    std::vector<uint8_t> bits;
    bits.reserve(str.length());
    for (char c : str) {
        bits.push_back(c == '1' ? 1 : 0);
    }
    return bits;
}

std::string BitUtils::bitsToString(const std::vector<uint8_t>& bits) {
    std::string str;
    str.reserve(bits.size());
    for (uint8_t bit : bits) {
        str.push_back(bit ? '1' : '0');
    }
    return str;
}

// ========== Вектор битов ↔ Байты ==========

std::vector<uint8_t> BitUtils::bitsToBytes(const std::vector<uint8_t>& bits) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < bits.size(); i += 8) {
        uint8_t byte = 0;
        for (size_t b = 0; b < 8 && i + b < bits.size(); b++) {
            if (bits[i + b]) {
                byte |= (1 << (7 - b));
            }
        }
        bytes.push_back(byte);
    }
    return bytes;
}

std::vector<uint8_t> BitUtils::bytesToBits(const std::vector<uint8_t>& bytes, size_t bitCount) {
    std::vector<uint8_t> bits;
    bits.reserve(bitCount);
    for (size_t i = 0; i < bitCount; i++) {
        size_t byteIdx = i / 8;
        size_t bitIdx = 7 - (i % 8);
        bits.push_back((bytes[byteIdx] >> bitIdx) & 1);
    }
    return bits;
}

// ========== uint16_t ↔ Биты ==========

std::vector<uint8_t> BitUtils::uint16ToBits(uint16_t value) {
    std::vector<uint8_t> bits(16);
    for (int i = 0; i < 16; i++) {
        bits[i] = (value >> (15 - i)) & 1;
    }
    return bits;
}

uint16_t BitUtils::bitsToUint16(const std::vector<uint8_t>& bits) {
    uint16_t value = 0;
    for (size_t i = 0; i < 16 && i < bits.size(); i++) {
        if (bits[i]) {
            value |= (1 << (15 - i));
        }
    }
    return value;
}