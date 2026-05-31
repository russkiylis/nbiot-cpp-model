#include "Modulation.h"

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

std::vector<std::vector<Complex>> Modulation::BpskChannels(const std::vector<std::vector<int>>& channels) {
    
    std::vector<std::vector<Complex>> modulated;
    modulated.reserve(channels.size());

    for (const auto& channel : channels) {
        modulated.push_back(Bpsk(channel));
    }

    return modulated;
}
