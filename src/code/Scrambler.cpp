
#include "code/Scrambler.h"
#include <bitset>
#include <algorithm>

NBIoTScrambler::NBIoTScrambler(const std::vector<uint8_t>& bits,
                               uint32_t c_init,
                               const std::string& signal_type)
    : bits_(bits)
    , c_init_(c_init)
    , signal_type_(signal_type) {
    
    // Генерируем Gold Sequence
    scramblingSequence_ = genGoldSequence(bits.size(), c_init);
    
    // Скремблируем
    scrambledBits_.resize(bits.size());
    for (size_t i = 0; i < bits.size(); i++) {
        scrambledBits_[i] = bits[i] ^ scramblingSequence_[i];  // XOR
    }
}

std::array<uint8_t, 31> NBIoTScrambler::initX2(uint32_t c_init) {
    std::array<uint8_t, 31> x2;
    // MATLAB: x2(1:31) = fliplr(dec2bin(c_init, 31)) - '0'
    // Это LSB first, поэтому берем биты от младшего к старшему
    for (int i = 0; i < 31; i++) {
        x2[i] = (c_init >> i) & 1;  // LSB first (как fliplr в MATLAB)
    }
    return x2;
}

std::vector<uint8_t> NBIoTScrambler::genGoldSequence(size_t length, uint32_t c_init) {
    size_t totalLength = length + NC;
    
    // ========== Регистр x1 ==========
    // Полином: x1(n+31) = x1(n+3) + x1(n) mod 2
    // Начальное состояние: x1(0) = 1, x1(1..30) = 0
    std::vector<uint8_t> x1(totalLength + 31, 0);
    x1[0] = 1;  // x1(0) = 1
    
    for (size_t n = 0; n < totalLength; n++) {
        x1[n + 31] = x1[n + 3] ^ x1[n];  // XOR = сложение по модулю 2
    }
    
    // ========== Регистр x2 ==========
    // Полином: x2(n+31) = x2(n+3) + x2(n+2) + x2(n+1) + x2(n) mod 2
    // Начальное состояние: из c_init (31 бит)
    std::vector<uint8_t> x2(totalLength + 31, 0);
    
    // Инициализация x2 из c_init (LSB first, как в MATLAB)
    // MATLAB: c_init_bin = dec2bin(c_init, 31); x2(1:31) = fliplr(c_init_bin) - '0';
    for (int i = 0; i < 31; i++) {
        x2[i] = (c_init >> i) & 1;  // LSB first
    }
    
    for (size_t n = 0; n < totalLength; n++) {
        x2[n + 31] = x2[n + 3] ^ x2[n + 2] ^ x2[n + 1] ^ x2[n];
    }
    
    // ========== СОХРАНЯЕМ ПЕРВЫЕ 31 БИТ ДЛЯ ОТЛАДКИ ==========
    for (int i = 0; i < 31; i++) {
        x1_[i] = x1[i];
        x2_[i] = x2[i];
    }
    
    // ========== Генерация Gold Sequence ==========
    std::vector<uint8_t> sequence(length);
    for (size_t n = 0; n < length; n++) {
        sequence[n] = x1[n + NC] ^ x2[n + NC];
    }
    
    return sequence;
}

std::vector<uint8_t> NBIoTScrambler::scramble_seqsubstract(const std::vector<uint8_t>& bits) {
    std::vector<uint8_t> result(bits.size());
    
    for (size_t i = 0; i < bits.size(); i++) {
        if (sequenceOffset_ < scramblingSequence_.size()) {
            result[i] = bits[i] ^ scramblingSequence_[sequenceOffset_];
            sequenceOffset_++;
        }
    }
    
    return result;
}

// ========== ОТЛАДОЧНЫЙ МЕТОД ==========
std::vector<uint8_t> NBIoTScrambler::getGoldSequenceFirst20() const {
    std::vector<uint8_t> first20;
    size_t count = std::min<size_t>(20, scramblingSequence_.size());
    for (size_t i = 0; i < count; i++) {
        first20.push_back(scramblingSequence_[i]);
    }
    return first20;
}