#pragma once

#include <vector>
#include "ComplexNumber.h"

// Добавляет циклический префикс к одному OFDM-символу.
// cpLength — число отсчётов префикса; зажимается в [0, symbol.size()].
// Возвращает вектор длиной symbol.size() + cpLength:
//   [ symbol[N-cp] ... symbol[N-1] | symbol[0] ... symbol[N-1] ]
std::vector<Complex> AddCyclicPrefix(const std::vector<Complex>& symbol, int cpLength);
