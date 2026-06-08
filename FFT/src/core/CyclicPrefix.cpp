#include "CyclicPrefix.h"

#include <algorithm>

std::vector<Complex> AddCyclicPrefix(const std::vector<Complex>& symbol, int cpLength) {
    const int n = static_cast<int>(symbol.size());
    const int cp = std::max(0, std::min(cpLength, n));

    std::vector<Complex> result;
    result.reserve(static_cast<size_t>(n + cp));

    // Хвост символа длиной cp — циклический префикс
    for (int i = n - cp; i < n; i++)
        result.push_back(symbol[static_cast<size_t>(i)]);

    // Сам символ
    for (const auto& sample : symbol)
        result.push_back(sample);

    return result;
}
