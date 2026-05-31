#pragma once
#include <vector>
#include "ComplexNumber.h"

class Modulation {
public:
    static Complex BpskMapBit(int bit);
    static std::vector<Complex> Bpsk(const std::vector<int>& bits);
    static std::vector<std::vector<Complex>> BpskChannels(const std::vector<std::vector<int>>& channels);
};
