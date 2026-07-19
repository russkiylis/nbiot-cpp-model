/**
 * @file crc16_bitwise.cpp
 * @brief Побитовая реализация CRC-16.
 * @authors russkiylis, Tobyret101
 * @date 04.07.2026
 * @version 1.0
 * @copyright MIT License
 */

#include "code/crc16_bitwise.h"
#include <sstream>
#include <iomanip>
// Побитовая реализация использует массивы для рассмотрения, однако
// есть иной вариант - использование unsigned long, потенциально даст серьезный прирост
// скорости и памяти

std::vector<uint8_t> CRC16Bitwise::stringToBits(const std::string& str) {
    std::vector<uint8_t> bits;
    bits.reserve(str.length());
    for (char c : str) {
        bits.push_back(c == '1' ? 1 : 0);
    }
    return bits;
}

std::string CRC16Bitwise::bitsToString(const std::vector<uint8_t>& bits) {
    std::string str;
    str.reserve(bits.size());
    for (uint8_t bit : bits) {
        str.push_back(bit ? '1' : '0');
    }
    return str;
}

std::vector<uint8_t> CRC16Bitwise::calculate(const std::vector<uint8_t>& arr) {
    // 1. Дополняем 16 нулями (как в MATLAB: tmp = [arr, zeros(1, 16)])
    std::vector<uint8_t> tmp = arr;
    tmp.resize(arr.size() + 16, 0);
    
    // 2. Полином (как в MATLAB: d = [1 5 12 17])
    //    Индексация в C++ с 0, поэтому вычитаем 1 из каждого элемента
    std::vector<size_t> d = {1, 5, 12, 17};
    for (auto& pos : d) {
        pos--;  // переводим в индексацию с 0
    }
    
    // 3. Основной цикл (как в MATLAB: while d(1) <= length(arr))
    size_t d_idx = 0;
    while (d[d_idx] < arr.size()) {
        // Если в текущей позиции 0 — сдвигаемся
        if (tmp[d[d_idx]] == 0) {
            for (auto& pos : d) pos++;
            continue;
        }
        
        // XOR с полиномом (как в MATLAB: tmp(d) = mod(tmp(d)+1,2))
        for (size_t pos : d) {
            if (pos < tmp.size()) {
                tmp[pos] ^= 1;  // то же самое что mod(tmp+1, 2)
            }
        }
        
        // Сдвигаем полином (как в MATLAB: d = d + 1)
        for (auto& pos : d) pos++;
    }
    
    // 4. Сохраняем CRC (последние 16 бит)
    last_crc_.clear();
    for (size_t i = tmp.size() - 16; i < tmp.size(); i++) {
        last_crc_.push_back(tmp[i]);
    }
    
    // 5. Результат (как в MATLAB: res = [arr, tmp(end-15:end)])
    std::vector<uint8_t> res = arr;
    for (size_t i = tmp.size() - 16; i < tmp.size(); i++) {
        res.push_back(tmp[i]);
    }
    
    return res;
}

std::string CRC16Bitwise::calculate(const std::string& str) {
    auto bits = stringToBits(str);
    auto result = calculate(bits);
    return bitsToString(result);
}

void CRC16Bitwise::reset() {
    last_crc_.clear();
}