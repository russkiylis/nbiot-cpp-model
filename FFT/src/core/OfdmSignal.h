#pragma once

#include <vector>

#include "ComplexNumber.h"
#include "Modulation.h"

std::vector<Complex> BuildOfdmSignal(const std::vector<int>& bits, ModulationType type = ModulationType::Bpsk);
std::vector<Complex> InstantSpectrum(const std::vector<Complex>& signal, int offset, int length);
std::vector<std::vector<Complex>> InstantSpectra(const std::vector<Complex>& signal, int length, int step = 1);
std::vector<Complex> AverageSpectrum(const std::vector<std::vector<Complex>>& spectra);
