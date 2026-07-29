#include "Modulation.h"

#include <cmath>

//перевод из кода Гпрея в индекс для дальнейшего вычисления угла
int Modulation::grayToIndex(int gray) {
    int index = 0;
    for (; gray > 0; gray >>= 1)
        index ^= gray;
    return index;
}

Modulation::Modulation(ModulationType type)
    : m_type(type) {}

ModulationType Modulation::type() const {
    return m_type;
}

// В зависимости от типа модуляции, возвращает количество бит на символ
// Нужно для выравнивания потока
int Modulation::bitsPerSymbol() const {
    switch (m_type) {
        case ModulationType::Bpsk:  return 1;
        case ModulationType::Qpsk:  return 2;
        case ModulationType::Psk8:  return 3;
        case ModulationType::Psk16: return 4;
    }
    return 1;
}

// выполняет преобразование одного бита в комплексное число
Complex Modulation::bpskMap(int bit) {
    return bit == 0 ? Complex(1.0, 0.0) : Complex(-1.0, 0.0);
}

// выполняет преобразование двух битов в комплексное число
Complex Modulation::qpskMap(int b0, int b1) {
    int index = grayToIndex(b0 * 2 + b1);
    double angle = PI / 4.0 + index * PI / 2.0;
    return Complex(std::cos(angle), std::sin(angle));
}

// выполняет преобразование трех битов в комплексное число
Complex Modulation::psk8Map(int b0, int b1, int b2) {
    int index = grayToIndex(b0 * 4 + b1 * 2 + b2);
    double angle = PI / 8.0 + index * 2.0 * PI / 8.0;
    return Complex(std::cos(angle), std::sin(angle));
}

Complex Modulation::psk16Map(int b0, int b1, int b2, int b3) {
    int index = grayToIndex(b0 * 8 + b1 * 4 + b2 * 2 + b3);
    double angle = PI / 16.0 + index * 2.0 * PI / 16.0;
    return Complex(std::cos(angle), std::sin(angle));
}

// Основной алгоритм модуляции
std::vector<Complex> Modulation::modulate(const std::vector<int>& bits) const {
    int bps = bitsPerSymbol();

    std::vector<int> padded = bits;
    int rem = static_cast<int>(padded.size()) % bps;
    if (rem != 0)
        padded.resize(padded.size() + (bps - rem), 0);

    std::vector<Complex> symbols;
    symbols.reserve(padded.size() / static_cast<size_t>(bps));

    for (size_t i = 0; i < padded.size(); i += bps) {
        switch (m_type) {
            case ModulationType::Bpsk:
                symbols.push_back(bpskMap(padded[i]));
                break;
            case ModulationType::Qpsk:
                symbols.push_back(qpskMap(padded[i], padded[i + 1]));
                break;
            case ModulationType::Psk8:
                symbols.push_back(psk8Map(padded[i], padded[i + 1], padded[i + 2]));
                break;
            case ModulationType::Psk16:
                symbols.push_back(psk16Map(padded[i], padded[i + 1], padded[i + 2], padded[i + 3]));
                break;
        }
    }

    return symbols;
}
