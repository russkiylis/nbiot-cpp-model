#include "CyclicPrefix.h"

#include <algorithm>

// Принимает временные отсчёты одного OFDM-символа (выход ОБПФ) и длину префикса.
// Возвращает символ с циклическим префиксом: [CP | symbol],
std::vector<Complex> AddCyclicPrefix(const std::vector<Complex>& symbol, int cpLength) {
    const int n = static_cast<int>(symbol.size());

    // Защита: CP не может быть отрицательным или длиннее самого символа.
    const int cp = std::max(0, std::min(cpLength, n));

    std::vector<Complex> result;
    result.reserve(static_cast<size_t>(n + cp));

    // копируем хвост символа
    for (int i = n - cp; i < n; i++)
        result.push_back(symbol[static_cast<size_t>(i)]);

    // На приёмнике первые cp отсчётов отбрасываются, остаётся ровно n отсчётов для БПФ.
    for (const auto& sample : symbol)
        result.push_back(sample);

    return result;
}
