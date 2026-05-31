#pragma once
#include <vector>
#include "ComplexNumber.h"

class FourierTransform {
private:
    static bool PowTwo(int n);
    static int NextPower(int n);
    static std::vector<Complex> PadZeros(const std::vector<Complex>& input);

public:
    static long opsCount; // Счетчик операций

    // Классическое ДПФ (O(N^2))
    static std::vector<Complex> DFT(const std::vector<Complex>& input);

    // Быстрое преобразование Фурье (O(N log N))
    static std::vector<Complex> FFT(const std::vector<Complex>& input);

    // Обратное быстрое преобразование Фурье
    static std::vector<Complex> IFFT(const std::vector<Complex>& input);

    static void resetOps() { opsCount = 0; }

};
