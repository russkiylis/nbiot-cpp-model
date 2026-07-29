
#include "code/qpsk.h"
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <iomanip>

NBIoTQPSK::NBIoTQPSK(const std::vector<uint8_t>& bits)
    : bits_(bits) {
    symbols_ = modQPSK();
}

NBIoTQPSK::NBIoTQPSK(const std::string& bitsStr) {
    bits_ = stringToBits(bitsStr);
    symbols_ = modQPSK();
}

std::vector<uint8_t> NBIoTQPSK::stringToBits(const std::string& str) {
    std::vector<uint8_t> bits;
    bits.reserve(str.length());
    for (char c : str) {
        if (c == '1') {
            bits.push_back(1);
        } else if (c == '0') {
            bits.push_back(0);
        } else {
            throw std::runtime_error("QPSK: недопустимый символ в строке битов");
        }
    }
    return bits;
}

std::vector<std::complex<float>> NBIoTQPSK::modQPSK() {
    // Проверка чётности
    if (bits_.size() % 2 != 0) {
        throw std::runtime_error("QPSK: количество битов нечётное!");
    }
    
    size_t numSymbols = bits_.size() / 2;
    std::vector<std::complex<float>> symbols;
    symbols.reserve(numSymbols);
    
    for (size_t i = 0; i < bits_.size(); i += 2) {
        uint8_t bit1 = bits_[i];
        uint8_t bit2 = bits_[i + 1];
        
        std::complex<float> symbol;
        
        if (bit1 == 1) {
            if (bit2 == 1) {
                // 11 → (-1 - 1i) / √2
                symbol = std::complex<float>(-INV_SQRT2, -INV_SQRT2);
            } else {
                // 10 → (-1 + 1i) / √2
                symbol = std::complex<float>(-INV_SQRT2, INV_SQRT2);
            }
        } else {
            if (bit2 == 1) {
                // 01 → (1 - 1i) / √2
                symbol = std::complex<float>(INV_SQRT2, -INV_SQRT2);
            } else {
                // 00 → (1 + 1i) / √2
                symbol = std::complex<float>(INV_SQRT2, INV_SQRT2);
            }
        }
        
        symbols.push_back(symbol);
    }
    
    return symbols;
}

std::vector<std::complex<float>> NBIoTQPSK::modulate(const std::vector<uint8_t>& bits) {
    NBIoTQPSK modulator(bits);
    return modulator.getSymbols();
}

std::vector<std::complex<float>> NBIoTQPSK::modulate(const std::string& bitsStr) {
    NBIoTQPSK modulator(bitsStr);
    return modulator.getSymbols();
}

std::string NBIoTQPSK::symbolsToString(const std::vector<std::complex<float>>& symbols,
                                        int precision) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision);
    
    for (size_t i = 0; i < symbols.size(); i++) {
        if (i > 0) {
            oss << ", ";
        }
        oss << "(" << symbols[i].real() << ", " << symbols[i].imag() << "i)";
    }
    
    return oss.str();
}