#include "Modulation.h"

#include <cmath>

namespace {

// Преобразует код Грея в натуральный двоичный индекс.
// Соседние точки на созвездии будут отличаться ровно на 1 бит.
int GrayToIndex(int gray) {
    int index = 0;
    for (; gray > 0; gray >>= 1)
        index ^= gray;
    return index;
}

}

Complex Modulation::BpskMapBit(int bit) {
    if (bit == 0) {
        return Complex(1.0, 0.0);
    }
    
    return Complex(-1.0, 0.0);
}

std::vector<Complex> Modulation::Bpsk(const std::vector<int>& bits) {

    std::vector<Complex> symbols;
    symbols.reserve(bits.size());

    for (int bit : bits) {
        symbols.push_back(BpskMapBit(bit));
    }

    return symbols;
}

Complex Modulation::QpskMapBits(int b0, int b1) {
    int index = GrayToIndex(b0 * 2 + b1);
    double angle = PI / 4.0 + index * PI / 2.0; // точки на 45°, 135°, 225°, 315°
    return Complex(std::cos(angle), std::sin(angle));
}

std::vector<Complex> Modulation::Qpsk(const std::vector<int>& bits) {
    std::vector<Complex> symbols;
    symbols.reserve(bits.size() / 2);
    for (size_t i = 0; i + 1 < bits.size(); i += 2) {
        symbols.push_back(QpskMapBits(bits[i], bits[i + 1]));
    }
    return symbols;
}

Complex Modulation::Psk16MapBits(int b0, int b1, int b2, int b3) {
    int index = GrayToIndex(b0 * 8 + b1 * 4 + b2 * 2 + b3);
    double angle = PI / 16.0 + index * 2.0 * PI / 16.0; // точки начиная с 11.25°, шаг 22.5°
    return Complex(std::cos(angle), std::sin(angle));
}

std::vector<Complex> Modulation::Psk16(const std::vector<int>& bits) {
    std::vector<Complex> symbols;
    symbols.reserve(bits.size() / 4);
    for (size_t i = 0; i + 3 < bits.size(); i += 4) {
        symbols.push_back(Psk16MapBits(bits[i], bits[i + 1], bits[i + 2], bits[i + 3]));
    }
    return symbols;
}

std::vector<std::vector<Complex>> Modulation::ModulateChannels(
    const std::vector<std::vector<int>>& channels, ModulationType type)
{
    std::vector<std::vector<Complex>> modulated;
    modulated.reserve(channels.size());
    for (const auto& channel : channels) {
        switch (type) {
            case ModulationType::Bpsk:  modulated.push_back(Bpsk(channel));  break;
            case ModulationType::Qpsk:  modulated.push_back(Qpsk(channel));  break;
            case ModulationType::Psk16: modulated.push_back(Psk16(channel)); break;
        }
    }
    return modulated;
}
