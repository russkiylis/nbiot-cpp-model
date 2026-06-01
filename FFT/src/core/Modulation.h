#pragma once
#include <vector>
#include "ComplexNumber.h"

enum class ModulationType { Bpsk, Qpsk, Psk16 };

class Modulation {
public:
    static Complex BpskMapBit(int bit);
    static std::vector<Complex> Bpsk(const std::vector<int>& bits);

    static Complex QpskMapBits(int b0, int b1);
    static std::vector<Complex> Qpsk(const std::vector<int>& bits);

    static Complex Psk16MapBits(int b0, int b1, int b2, int b3);
    static std::vector<Complex> Psk16(const std::vector<int>& bits);

    static std::vector<std::vector<Complex>> ModulateChannels(
        const std::vector<std::vector<int>>& channels, ModulationType type);
};
