#pragma once
#include <vector>
#include "ComplexNumber.h"

enum class ModulationType { Bpsk, Qpsk, Psk8, Psk16 };

class Modulation {
public:
    explicit Modulation(ModulationType type);

    // Маппинг потока бит → вектор символов созвездия
    std::vector<Complex> modulate(const std::vector<int>& bits) const;

    // Сколько бит кодирует один символ (1 / 2 / 4)
    int bitsPerSymbol() const;

    ModulationType type() const;

private:
    ModulationType m_type;

    static int grayToIndex(int gray);
    static Complex bpskMap(int bit);
    static Complex qpskMap(int b0, int b1);
    static Complex psk8Map(int b0, int b1, int b2);
    static Complex psk16Map(int b0, int b1, int b2, int b3);
};
