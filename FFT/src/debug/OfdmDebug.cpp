#include "BitUtils.h"
#include "ComplexNumber.h"
#include "Modulation.h"
#include "OfdmSignal.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace {
constexpr int kBitCount = 64;

void PrintBits(const std::vector<int>& bits) {
    std::cout << "Bits (" << bits.size() << "): ";
    for (int bit : bits) std::cout << bit;
    std::cout << "\n\n";
}

void PrintSymbols(const std::string& title, const std::vector<Complex>& symbols) {
    std::cout << title << " (" << symbols.size() << "):\n";
    for (size_t i = 0; i < symbols.size(); ++i) {
        std::cout << "  [" << std::setw(2) << i << "]"
                  << "  re=" << std::setw(8) << symbols[i].re
                  << "  im=" << std::setw(8) << symbols[i].im
                  << "  |x|=" << std::setw(8) << symbols[i].abs()
                  << "\n";
    }
    std::cout << "\n";
}

std::string ModulationName(ModulationType type) {
    switch (type) {
        case ModulationType::Bpsk:  return "BPSK";
        case ModulationType::Qpsk:  return "QPSK";
        case ModulationType::Psk16: return "16-PSK";
    }
    return "";
}

// Печатает таблицу созвездия: все битовые комбинации и соответствующие символы
void PrintConstellation(ModulationType type) {
    const std::string name = ModulationName(type);
    std::cout << "--- Constellation " << name << " ---\n";
    std::cout << "  bits  |    re    |    im    | angle(deg)\n";

    if (type == ModulationType::Bpsk) {
        for (int b = 0; b < 2; ++b) {
            Complex s = Modulation::BpskMapBit(b);
            double deg = std::atan2(s.im, s.re) * 180.0 / PI;
            std::cout << "  " << b
                      << "     | " << std::setw(8) << s.re
                      << " | " << std::setw(8) << s.im
                      << " | " << std::setw(8) << deg << "\n";
        }
    } else if (type == ModulationType::Qpsk) {
        for (int b0 = 0; b0 < 2; ++b0)
        for (int b1 = 0; b1 < 2; ++b1) {
            Complex s = Modulation::QpskMapBits(b0, b1);
            double deg = std::atan2(s.im, s.re) * 180.0 / PI;
            std::cout << "  " << b0 << b1
                      << "    | " << std::setw(8) << s.re
                      << " | " << std::setw(8) << s.im
                      << " | " << std::setw(8) << deg << "\n";
        }
    } else if (type == ModulationType::Psk16) {
        for (int b0 = 0; b0 < 2; ++b0)
        for (int b1 = 0; b1 < 2; ++b1)
        for (int b2 = 0; b2 < 2; ++b2)
        for (int b3 = 0; b3 < 2; ++b3) {
            Complex s = Modulation::Psk16MapBits(b0, b1, b2, b3);
            double deg = std::atan2(s.im, s.re) * 180.0 / PI;
            std::cout << "  " << b0 << b1 << b2 << b3
                      << " | " << std::setw(8) << s.re
                      << " | " << std::setw(8) << s.im
                      << " | " << std::setw(8) << deg << "\n";
        }
    }
    std::cout << "\n";
}

// Выводит модулированные символы по каналам и итоговый OFDM-сигнал для заданного типа модуляции
void PrintModulationResult(const std::vector<int>& bits, ModulationType type) {
    const std::string name = ModulationName(type);
    std::cout << "==============================\n";
    std::cout << "  " << name << "\n";
    std::cout << "==============================\n\n";

    // Разбиваем биты по каналам и модулируем
    const auto channels = SplitChannels(bits);
    const auto modulated = Modulation::ModulateChannels(channels, type);

    for (size_t ch = 0; ch < modulated.size(); ++ch) {
        PrintSymbols(name + " channel " + std::to_string(ch), modulated[ch]);
    }

    // Итоговый OFDM-сигнал во временной области (после IFFT)
    const auto signal = BuildOfdmSignal(bits, type);
    PrintSymbols("OFDM time signal (" + name + ")", signal);
}
}

int main() {
    std::cout << std::fixed << std::setprecision(4);

    // Таблицы созвездий для каждой модуляции
    for (ModulationType type : { ModulationType::Bpsk, ModulationType::Qpsk, ModulationType::Psk16 }) {
        PrintConstellation(type);
    }

    // Одна случайная последовательность для всех модуляций
    const auto bits = GenerateBitSequence(kBitCount);
    PrintBits(bits);

    for (ModulationType type : { ModulationType::Bpsk, ModulationType::Qpsk, ModulationType::Psk16 }) {
        PrintModulationResult(bits, type);
    }

    return 0;
}
